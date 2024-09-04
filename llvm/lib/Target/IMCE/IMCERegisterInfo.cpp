//===-- IMCERegisterInfo.cpp - IMCE Register Information
//------------------===//
//
// Part of the LLVM Project, under the Apache License
// v2.0 with LLVM Exceptions. See
// https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH
// LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the IMCE implementation of the
// TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "IMCERegisterInfo.h"
#include "IMCEFrameLowering.h"
#include "MCTargetDesc/IMCEMCTargetDesc.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/Register.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/MC/MCRegister.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "IMCEGenRegisterInfo.inc"

IMCERegisterInfo::IMCERegisterInfo() : IMCEGenRegisterInfo(IMCE::V1) {}

const MCPhysReg *IMCERegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_IMCE_SaveList;
}

BitVector IMCERegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  // R31 is the stack pointer.
  Reserved.set(IMCE::V31);

  return Reserved;
}

bool IMCERegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                                           unsigned FIOperandNum, RegScavenger *RS) const {
  return false;
}

Register IMCERegisterInfo::getFrameRegister(const MachineFunction &MF) const { return IMCE::V30; }
