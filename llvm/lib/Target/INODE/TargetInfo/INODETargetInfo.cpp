//===-- INODETargetInfo.cpp - INODE target implementation -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/INODETargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheINODETarget() {
  static Target TheINODETarget;
  return TheINODETarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeINODETargetInfo() {
  RegisterTarget<Triple::INODE, /*HasJIT=*/false> X(getTheINODETarget(), "inode", "INODE Description", "INODE");
}
