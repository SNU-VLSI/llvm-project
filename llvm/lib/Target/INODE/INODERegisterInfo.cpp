//===-- INODERegisterInfo.cpp - INODE Register Information
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
// This file contains the INODE implementation of the
// TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "INODERegisterInfo.h"
#include "INODEFrameLowering.h"
#include "MCTargetDesc/INODEMCTargetDesc.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/Register.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/MC/MCRegister.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "INODEGenRegisterInfo.inc"

INODERegisterInfo::INODERegisterInfo() : INODEGenRegisterInfo(INODE::V1) {}

const MCPhysReg *INODERegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_INODE_SaveList;
}

BitVector INODERegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  // R31 is the stack pointer.
  Reserved.set(INODE::V31);
  // markSuperRegs(Reserved, INODE::S0); // zero
  markSuperRegs(Reserved, INODE::V0); // zero

  return Reserved;
}

bool INODERegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                                           unsigned FIOperandNum, RegScavenger *RS) const {
  return false;
}

Register INODERegisterInfo::getFrameRegister(const MachineFunction &MF) const { return INODE::V30; }
