//===-- IMCEInstrInfo.cpp - IMCE instruction information ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the IMCE implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "IMCEInstrInfo.h"
#include "IMCESubtarget.h"
#include "MCTargetDesc/IMCEMCTargetDesc.h"
#include "llvm/CodeGen/LiveIntervals.h"
#include "llvm/CodeGen/LiveVariables.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#define GET_INSTRMAP_INFO
#include "IMCEGenInstrInfo.inc"

#define DEBUG_TYPE "IMCE-ii"

// Pin the vtable to this file.
void IMCEInstrInfo::anchor() {}

IMCEInstrInfo::IMCEInstrInfo(IMCESubtarget &STI) : IMCEGenInstrInfo(), RI(), STI(STI) {}

bool IMCEInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {
  MachineBasicBlock &MBB = *MI.getParent();

  switch (MI.getOpcode()) {
  default:
    return false;
  case IMCE::IMCE_RET_INST: {
    MachineInstrBuilder MIB = BuildMI(MBB, &MI, MI.getDebugLoc(), get(IMCE::IMCE_STOP_INST));

    // Retain any imp-use flags.
    for (auto &MO : MI.operands()) {
      if (MO.isImplicit())
        MIB.add(MO);
    }
    break;
  }
  case IMCE::IMCE_LOAD_LB: {
    BuildMI(MBB, &MI, MI.getDebugLoc(), get(IMCE::IMCE_RECV_INST))
        .addReg(IMCE::V0, RegState::Undef)
        .addImm(MI.getOperand(0).getImm());
    break;
  }
  }

  // Erase the pseudo instruction.
  MBB.erase(MI);
  return true;
}

void IMCEInstrInfo::copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                                const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                                bool KillSrc) const {

  const TargetRegisterInfo *TRI = STI.getRegisterInfo();
  // if (IMCE::SGPRRegClass.contains(DestReg, SrcReg)) {
  //   BuildMI(MBB, I, DL, get(IMCE::IMCE_VADD_INST), DestReg)
  //       .addReg(*TRI->superregs(SrcReg).begin(), getKillRegState(KillSrc))
  //       .addReg(IMCE::V0)
  //       .addImm(0);
  //   return;
  if(IMCE::VGPRRegClass.contains(DestReg, SrcReg)) {
    BuildMI(MBB, I, DL, get(IMCE::IMCE_VADDI_INST), DestReg)
        .addReg(SrcReg, getKillRegState(KillSrc))
        .addImm(0);
  } else {
    // errs() << "Warning : Cannot copy " << TRI->getName(DestReg) << " to " << TRI->getName(SrcReg) << "\n";
    // errs() << "Warning : No Action For Copy " << TRI->getName(DestReg) << " to " << TRI->getName(SrcReg) << "\n";
    errs() << "Warning : copy between different regclass" << TRI->getName(DestReg) << " to " << TRI->getName(SrcReg) << "\n";
    // errs() << "Warning : No Action For Copy " << TRI->getName(DestReg) << " to " << TRI->getName(SrcReg) << "\n";
    BuildMI(MBB, I, DL, get(IMCE::IMCE_VADDI_INST), DestReg)
        .addReg(SrcReg, getKillRegState(KillSrc))
        .addImm(0);
  }

  //TODO :
  // if(TRI->getMinimalPhysRegClass(DestReg) != TRI->getMinimalPhysRegClass(SrcReg)) {
  //   llvm_unreachable("Not implemented yet");
  // }
}

static bool isUncondBranch(unsigned Opc) {
  return Opc == IMCE::IMCE_JMP_INST;
}

static bool isCondBranch(unsigned Opc) {
  return Opc == IMCE::IMCE_BNE_INST;
}

