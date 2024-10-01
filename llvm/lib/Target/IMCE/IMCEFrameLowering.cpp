//===-- IMCEFrameLowering.cpp - Frame lowering for IMCE -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "IMCEFrameLowering.h"
#include "IMCERegisterInfo.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/Support/Alignment.h"

using namespace llvm;

IMCEFrameLowering::IMCEFrameLowering()
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(8), 0, Align(8),
                          false /* StackRealignable */) {}

void IMCEFrameLowering::emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const {}

void IMCEFrameLowering::emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const {}

bool IMCEFrameLowering::hasFP(const MachineFunction &MF) const { return false; }
