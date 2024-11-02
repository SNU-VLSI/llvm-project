//===-- INODEFrameLowering.cpp - Frame lowering for INODE -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "INODEFrameLowering.h"
#include "INODERegisterInfo.h"
#include "MCTargetDesc/INODEMCTargetDesc.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/Support/Alignment.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

INODEFrameLowering::INODEFrameLowering()
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(4), 0, Align(4),
                          false /* StackRealignable */) {}

void INODEFrameLowering::emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const {
  errs() << "INODEFrameLowering::emitPrologue\n";
}

void INODEFrameLowering::emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const {
  errs() << "INODEFrameLowering::emitEpilogue\n";
}

bool INODEFrameLowering::hasFP(const MachineFunction &MF) const { return true; }

// Returns the register used to hold the frame pointer.
static Register getFPReg(const INODESubtarget &STI) { return INODE::SReg6; }

// Returns the register used to hold the stack pointer.
static Register getSPReg(const INODESubtarget &STI) { return INODE::SReg7; }