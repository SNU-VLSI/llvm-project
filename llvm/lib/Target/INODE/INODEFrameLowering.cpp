//===-- INODEFrameLowering.cpp - Frame lowering for INODE -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "INODEFrameLowering.h"
#include "INODERegisterInfo.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/Support/Alignment.h"

using namespace llvm;

INODEFrameLowering::INODEFrameLowering()
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(8), 0, Align(8),
                          false /* StackRealignable */) {}

void INODEFrameLowering::emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const {}

void INODEFrameLowering::emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const {}

bool INODEFrameLowering::hasFP(const MachineFunction &MF) const { return false; }
