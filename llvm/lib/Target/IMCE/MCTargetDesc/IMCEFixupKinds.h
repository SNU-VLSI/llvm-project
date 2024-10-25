//===-- IMCEFixupKinds.h - IMCE Specific Fixup Entries --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_IMCE_MCTARGETDESC_IMCEFIXUPKINDS_H
#define LLVM_LIB_TARGET_IMCE_MCTARGETDESC_IMCEFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"
#include <utility>

#undef IMCE

namespace llvm::IMCE {
enum Fixups {
  // PC relative 6-bit fixup for branch resulting in - R_IMCE_PC6
  fixup_imce_PC6 = FirstTargetFixupKind,

  // 26-bit fixup for jump resulting in - R_IMCE_26
  fixup_imce_26,

  // Used as a sentinel, must be the last
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};

} // end namespace llvm::IMCE

#endif
