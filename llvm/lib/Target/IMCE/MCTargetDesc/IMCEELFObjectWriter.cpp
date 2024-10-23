//===-- IMCEELFObjectWriter.cpp - RISC-V ELF Writer ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/IMCEFixupKinds.h"
#include "MCTargetDesc/IMCEMCExpr.h"
#include "MCTargetDesc/IMCEMCTargetDesc.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
class IMCEELFObjectWriter : public MCELFObjectTargetWriter {
public:
  IMCEELFObjectWriter(uint8_t OSABI, bool Is64Bit);

  ~IMCEELFObjectWriter() override;

  // // Return true if the given relocation must be with a symbol rather than
  // // section plus offset.
  // bool needsRelocateWithSymbol(const MCSymbol &Sym,
  //                              unsigned Type) const override {
  //   // TODO: this is very conservative, update once RISC-V psABI requirements
  //   //       are clarified.
  //   return true;
  // }

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
};
}

IMCEELFObjectWriter::IMCEELFObjectWriter(uint8_t OSABI, bool Is64Bit)
    : MCELFObjectTargetWriter(Is64Bit, OSABI, ELF::EM_IMCE,
                              /*HasRelocationAddend*/ true) {}

IMCEELFObjectWriter::~IMCEELFObjectWriter() = default;

unsigned IMCEELFObjectWriter::getRelocType(MCContext &Ctx,
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
      return ELF::R_IMCE_NONE;
    case IMCE::fixup_imce_PC6:
      return ELF::R_IMCE_PC6;
    }
  }

  switch (Kind) {
  default:
    Ctx.reportError(Fixup.getLoc(), "unsupported relocation type");
    return ELF::R_IMCE_NONE;
  case IMCE::fixup_imce_26:
    return ELF::R_IMCE_26;
  }
}

std::unique_ptr<MCObjectTargetWriter> llvm::createIMCEELFObjectWriter(uint8_t OSABI, bool Is64Bit) {
  return std::make_unique<IMCEELFObjectWriter>(OSABI, Is64Bit);
}
