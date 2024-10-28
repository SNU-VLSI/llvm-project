//===- IMCE.cpp -----------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Symbols.h"
#include "Target.h"
#include "OutputSections.h"
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
class IMCE final : public TargetInfo {
public:
  IMCE();
  RelExpr getRelExpr(RelType type, const Symbol &s,
                     const uint8_t *loc) const override;
  void relocate(uint8_t *loc, const Relocation &rel,
                uint64_t val) const override;
};
} // namespace

IMCE::IMCE() {
}

RelExpr IMCE::getRelExpr(RelType type, const Symbol &s,
                           const uint8_t *loc) const {
  switch (type) {
  case R_IMCE_NONE:
    return R_NONE;
  case R_IMCE_PC6:
    return R_PC;
  case R_IMCE_TARGET_26:
    return R_IMCE_ADD_PC_TO_OFFSET;
  default:
    return R_ABS;
  }
}

void IMCE::relocate(uint8_t *loc, const Relocation &rel, uint64_t val) const {
  switch (rel.type) {
  case R_IMCE_PC6: {
    uint32_t mask = 0x00000FC0;
    checkInt(loc, val, 8, rel);
    write32(loc, (read32(loc) & ~mask) | ((val << 4) & mask));
    break;
  }
  case R_IMCE_TARGET_26:
  case R_IMCE_26: {
    uint32_t mask = 0xFFFFFFC0;
    checkIntUInt(loc, val, 26, rel);
    write32(loc, (read32(loc) & ~mask) | ((val << 4) & mask));
    break;
  }
  default:
    error(getErrorLocation(loc) + "unrecognized relocation " +
          toString(rel.type));
  }
}

TargetInfo *elf::getIMCETargetInfo() {
  static IMCE target;
  return &target;
}