/// analyzeBranch - Analyze the branching code at the end of MBB.
/// Return true if it cannot be understood (e.g. hardware-loop pseudos).
bool IMCEInstrInfo::analyzeBranch(MachineBasicBlock &MBB,
                                  MachineBasicBlock *&TBB,
                                  MachineBasicBlock *&FBB,
                                  SmallVectorImpl<MachineOperand> &Cond,
                                  bool AllowModify) const {
  TBB = nullptr;
  FBB = nullptr;
  Cond.clear();

  MachineBasicBlock::iterator I = MBB.end();
  if (I == MBB.begin())
    return false;

  // Pre-scan: if any terminator is a hardware-loop pseudo, clean up
  // redundant fallthrough JMPs but otherwise declare the block unanalyzable.
  // This prevents BranchFolding from partially modifying the block and then
  // misanalyzing it (the JMP deletion + CLOOP interaction bug).
  {
    bool HasCLOOP = false;
    for (auto &MI : MBB.terminators()) {
      unsigned Opc = MI.getOpcode();
      if (Opc == IMCE::CLOOP_END_BRANCH ||
          Opc == IMCE::CLOOP_BEGIN_TERMINATOR ||
          Opc == IMCE::IMCE_LONG_BNE) {
        HasCLOOP = true;
        break;
      }
    }
    if (HasCLOOP) {
      // Still remove redundant fallthrough JMPs so they don't persist in the
      // final binary -- but do nothing else.
      if (AllowModify) {
        MachineBasicBlock::iterator E = MBB.end();
        if (E != MBB.begin()) {
          --E;
          if (E->isTerminator() && isUncondBranch(E->getOpcode())) {
            MachineBasicBlock *Target = E->getOperand(0).getMBB();
            if (MBB.isLayoutSuccessor(Target))
              E->eraseFromParent();
          }
        }
      }
      return true;
    }
  }

  // Walk backwards over terminators.
  do {
    --I;
    if (!I->isTerminator())
      break;

    unsigned Opc = I->getOpcode();

    if (isUncondBranch(Opc)) {
      MachineBasicBlock *Target = I->getOperand(0).getMBB();

      // If AllowModify, remove fallthrough unconditional jumps.
      if (AllowModify && MBB.isLayoutSuccessor(Target)) {
        I->eraseFromParent();
        I = MBB.end();
        if (I == MBB.begin())
          return false;
        --I;
        if (!I->isTerminator())
          return false;
      }

      if (!TBB) {
        TBB = Target;
        continue;
      }

      // Unconditional branch following a conditional branch: TBB is the
      // conditional target, FBB is the unconditional target.
      // But we're walking backwards, so the unconditional comes first.
      // This means we've seen the unconditional jump (now stored in TBB),
      // and next we should see a conditional branch.
      // For now, return true if we see two unconditional branches.
      return true;
    }

    if (isCondBranch(Opc)) {
      // BNE operand layout:
      //   Op 0: $rs2 (def, HWLOOPREG, tied to $rs1)
      //   Op 1: $imm2 (brtarget MBB)
      //   Op 2: $rs1 (HWLOOPREG)
      //   Op 3: $imm (uimm14)
      MachineBasicBlock *Target = I->getOperand(1).getMBB();

      if (TBB) {
        // TBB was set by the unconditional branch we saw first (walking
        // backwards). That becomes FBB, and this conditional target is TBB.
        FBB = TBB;
        TBB = Target;
      } else {
        TBB = Target;
      }

      // Encode condition: store the def reg, src reg, and immediate so
      // insertBranch can reconstruct the BNE.
      Cond.push_back(I->getOperand(0)); // $rs2 (def reg)
      Cond.push_back(I->getOperand(2)); // $rs1 (HWLOOPREG src)
      Cond.push_back(I->getOperand(3)); // $imm (uimm14)

      continue;
    }

    // Unknown terminator - cannot analyze.
    return true;

  } while (I != MBB.begin());

  return false;
}

unsigned IMCEInstrInfo::removeBranch(MachineBasicBlock &MBB,
                                     int *BytesRemoved) const {
  unsigned Count = 0;

  MachineBasicBlock::iterator I = MBB.end();
  while (I != MBB.begin()) {
    --I;
    if (!I->isTerminator())
      break;

    unsigned Opc = I->getOpcode();
    if (!isUncondBranch(Opc) && !isCondBranch(Opc))
      break;

    I->eraseFromParent();
    I = MBB.end();
    ++Count;
  }

  if (BytesRemoved)
    *BytesRemoved = Count * 4;

  return Count;
}

unsigned IMCEInstrInfo::insertBranch(MachineBasicBlock &MBB,
                                     MachineBasicBlock *TBB,
                                     MachineBasicBlock *FBB,
                                     ArrayRef<MachineOperand> Cond,
                                     const DebugLoc &DL,
                                     int *BytesAdded) const {
  unsigned Count = 0;

  if (Cond.empty()) {
    // Unconditional branch.
    assert(TBB && "insertBranch must not be told to insert a fallthrough");
    assert(!FBB && "Unconditional branch with two targets");
    BuildMI(&MBB, DL, get(IMCE::IMCE_JMP_INST)).addMBB(TBB);
    ++Count;
  } else {
    // Conditional branch: BNE.
    // Cond[0] = $rs2 (def reg), Cond[1] = $rs1 (HWLOOPREG), Cond[2] = $imm
    assert(Cond.size() == 3 && "IMCE branch condition must have 3 operands");
    BuildMI(&MBB, DL, get(IMCE::IMCE_BNE_INST), Cond[0].getReg())
        .addMBB(TBB)          // brtarget $imm2
        .add(Cond[1])         // HWLOOPREGOpnd $rs1
        .add(Cond[2]);        // uimm14 $imm
    ++Count;

    if (FBB) {
      BuildMI(&MBB, DL, get(IMCE::IMCE_JMP_INST)).addMBB(FBB);
      ++Count;
    }
  }

  if (BytesAdded)
    *BytesAdded = Count * 4;

  return Count;
}