//====-- INODEMCAsmInfo.h - INODE asm properties ---------------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_INODE_MCTARGETDESC_INODEMCASMINFO_H
#define LLVM_LIB_TARGET_INODE_MCTARGETDESC_INODEMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
class Triple;

class INODEMCAsmInfo : public MCAsmInfoELF {
public:
  explicit INODEMCAsmInfo(const Triple &TT);
};

} // end namespace llvm

#endif
