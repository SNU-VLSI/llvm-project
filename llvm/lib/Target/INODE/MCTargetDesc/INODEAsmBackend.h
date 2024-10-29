//===-- INODEAsmBackend.h - RISC-V Assembler Backend ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_INODE_MCTARGETDESC_INODEASMBACKEND_H
#define LLVM_LIB_TARGET_INODE_MCTARGETDESC_INODEASMBACKEND_H

#include "MCTargetDesc/INODEBaseInfo.h"
#include "MCTargetDesc/INODEFixupKinds.h"
#include "MCTargetDesc/INODEMCTargetDesc.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"

namespace llvm {
class MCAssembler;
class MCObjectTargetWriter;
class raw_ostream;

class INODEAsmBackend : public MCAsmBackend {
  const MCSubtargetInfo &STI;
  uint8_t OSABI;
  bool Is64Bit;
  bool ForceRelocs = false;
  const MCTargetOptions &TargetOptions;

public:
  INODEAsmBackend(const MCSubtargetInfo &STI, uint8_t OSABI, bool Is64Bit,
                  const MCTargetOptions &Options)
      : MCAsmBackend(llvm::endianness::big), STI(STI),
        OSABI(OSABI), Is64Bit(Is64Bit), TargetOptions(Options) {}
  ~INODEAsmBackend() override = default;

  const MCTargetOptions &getTargetOptions() const { return TargetOptions; }
  std::unique_ptr<MCObjectTargetWriter> createObjectTargetWriter() const override;
  unsigned getNumFixupKinds() const override { return INODE::NumTargetFixupKinds; }

  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override;

  bool evaluateTargetFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                           const MCFragment *DF, const MCValue &Target,
                           const MCSubtargetInfo *STI, uint64_t &Value,
                           bool &WasForced) override;

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override;

  bool mayNeedRelaxation(const MCInst &Inst,
                         const MCSubtargetInfo &STI) const override;

  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value) const override;

  void relaxInstruction(MCInst &Inst,
                        const MCSubtargetInfo &STI) const override;

  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override;
};
}

#endif
