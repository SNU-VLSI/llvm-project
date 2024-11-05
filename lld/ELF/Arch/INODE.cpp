//===- INODE.cpp
//-----------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "OutputSections.h"
#include "Symbols.h"
#include "Target.h"
#include "lld/Common/ErrorHandler.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/Support/Endian.h"

using namespace llvm;
using namespace llvm::object;
using namespace llvm::support::endian;
using namespace llvm::ELF;
using namespace lld;
using namespace lld::elf;

namespace {
class INODE final : public TargetInfo {
public:
  INODE();
  RelExpr getRelExpr(RelType type, const Symbol &s,
                     const uint8_t *loc) const override;
  void relocate(uint8_t *loc, const Relocation &rel,
                uint64_t val) const override;
};
} // namespace

INODE::INODE() {}

RelExpr INODE::getRelExpr(RelType type, const Symbol &s,
                          const uint8_t *loc) const {
  switch (type) {
  case R_INODE_NONE:
    return R_NONE;
  case R_INODE_PC9:
  case R_INODE_PC20:
    return R_PC;
  case R_INODE_TARGET_26:
    return R_INODE_ADD_PC_TO_OFFSET;
  default:
    return R_ABS;
  }
}

void INODE::relocate(uint8_t *loc, const Relocation &rel, uint64_t val) const {
  switch (rel.type) {
  case R_INODE_PC9: {
    val = val / 4;
    int Offset = 23;
    int Width = 9;
    uint32_t mask = (1 << (Offset + Width)) - (1 << Offset);
    checkInt(loc, val, 9, rel);
    write32(loc, (read32(loc) & ~mask) | ((val << Offset) & mask));
    break;
  }
  case R_INODE_PC20: {
    val = val / 4;
    int Offset = 12;
    int Width = 20;
    uint32_t mask = (1L << (Offset + Width)) - (1 << Offset);
    checkInt(loc, val, 20, rel);
    write32(loc, (read32(loc) & ~mask) | ((val << Offset) & mask));
    break;
  }
  case R_INODE_TARGET_26:
  case R_INODE_26: {
    val = val / 4;
    int Offset = 6;
    int Width = 26;
    uint32_t mask = (1L << (Offset + Width)) - (1 << Offset);
    checkIntUInt(loc, val, 26, rel);
    write32(loc, (read32(loc) & ~mask) | ((val << Offset) & mask));
    break;
  }
  default:
    error(getErrorLocation(loc) + "unrecognized relocation " +
          toString(rel.type));
  }
}

TargetInfo *elf::getINODETargetInfo() {
  static INODE target;
  return &target;
}
