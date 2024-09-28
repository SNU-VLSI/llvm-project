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
    MachineInstrBuilder MIB = BuildMI(MBB, &MI, MI.getDebugLoc(), get(IMCE::IMCE_JMP_INST))
                                  .addReg(IMCE::V1, RegState::Undef);

    // Retain any imp-use flags.
    for (auto &MO : MI.operands()) {
      if (MO.isImplicit())
        MIB.add(MO);
    }
    break;
  }
  // case IMCE::Pseudo_IMCE_VADD_INST_one_lane : {
  //   MachineInstrBuilder MIB = BuildMI(MBB, &MI, MI.getDebugLoc(), get(IMCE::IMCE_VADD_INST))
  //                                 .add(MI.getOperand(0))
  //                                 .add(MI.getOperand(1))
  //                                 .add(MI.getOperand(2))
  //                                 .addImm(0);
  //   break;
  // }
  // case IMCE::Pseudo_IMCE_VADD_INST_all_lane: {
  //   MachineInstrBuilder MIB = BuildMI(MBB, &MI, MI.getDebugLoc(), get(IMCE::IMCE_VADD_INST))
  //                                 .add(MI.getOperand(0))
  //                                 .add(MI.getOperand(1))
  //                                 .add(MI.getOperand(2))
  //                                 .addImm(15);
  // }
 
  // case IMCE::Pseudo_IMCE_VADDI_INST_SCALAR: {
  //   MachineInstrBuilder MIB = BuildMI(MBB, &MI, MI.getDebugLoc(), get(IMCE::IMCE_VADDI_INST))
  //                                 .add(MI.getOperand(0))
  //                                 .add(MI.getOperand(1))
  //                                 .add(MI.getOperand(2));
  //   break;
  // }
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
    BuildMI(MBB, I, DL, get(IMCE::IMCE_VADD_INST), DestReg)
        .addReg(SrcReg, getKillRegState(KillSrc))
        .addReg(IMCE::V0)
        .addImm(15);
  } else {
    errs() << "Warning : Cannot copy " << TRI->getName(DestReg) << " to " << TRI->getName(SrcReg) << "\n";
    errs() << "Warning : No Action For Copy " << TRI->getName(DestReg) << " to " << TRI->getName(SrcReg) << "\n";
  }

  //TODO : 
  // if(TRI->getMinimalPhysRegClass(DestReg) != TRI->getMinimalPhysRegClass(SrcReg)) {
  //   llvm_unreachable("Not implemented yet");
  // }
}