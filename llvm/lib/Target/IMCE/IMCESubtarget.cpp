//===-- IMCESubtarget.cpp - IMCE Subtarget Information ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the IMCE specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "IMCESubtarget.h"
#include "IMCETargetMachine.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/TargetParser/Triple.h"
#include <string>

using namespace llvm;

#define DEBUG_TYPE "IMCE-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "IMCEGenSubtargetInfo.inc"

void IMCESubtarget::anchor() {}

IMCESubtarget::IMCESubtarget(const Triple &TT, const std::string &CPU, const std::string &FS,
                             const TargetMachine &TM)
    : IMCEGenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS), InstrInfo(*this), TLInfo(TM, *this),
      FrameLowering() {}
