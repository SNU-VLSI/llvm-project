//===-- INODESubtarget.cpp - INODE Subtarget Information ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the INODE specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "INODESubtarget.h"
#include "INODETargetMachine.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/TargetParser/Triple.h"
#include <string>

using namespace llvm;

#define DEBUG_TYPE "INODE-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "INODEGenSubtargetInfo.inc"

void INODESubtarget::anchor() {}

INODESubtarget::INODESubtarget(const Triple &TT, const std::string &CPU, const std::string &FS,
                             const TargetMachine &TM)
    : INODEGenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS), InstrInfo(*this), TLInfo(TM, *this),
      FrameLowering() {}
