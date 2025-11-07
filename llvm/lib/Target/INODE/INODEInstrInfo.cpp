//===-- INODEInstrInfo.cpp - INODE instruction information ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the INODE implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "INODEInstrInfo.h"
#include "INODESubtarget.h"
#include "MCTargetDesc/INODEMCTargetDesc.h"
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
#include "INODEGenInstrInfo.inc"

#define DEBUG_TYPE "INODE-ii"

// Pin the vtable to this file.
void INODEInstrInfo::anchor() {}

INODEInstrInfo::INODEInstrInfo(INODESubtarget &STI) : INODEGenInstrInfo(), RI(), STI(STI) {}

bool INODEInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {
  MachineBasicBlock &MBB = *MI.getParent();

  switch (MI.getOpcode()) {
  default:
    return false;
  case INODE::INODE_RET_INST: {
    MachineInstrBuilder MIB = BuildMI(MBB, &MI, MI.getDebugLoc(), get(INODE::INODE_HALT_INST));

    // Retain any imp-use flags.
    for (auto &MO : MI.operands()) {
      if (MO.isImplicit())
        MIB.add(MO);
    }
    break;
  }
  }

  // Erase the pseudo instruction.
  MBB.erase(MI);
  return true;
}

void INODEInstrInfo::copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                                const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                                bool KillSrc) const {

  const TargetRegisterInfo *TRI = STI.getRegisterInfo();
  if(INODE::SGPRRegClass.contains(DestReg, SrcReg)) {
    BuildMI(MBB, I, DL, get(INODE::INODE_ADDI_INST), DestReg)
        .addReg(SrcReg, getKillRegState(KillSrc))
        .addImm(0);
  } else {
    llvm_unreachable("copy between different register classes is not possible");
  }
}


void INODEInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                         MachineBasicBlock::iterator I,
                                         Register SrcReg, bool IsKill, int FI,
                                         const TargetRegisterClass *RC,
                                         const TargetRegisterInfo *TRI,
                                         Register VReg) const {

  MachineFunction *MF = MBB.getParent();
  MachineFrameInfo &MFI = MF->getFrameInfo();

  unsigned Opcode;
  if (INODE::SGPRRegClass.hasSubClassEq(RC)) {
    Opcode =  INODE::INODE_STORE_INST;
  } else {
    llvm_unreachable("Can't store this register to stack slot");
  }

  MachineMemOperand *MMO = MF->getMachineMemOperand(
      MachinePointerInfo::getFixedStack(*MF, FI), MachineMemOperand::MOStore,
      MFI.getObjectSize(FI), MFI.getObjectAlign(FI));

  BuildMI(MBB, I, DebugLoc(), get(Opcode))
      .addReg(SrcReg, getKillRegState(IsKill))
      .addFrameIndex(FI)
      .addImm(0)
      .addMemOperand(MMO);
}

void INODEInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator I,
                                          Register DstReg, int FI,
                                          const TargetRegisterClass *RC,
                                          const TargetRegisterInfo *TRI,
                                          Register VReg) const {
  MachineFunction *MF = MBB.getParent();
  MachineFrameInfo &MFI = MF->getFrameInfo();

  unsigned Opcode;
  if (INODE::SGPRRegClass.hasSubClassEq(RC)) {
    Opcode = INODE::INODE_LOAD_INST;
  } else {
    llvm_unreachable("Can't load this register from stack slot");
  }

  MachineMemOperand *MMO = MF->getMachineMemOperand(
      MachinePointerInfo::getFixedStack(*MF, FI), MachineMemOperand::MOLoad,
      MFI.getObjectSize(FI), MFI.getObjectAlign(FI));

  BuildMI(MBB, I, DebugLoc(), get(Opcode), DstReg)
      .addFrameIndex(FI)
      .addImm(0)
      .addMemOperand(MMO);
}

INODECC::CondCode INODECC::getOppositeBranchCondition(INODECC::CondCode CC) {
  switch (CC) {
  default:
    llvm_unreachable("Unrecognized conditional branch");
  case INODECC::COND_EQ:
    return INODECC::COND_NE;
  case INODECC::COND_NE:
    return INODECC::COND_EQ;
  case INODECC::COND_LT:
    return INODECC::COND_GE;
  case INODECC::COND_GE:
    return INODECC::COND_LT;
  }
}

unsigned INODECC::getBrCond(INODECC::CondCode CC, bool Imm) {
  switch (CC) {
  default:
    llvm_unreachable("Unknown condition code!");
  case INODECC::COND_EQ:
    return INODE::INODE_BEQ;
  case INODECC::COND_NE:
    return INODE::INODE_BNE;
  case INODECC::COND_LT:
    return INODE::INODE_BLT;
  case INODECC::COND_GE:
    return INODE::INODE_BGE;
  }
}

const MCInstrDesc &INODEInstrInfo::getBrCond(INODECC::CondCode CC, bool Imm) const {
  return get(INODECC::getBrCond(CC, Imm));
}

bool INODEInstrInfo::isReallyTriviallyReMaterializable(const MachineInstr &MI) const {
  // ADDI with an immediate operand that loads a constant is trivially rematerializable.
  // This is cheaper than spilling/reloading from stack.
  if (MI.getOpcode() == INODE::INODE_ADDI_INST) {
    // Check if this is loading an immediate constant (i.e., adding to %sreg0)
    // Format: addi rd, rs1, imm
    if (MI.getNumOperands() >= 3 &&
        MI.getOperand(1).isReg() &&
        MI.getOperand(1).getReg() == INODE::SReg0 &&
        MI.getOperand(2).isImm()) {
      // This is "addi rd, %sreg0, imm" which is just loading a constant
      return true;
    }
  }

  return false;
}