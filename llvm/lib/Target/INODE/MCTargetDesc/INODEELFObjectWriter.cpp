//===-- INODEELFObjectWriter.cpp - RISC-V ELF Writer ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/INODEFixupKinds.h"
#include "MCTargetDesc/INODEMCExpr.h"
#include "MCTargetDesc/INODEMCTargetDesc.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
class INODEELFObjectWriter : public MCELFObjectTargetWriter {
public:
  INODEELFObjectWriter(uint8_t OSABI, bool Is64Bit);

  ~INODEELFObjectWriter() override;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
};
}

INODEELFObjectWriter::INODEELFObjectWriter(uint8_t OSABI, bool Is64Bit)
    : MCELFObjectTargetWriter(Is64Bit, OSABI, ELF::EM_INODE,
                              /*HasRelocationAddend*/ true) {}

INODEELFObjectWriter::~INODEELFObjectWriter() = default;

unsigned INODEELFObjectWriter::getRelocType(MCContext &Ctx,
                                            const MCValue &Target,
                                            const MCFixup &Fixup,
                                            bool IsPCRel) const {
  // Determine the type of the relocation
  unsigned Kind = Fixup.getTargetKind();
  if (Kind >= FirstLiteralRelocationKind)
    return Kind - FirstLiteralRelocationKind;

  if (IsPCRel) {
    switch (Kind) {
    default:
      Ctx.reportError(Fixup.getLoc(), "unsupported relocation type");
      return ELF::R_INODE_NONE;
    case INODE::fixup_INODE_PC6:
      return ELF::R_INODE_PC6;
    }
  }

  switch (Kind) {
  default:
    Ctx.reportError(Fixup.getLoc(), "unsupported relocation type");
    return ELF::R_INODE_NONE;
  case INODE::fixup_INODE_26:
    return ELF::R_INODE_26;
  case INODE::fixup_INODE_target_26:
    return ELF::R_INODE_TARGET_26;
  }
}

std::unique_ptr<MCObjectTargetWriter> llvm::createINODEELFObjectWriter(uint8_t OSABI, bool Is64Bit) {
  return std::make_unique<INODEELFObjectWriter>(OSABI, Is64Bit);
}
