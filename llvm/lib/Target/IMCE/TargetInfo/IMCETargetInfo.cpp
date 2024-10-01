//===-- IMCETargetInfo.cpp - IMCE target implementation -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/IMCETargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheIMCETarget() {
  static Target TheIMCETarget;
  return TheIMCETarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeIMCETargetInfo() {
  RegisterTarget<Triple::IMCE, /*HasJIT=*/false> X(getTheIMCETarget(), "IMCE", "IMCE", "IMCE");
}
