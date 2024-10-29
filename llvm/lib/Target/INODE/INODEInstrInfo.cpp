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
    MachineInstrBuilder MIB = BuildMI(MBB, &MI, MI.getDebugLoc(), get(INODE::INODE_STOP_INST));

    // Retain any imp-use flags.
    for (auto &MO : MI.operands()) {
      if (MO.isImplicit())
        MIB.add(MO);
    }
    break;
  }
  case INODE::INODE_LOAD_LB: {
    BuildMI(MBB, &MI, MI.getDebugLoc(), get(INODE::INODE_RECV_INST))
        .addReg(INODE::V0, RegState::Undef)
        .addImm(MI.getOperand(0).getImm());
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
  // if (INODE::SGPRRegClass.contains(DestReg, SrcReg)) {
  //   BuildMI(MBB, I, DL, get(INODE::INODE_VADD_INST), DestReg)
  //       .addReg(*TRI->superregs(SrcReg).begin(), getKillRegState(KillSrc))
  //       .addReg(INODE::V0)
  //       .addImm(0);
  //   return;
  if(INODE::VGPRRegClass.contains(DestReg, SrcReg)) {
    BuildMI(MBB, I, DL, get(INODE::INODE_VADDI_INST), DestReg)
        .addReg(SrcReg, getKillRegState(KillSrc))
        .addImm(0);
  } else {
    // errs() << "Warning : Cannot copy " << TRI->getName(DestReg) << " to " << TRI->getName(SrcReg) << "\n";
    // errs() << "Warning : No Action For Copy " << TRI->getName(DestReg) << " to " << TRI->getName(SrcReg) << "\n";
    errs() << "Warning : copy between different regclass" << TRI->getName(DestReg) << " to " << TRI->getName(SrcReg) << "\n";
    // errs() << "Warning : No Action For Copy " << TRI->getName(DestReg) << " to " << TRI->getName(SrcReg) << "\n";
    BuildMI(MBB, I, DL, get(INODE::INODE_VADDI_INST), DestReg)
        .addReg(SrcReg, getKillRegState(KillSrc))
        .addImm(0);
  }

  //TODO :
  // if(TRI->getMinimalPhysRegClass(DestReg) != TRI->getMinimalPhysRegClass(SrcReg)) {
  //   llvm_unreachable("Not implemented yet");
  // }
}