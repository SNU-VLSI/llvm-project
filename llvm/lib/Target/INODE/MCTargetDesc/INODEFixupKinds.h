//===-- INODEFixupKinds.h - INODE Specific Fixup Entries --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_INODE_MCTARGETDESC_INODEFIXUPKINDS_H
#define LLVM_LIB_TARGET_INODE_MCTARGETDESC_INODEFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"
#include <utility>

#undef INODE

namespace llvm::INODE {
enum Fixups {
  fixup_INODE_PC9 = FirstTargetFixupKind,
  fixup_INODE_PC20,

  // target-dependent 26-bit fixup for jump resulting in - R_INODE_TARGET_26
  fixup_INODE_target_26,

  // 26-bit fixup for jump resulting in - R_INODE_26
  fixup_INODE_26,

  // Used as a sentinel, must be the last
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};

} // end namespace llvm::INODE

#endif
