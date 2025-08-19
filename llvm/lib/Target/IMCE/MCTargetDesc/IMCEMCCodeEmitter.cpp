//===-- IMCEMCCodeEmitter.cpp - Convert IMCE code to machine code ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the IMCEMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include "IMCEMCCodeEmitter.h"
#include "MCTargetDesc/IMCEMCTargetDesc.h"
#include "MCTargetDesc/IMCEFixupKinds.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/raw_ostream.h"
#include <cassert>

using namespace llvm;

#define DEBUG_TYPE "mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");

namespace llvm {
MCCodeEmitter *createIMCEMCCodeEmitter(const MCInstrInfo &MCII, MCContext &Ctx) {
  return new IMCEMCCodeEmitter(MCII, Ctx);
}

} // namespace llvm

// Expand IMCE_LONG_BNE to a BNE, JMP, JUMP instruction sequence.
void IMCEMCCodeEmitter::expandLongBNE(const MCInst &MI,
                                          SmallVectorImpl<char> &CB,
                                          SmallVectorImpl<MCFixup> &Fixups,
                                          const MCSubtargetInfo &STI) const {
  MCRegister OutReg = MI.getOperand(0).getReg();
  MCOperand SrcSymbol = MI.getOperand(1);
  MCRegister SrcReg = MI.getOperand(2).getReg();
  MCRegister SrcImm = MI.getOperand(3).getImm();

  // Emit a bne where if not taken, proceed to first JMP_INST, if taken jump to the second.
  MCInst TmpInst =
      MCInstBuilder(IMCE::IMCE_BNE_INST).addReg(OutReg).addImm(8).addReg(SrcReg).addImm(SrcImm);
  uint32_t Binary = getBinaryCodeForInstr(TmpInst, Fixups, STI);
  support::endian::write(CB, Binary, llvm::endianness::little);

  // Emit an unconditional jump to skip the next instruction.
  int64_t TargetOffset = 8;
  const MCExpr *OffsetExpr = MCConstantExpr::create(TargetOffset, Ctx);

  TmpInst =
      MCInstBuilder(IMCE::IMCE_JMP_INST).addExpr(OffsetExpr);
  Binary = getBinaryCodeForInstr(TmpInst, Fixups, STI);
  support::endian::write(CB, Binary, llvm::endianness::little);

  // Emit an unconditional jump to the destination.
  TmpInst =
      MCInstBuilder(IMCE::IMCE_JMP_INST).addOperand(SrcSymbol);
  Binary = getBinaryCodeForInstr(TmpInst, Fixups, STI);
  support::endian::write(CB, Binary, llvm::endianness::little);

  // override the fixups.
  Fixups.clear();
  uint32_t Offset = 4;
  Fixups.push_back(MCFixup::create(Offset, OffsetExpr,
                                    MCFixupKind(IMCE::fixup_imce_target_26),
                                    MI.getLoc()));
  if (SrcSymbol.isExpr()) {
    Offset = 8;
    Fixups.push_back(MCFixup::create(Offset, SrcSymbol.getExpr(),
                                     MCFixupKind(IMCE::fixup_imce_26),
                                     MI.getLoc()));
  }
}

void IMCEMCCodeEmitter::encodeInstruction(const MCInst &MI, SmallVectorImpl<char> &CB,
                                          SmallVectorImpl<MCFixup> &Fixups,
                                          const MCSubtargetInfo &STI) const {
  switch (MI.getOpcode()) {
  default:
    break;
  case IMCE::IMCE_LONG_BNE:
    expandLongBNE(MI, CB, Fixups, STI);
    MCNumEmitted += 3;
    return;
  }

  // Get instruction encoding and emit it.
  uint64_t Bits = getBinaryCodeForInstr(MI, Fixups, STI);
  ++MCNumEmitted; // Keep track of the number of emitted insns.

  support::endian::write<uint32_t>(CB, Bits, endianness::little);
}

unsigned IMCEMCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                              SmallVectorImpl<MCFixup> &Fixups,
                                              const MCSubtargetInfo &STI) const {
  if (MO.isReg())
    return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg());
  if (MO.isImm())
    return static_cast<uint64_t>(MO.getImm());
  return 0;
}

unsigned IMCEMCCodeEmitter::
getJumpTargetOpValue(const MCInst &MI, unsigned OpNo,
                     SmallVectorImpl<MCFixup> &Fixups,
                     const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  // If the destination is an immediate, divide by 4.
  if (MO.isImm()) return MO.getImm()>>2;

  assert(MO.isExpr() &&
         "getJumpTargetOpValue expects only expressions or an immediate");

  const MCExpr *Expr = MO.getExpr();
  Fixups.push_back(MCFixup::create(0, Expr,
                                   MCFixupKind(IMCE::fixup_imce_26)));
  return 0;
}

unsigned IMCEMCCodeEmitter::
getBranchTargetOpValue(const MCInst &MI, unsigned OpNo,
                       SmallVectorImpl<MCFixup> &Fixups,
                       const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);

  // If the destination is an immediate, divide by 4.
  if (MO.isImm()) return MO.getImm() >> 2;

  assert(MO.isExpr() &&
         "getBranchTargetOpValue expects only expressions or immediates");

  const MCExpr *Expr = MO.getExpr();
  Fixups.push_back(MCFixup::create(0, Expr,
                                   MCFixupKind(IMCE::fixup_imce_PC6)));
  return 0;
}

// #define ENABLE_INSTR_PREDICATE_VERIFIER
#include "IMCEGenMCCodeEmitter.inc"