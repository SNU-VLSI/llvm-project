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
  // 20-bit fixup corresponding to %hi(foo) for instructions like lui
  fixup_imce_hi20 = FirstTargetFixupKind,
  // 12-bit fixup corresponding to %lo(foo) for instructions like addi
  fixup_imce_lo12_i,
  // 12-bit fixup corresponding to foo-bar for instructions like addi
  fixup_imce_12_i,
  // 12-bit fixup corresponding to %lo(foo) for the S-type store instructions
  fixup_imce_lo12_s,
  // 20-bit fixup corresponding to %pcrel_hi(foo) for instructions like auipc
  fixup_imce_pcrel_hi20,
  // 12-bit fixup corresponding to %pcrel_lo(foo) for instructions like addi
  fixup_imce_pcrel_lo12_i,
  // 12-bit fixup corresponding to %pcrel_lo(foo) for the S-type store
  // instructions
  fixup_imce_pcrel_lo12_s,
  // 20-bit fixup corresponding to %got_pcrel_hi(foo) for instructions like
  // auipc
  fixup_imce_got_hi20,
  // 20-bit fixup corresponding to %tprel_hi(foo) for instructions like lui
  fixup_imce_tprel_hi20,
  // 12-bit fixup corresponding to %tprel_lo(foo) for instructions like addi
  fixup_imce_tprel_lo12_i,
  // 12-bit fixup corresponding to %tprel_lo(foo) for the S-type store
  // instructions
  fixup_imce_tprel_lo12_s,
  // Fixup corresponding to %tprel_add(foo) for PseudoAddTPRel, used as a linker
  // hint
  fixup_imce_tprel_add,
  // 20-bit fixup corresponding to %tls_ie_pcrel_hi(foo) for instructions like
  // auipc
  fixup_imce_tls_got_hi20,
  // 20-bit fixup corresponding to %tls_gd_pcrel_hi(foo) for instructions like
  // auipc
  fixup_imce_tls_gd_hi20,
  // 20-bit fixup for symbol references in the jal instruction
  fixup_imce_jal,
  // 12-bit fixup for symbol references in the branch instructions
  fixup_imce_branch,
  // 11-bit fixup for symbol references in the compressed jump instruction
  fixup_imce_rvc_jump,
  // 8-bit fixup for symbol references in the compressed branch instruction
  fixup_imce_rvc_branch,
  // Fixup representing a legacy no-pic function call attached to the auipc
  // instruction in a pair composed of adjacent auipc+jalr instructions.
  fixup_imce_call,
  // Fixup representing a function call attached to the auipc instruction in a
  // pair composed of adjacent auipc+jalr instructions.
  fixup_imce_call_plt,
  // Used to generate an R_IMCE_RELAX relocation, which indicates the linker
  // may relax the instruction pair.
  fixup_imce_relax,
  // Used to generate an R_IMCE_ALIGN relocation, which indicates the linker
  // should fixup the alignment after linker relaxation.
  fixup_imce_align,
  // 8-bit fixup corresponding to R_IMCE_SET8 for local label assignment.
  fixup_imce_set_8,
  // 8-bit fixup corresponding to R_IMCE_ADD8 for 8-bit symbolic difference
  // paired relocations.
  fixup_imce_add_8,
  // 8-bit fixup corresponding to R_IMCE_SUB8 for 8-bit symbolic difference
  // paired relocations.
  fixup_imce_sub_8,
  // 16-bit fixup corresponding to R_IMCE_SET16 for local label assignment.
  fixup_imce_set_16,
  // 16-bit fixup corresponding to R_IMCE_ADD16 for 16-bit symbolic difference
  // paired reloctions.
  fixup_imce_add_16,
  // 16-bit fixup corresponding to R_IMCE_SUB16 for 16-bit symbolic difference
  // paired reloctions.
  fixup_imce_sub_16,
  // 32-bit fixup corresponding to R_IMCE_SET32 for local label assignment.
  fixup_imce_set_32,
  // 32-bit fixup corresponding to R_IMCE_ADD32 for 32-bit symbolic difference
  // paired relocations.
  fixup_imce_add_32,
  // 32-bit fixup corresponding to R_IMCE_SUB32 for 32-bit symbolic difference
  // paired relocations.
  fixup_imce_sub_32,
  // 64-bit fixup corresponding to R_IMCE_ADD64 for 64-bit symbolic difference
  // paired relocations.
  fixup_imce_add_64,
  // 64-bit fixup corresponding to R_IMCE_SUB64 for 64-bit symbolic difference
  // paired relocations.
  fixup_imce_sub_64,
  // 6-bit fixup corresponding to R_IMCE_SET6 for local label assignment in
  // DWARF CFA.
  fixup_imce_set_6b,
  // 6-bit fixup corresponding to R_IMCE_SUB6 for local label assignment in
  // DWARF CFA.
  fixup_imce_sub_6b,

  // Used as a sentinel, must be the last
  fixup_imce_invalid,
  NumTargetFixupKinds = fixup_imce_invalid - FirstTargetFixupKind
};

static inline std::pair<MCFixupKind, MCFixupKind>
getRelocPairForSize(unsigned Size) {
  switch (Size) {
  default:
    llvm_unreachable("unsupported fixup size");
  case 1:
    return std::make_pair(MCFixupKind(IMCE::fixup_imce_add_8),
                          MCFixupKind(IMCE::fixup_imce_sub_8));
  case 2:
    return std::make_pair(MCFixupKind(IMCE::fixup_imce_add_16),
                          MCFixupKind(IMCE::fixup_imce_sub_16));
  case 4:
    return std::make_pair(MCFixupKind(IMCE::fixup_imce_add_32),
                          MCFixupKind(IMCE::fixup_imce_sub_32));
  case 8:
    return std::make_pair(MCFixupKind(IMCE::fixup_imce_add_64),
                          MCFixupKind(IMCE::fixup_imce_sub_64));
  }
}

} // end namespace llvm::IMCE

#endif
