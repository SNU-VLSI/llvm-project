//===-- INODEMCCodeEmitter.cpp - Convert INODE code to machine code
//---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the INODEMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include "INODEMCCodeEmitter.h"
#include "MCTargetDesc/INODEFixupKinds.h"
#include "MCTargetDesc/INODEMCTargetDesc.h"
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
MCCodeEmitter *createINODEMCCodeEmitter(const MCInstrInfo &MCII,
                                        MCContext &Ctx) {
  return new INODEMCCodeEmitter(MCII, Ctx);
}

} // namespace llvm

// Expand INODE_LONG_BNE to a BNE, JMP, JUMP instruction sequence.
void INODEMCCodeEmitter::expandLongBR(const MCInst &MI,
                                      SmallVectorImpl<char> &CB,
                                      SmallVectorImpl<MCFixup> &Fixups,
                                      const MCSubtargetInfo &STI,
                                      bool IsBrUpdate,
                                      unsigned int BrOpcode) const {

  MCRegister OutReg;
  MCRegister SrcReg;
  MCRegister SrcReg2;
  int64_t SrcImm;
  MCOperand SrcSymbol;
  MCInst TmpInst;

  if (IsBrUpdate) {
    OutReg = MI.getOperand(0).getReg();
    SrcReg = MI.getOperand(1).getReg();
    SrcImm = MI.getOperand(2).getImm();
    SrcSymbol = MI.getOperand(3);
    TmpInst = MCInstBuilder(BrOpcode)
                  .addReg(OutReg)
                  .addReg(SrcReg)
                  .addImm(SrcImm)
                  .addImm(8);
  } else {
    SrcReg = MI.getOperand(0).getReg();
    SrcReg2 = MI.getOperand(1).getReg();
    SrcSymbol = MI.getOperand(2);
    TmpInst = MCInstBuilder(BrOpcode).addReg(SrcReg).addReg(SrcReg2).addImm(8);
  }

  // Emit a bne where if not taken, proceed to first JMP_INST, if taken jump to
  // the second.
  uint32_t Binary = getBinaryCodeForInstr(TmpInst, Fixups, STI);
  support::endian::write(CB, Binary, llvm::endianness::big);

  // Emit an unconditional jump to skip the next instruction.
  int64_t TargetOffset = 8;
  const MCExpr *OffsetExpr = MCConstantExpr::create(TargetOffset, Ctx);

  TmpInst = MCInstBuilder(INODE::INODE_JMP_INST).addExpr(OffsetExpr);
  Binary = getBinaryCodeForInstr(TmpInst, Fixups, STI);
  support::endian::write(CB, Binary, llvm::endianness::big);

  // Emit an unconditional jump to the destination.
  TmpInst = MCInstBuilder(INODE::INODE_JMP_INST).addOperand(SrcSymbol);
  Binary = getBinaryCodeForInstr(TmpInst, Fixups, STI);
  support::endian::write(CB, Binary, llvm::endianness::big);

  // override the fixups.
  Fixups.clear();
  uint32_t Offset = 4;
  Fixups.push_back(MCFixup::create(Offset, OffsetExpr,
                                   MCFixupKind(INODE::fixup_INODE_target_26),
                                   MI.getLoc()));
  if (SrcSymbol.isExpr()) {
    Offset = 8;
    Fixups.push_back(MCFixup::create(Offset, SrcSymbol.getExpr(),
                                     MCFixupKind(INODE::fixup_INODE_26),
                                     MI.getLoc()));
  }
}

void INODEMCCodeEmitter::encodeInstruction(const MCInst &MI,
                                           SmallVectorImpl<char> &CB,
                                           SmallVectorImpl<MCFixup> &Fixups,
                                           const MCSubtargetInfo &STI) const {
  switch (MI.getOpcode()) {
  default:
    break;
  case INODE::INODE_LONG_BNE_UPDATE:
    expandLongBR(MI, CB, Fixups, STI, true, INODE::INODE_BNE_UPDATE_INST);
    MCNumEmitted += 3;
    return;
  case INODE::INODE_LONG_BNE:
    expandLongBR(MI, CB, Fixups, STI, false, INODE::INODE_BNE);
    MCNumEmitted += 3;
    return;
  case INODE::INODE_LONG_BEQ:
    expandLongBR(MI, CB, Fixups, STI, false, INODE::INODE_BEQ);
    MCNumEmitted += 3;
    return;
  case INODE::INODE_LONG_BGE:
    expandLongBR(MI, CB, Fixups, STI, false, INODE::INODE_BGE);
    MCNumEmitted += 3;
    return;
  case INODE::INODE_LONG_BLT:
    expandLongBR(MI, CB, Fixups, STI, false, INODE::INODE_BLT);
    MCNumEmitted += 3;
    return;
  }

  // Get instruction encoding and emit it.
  uint64_t Bits = getBinaryCodeForInstr(MI, Fixups, STI);
  ++MCNumEmitted; // Keep track of the number of emitted insns.

  support::endian::write<uint32_t>(CB, Bits, endianness::big);
}

unsigned
INODEMCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                      SmallVectorImpl<MCFixup> &Fixups,
                                      const MCSubtargetInfo &STI) const {
  if (MO.isReg())
    return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg());
  if (MO.isImm())
    return static_cast<uint64_t>(MO.getImm());
  return 0;
}

unsigned
INODEMCCodeEmitter::getJumpTargetOpValue(const MCInst &MI, unsigned OpNo,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  // If the destination is an immediate, divide by 4.
  if (MO.isImm())
    return MO.getImm() >> 2;

  assert(MO.isExpr() &&
         "getJumpTargetOpValue expects only expressions or an immediate");

  const MCExpr *Expr = MO.getExpr();
  Fixups.push_back(
      MCFixup::create(0, Expr, MCFixupKind(INODE::fixup_INODE_26)));
  return 0;
}

unsigned
INODEMCCodeEmitter::getBranchTargetOpValue(const MCInst &MI, unsigned OpNo,
                                           SmallVectorImpl<MCFixup> &Fixups,
                                           const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);

  // If the destination is an immediate, divide by 4.
  if (MO.isImm())
    return MO.getImm() >> 2;

  assert(MO.isExpr() &&
         "getBranchTargetOpValue expects only expressions or immediates");

  const MCExpr *Expr = MO.getExpr();
  Fixups.push_back(
      MCFixup::create(0, Expr, MCFixupKind(INODE::fixup_INODE_PC20)));
  return 0;
}

unsigned
INODEMCCodeEmitter::getBranchTargetOpValue9(const MCInst &MI, unsigned OpNo,
                                            SmallVectorImpl<MCFixup> &Fixups,
                                            const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);

  // If the destination is an immediate, divide by 4.
  if (MO.isImm())
    return MO.getImm() >> 2;

  assert(MO.isExpr() &&
         "getBranchTargetOpValue expects only expressions or immediates");

  const MCExpr *Expr = MO.getExpr();
  Fixups.push_back(
      MCFixup::create(0, Expr, MCFixupKind(INODE::fixup_INODE_PC9)));
  return 0;
}

// #define ENABLE_INSTR_PREDICATE_VERIFIER
#include "INODEGenMCCodeEmitter.inc"