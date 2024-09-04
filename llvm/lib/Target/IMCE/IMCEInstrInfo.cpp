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
  case IMCE::RET: {
    MachineInstrBuilder MIB =
        BuildMI(MBB, &MI, MI.getDebugLoc(), get(IMCE::JMP)).addReg(IMCE::S1, RegState::Undef);

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

void IMCEInstrInfo::copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                                const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                                bool KillSrc) const {

  // MachineInstrBuilder MIB = BuildMI(MBB, I, DL, get(IMCE::ADD));

  // if (DestReg)
  //   MIB.addReg(DestReg, RegState::Define);

  // if (SrcReg)
  //   MIB.addReg(SrcReg, getKillRegState(KillSrc));
}