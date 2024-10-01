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

  // Return true if the given relocation must be with a symbol rather than
  // section plus offset.
  bool needsRelocateWithSymbol(const MCSymbol &Sym,
                               unsigned Type) const override {
    // TODO: this is very conservative, update once RISC-V psABI requirements
    //       are clarified.
    return true;
  }

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
  const MCExpr *Expr = Fixup.getValue();
  // Determine the type of the relocation
  unsigned Kind = Fixup.getTargetKind();
  if (Kind >= FirstLiteralRelocationKind)
    return Kind - FirstLiteralRelocationKind;
  if (IsPCRel) {
    switch (Kind) {
    default:
      Ctx.reportError(Fixup.getLoc(), "unsupported relocation type");
      return ELF::R_IMCE_NONE;
    case FK_Data_4:
    case FK_PCRel_4:
      return Target.getAccessVariant() == MCSymbolRefExpr::VK_PLT
                 ? ELF::R_IMCE_PLT32
                 : ELF::R_IMCE_32_PCREL;
    case IMCE::fixup_imce_pcrel_hi20:
      return ELF::R_IMCE_PCREL_HI20;
    case IMCE::fixup_imce_pcrel_lo12_i:
      return ELF::R_IMCE_PCREL_LO12_I;
    case IMCE::fixup_imce_pcrel_lo12_s:
      return ELF::R_IMCE_PCREL_LO12_S;
    case IMCE::fixup_imce_got_hi20:
      return ELF::R_IMCE_GOT_HI20;
    case IMCE::fixup_imce_tls_got_hi20:
      return ELF::R_IMCE_TLS_GOT_HI20;
    case IMCE::fixup_imce_tls_gd_hi20:
      return ELF::R_IMCE_TLS_GD_HI20;
    case IMCE::fixup_imce_jal:
      return ELF::R_IMCE_JAL;
    case IMCE::fixup_imce_branch:
      return ELF::R_IMCE_BRANCH;
    case IMCE::fixup_imce_rvc_jump:
      return ELF::R_IMCE_RVC_JUMP;
    case IMCE::fixup_imce_rvc_branch:
      return ELF::R_IMCE_RVC_BRANCH;
    case IMCE::fixup_imce_call:
      return ELF::R_IMCE_CALL_PLT;
    case IMCE::fixup_imce_call_plt:
      return ELF::R_IMCE_CALL_PLT;
    case IMCE::fixup_imce_add_8:
      return ELF::R_IMCE_ADD8;
    case IMCE::fixup_imce_sub_8:
      return ELF::R_IMCE_SUB8;
    case IMCE::fixup_imce_add_16:
      return ELF::R_IMCE_ADD16;
    case IMCE::fixup_imce_sub_16:
      return ELF::R_IMCE_SUB16;
    case IMCE::fixup_imce_add_32:
      return ELF::R_IMCE_ADD32;
    case IMCE::fixup_imce_sub_32:
      return ELF::R_IMCE_SUB32;
    case IMCE::fixup_imce_add_64:
      return ELF::R_IMCE_ADD64;
    case IMCE::fixup_imce_sub_64:
      return ELF::R_IMCE_SUB64;
    }
  }

  switch (Kind) {
  default:
    Ctx.reportError(Fixup.getLoc(), "unsupported relocation type");
    return ELF::R_IMCE_NONE;
  case FK_Data_1:
    Ctx.reportError(Fixup.getLoc(), "1-byte data relocations not supported");
    return ELF::R_IMCE_NONE;
  case FK_Data_2:
    Ctx.reportError(Fixup.getLoc(), "2-byte data relocations not supported");
    return ELF::R_IMCE_NONE;
  case FK_Data_4:
    if (Expr->getKind() == MCExpr::Target &&
        cast<IMCEMCExpr>(Expr)->getKind() == IMCEMCExpr::VK_IMCE_32_PCREL)
      return ELF::R_IMCE_32_PCREL;
    return ELF::R_IMCE_32;
  case FK_Data_8:
    return ELF::R_IMCE_64;
  case IMCE::fixup_imce_hi20:
    return ELF::R_IMCE_HI20;
  case IMCE::fixup_imce_lo12_i:
    return ELF::R_IMCE_LO12_I;
  case IMCE::fixup_imce_lo12_s:
    return ELF::R_IMCE_LO12_S;
  case IMCE::fixup_imce_tprel_hi20:
    return ELF::R_IMCE_TPREL_HI20;
  case IMCE::fixup_imce_tprel_lo12_i:
    return ELF::R_IMCE_TPREL_LO12_I;
  case IMCE::fixup_imce_tprel_lo12_s:
    return ELF::R_IMCE_TPREL_LO12_S;
  case IMCE::fixup_imce_tprel_add:
    return ELF::R_IMCE_TPREL_ADD;
  case IMCE::fixup_imce_relax:
    return ELF::R_IMCE_RELAX;
  case IMCE::fixup_imce_align:
    return ELF::R_IMCE_ALIGN;
  case IMCE::fixup_imce_set_6b:
    return ELF::R_IMCE_SET6;
  case IMCE::fixup_imce_sub_6b:
    return ELF::R_IMCE_SUB6;
  case IMCE::fixup_imce_add_8:
    return ELF::R_IMCE_ADD8;
  case IMCE::fixup_imce_set_8:
    return ELF::R_IMCE_SET8;
  case IMCE::fixup_imce_sub_8:
    return ELF::R_IMCE_SUB8;
  case IMCE::fixup_imce_set_16:
    return ELF::R_IMCE_SET16;
  case IMCE::fixup_imce_add_16:
    return ELF::R_IMCE_ADD16;
  case IMCE::fixup_imce_sub_16:
    return ELF::R_IMCE_SUB16;
  case IMCE::fixup_imce_set_32:
    return ELF::R_IMCE_SET32;
  case IMCE::fixup_imce_add_32:
    return ELF::R_IMCE_ADD32;
  case IMCE::fixup_imce_sub_32:
    return ELF::R_IMCE_SUB32;
  case IMCE::fixup_imce_add_64:
    return ELF::R_IMCE_ADD64;
  case IMCE::fixup_imce_sub_64:
    return ELF::R_IMCE_SUB64;
  }
}

std::unique_ptr<MCObjectTargetWriter> llvm::createIMCEELFObjectWriter(uint8_t OSABI, bool Is64Bit) {
  return std::make_unique<IMCEELFObjectWriter>(OSABI, Is64Bit);
}
