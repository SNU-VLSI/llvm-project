//===-- INODEAsmBackend.cpp - INODE Assembler Backend ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "INODEAsmBackend.h"
#include "llvm/ADT/APInt.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDirectives.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/Endian.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/LEB128.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

static unsigned adjustFixupValue(const MCFixup &Fixup, uint64_t Value,
                                 MCContext &Ctx) {

  unsigned Kind = Fixup.getKind();

  // Add/subtract and shift
  switch (Kind) {
  default:
    return 0;
  case FK_Data_2:
    Value &= 0xffff;
    break;
  case FK_DTPRel_4:
  case FK_DTPRel_8:
  case FK_TPRel_4:
  case FK_TPRel_8:
  case FK_GPRel_4:
  case FK_Data_4:
  case FK_Data_8:
    break;
  case INODE::fixup_INODE_PC9: {
    // The displacement is then divided by 4 to give us an 8 bit
    // address range. Forcing a signed division because Value can be negative.
    Value = (int64_t)Value / 4;
    // We now check if Value can be encoded as a 6-bit signed immediate.
    if (!isInt<9>(Value)) {
      Ctx.reportError(Fixup.getLoc(), "out of range PC9 fixup");
      return 0;
    }
    break;
  }
  case INODE::fixup_INODE_PC20: {
    // The displacement is then divided by 4 to give us an 8 bit
    // address range. Forcing a signed division because Value can be negative.
    Value = (int64_t)Value / 4;
    // We now check if Value can be encoded as a 6-bit signed immediate.
    if (!isInt<20>(Value)) {
      Ctx.reportError(Fixup.getLoc(), "out of range PC20 fixup");
      return 0;
    }
    break;
  }
  case INODE::fixup_INODE_target_26:
  case INODE::fixup_INODE_26: {
    // So far we are only using this type for jumps.
    // The displacement is then divided by 4 to give us an 28 bit
    // address range.
    Value >>= 2;
    break;
  }
  }

  return Value;
}

std::unique_ptr<MCObjectTargetWriter>
INODEAsmBackend::createObjectTargetWriter() const {
  return createINODEELFObjectWriter(OSABI, Is64Bit);
}

const MCFixupKindInfo &
INODEAsmBackend::getFixupKindInfo(MCFixupKind Kind) const {
  const static MCFixupKindInfo Infos[] = {
      // This table *must* be in the order that the fixup_* kinds are defined in
      // INODEFixupKinds.h. the offset and bits are in big endian.
      //
      // name              offset bits  flags
      {"fixup_INODE_PC9", 23, 9, MCFixupKindInfo::FKF_IsPCRel},
      {"fixup_INODE_PC20", 12, 20, MCFixupKindInfo::FKF_IsPCRel},
      {"fixup_INODE_target_26", 6, 26, MCFixupKindInfo::FKF_IsTarget},
      {"fixup_INODE_26", 6, 26, 0},
  };

  if (Kind < FirstTargetFixupKind)
    return MCAsmBackend::getFixupKindInfo(Kind);

  assert(unsigned(Kind - FirstTargetFixupKind) < getNumFixupKinds() &&
         "Invalid kind!");
  return Infos[Kind - FirstTargetFixupKind];
}

bool INODEAsmBackend::evaluateTargetFixup(const MCAssembler &Asm,
                                          const MCFixup &Fixup,
                                          const MCFragment *DF,
                                          const MCValue &Target,
                                          const MCSubtargetInfo *STI,
                                          uint64_t &Value, bool &WasForced) {
  if (Fixup.getTargetKind() != INODE::fixup_INODE_target_26)
    llvm_unreachable("Unexpected fixup kind!");

  Value = Target.getConstant();
  return false;
}

/// ApplyFixup - Apply the \p Value for given \p Fixup into the provided
/// data fragment, at the offset specified by the fixup and following the
/// fixup kind as appropriate.
void INODEAsmBackend::applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                                 const MCValue &Target,
                                 MutableArrayRef<char> Data, uint64_t Value,
                                 bool IsResolved,
                                 const MCSubtargetInfo *STI) const {
  MCFixupKind Kind = Fixup.getKind();
  MCContext &Ctx = Asm.getContext();
  Value = adjustFixupValue(Fixup, Value, Ctx);

  unsigned TargetOffset = getFixupKindInfo(Kind).TargetOffset;
  unsigned TargetSize = getFixupKindInfo(Kind).TargetSize;

  // Shift the value into position.
  Value <<= TargetOffset;
  // Mask out the MSBs that don't fit in the fixup.
  Value &= ((uint64_t)1 << (TargetSize + TargetOffset)) - 1;

  if (!Value)
    return; // Doesn't change encoding.

  // Where do we start in the object
  unsigned Offset = Fixup.getOffset();
  // Number of bytes we need to fixup
  unsigned NumBytes = (TargetSize + TargetOffset + 7) / 8;
  assert(Offset + NumBytes <= Data.size() && "Invalid fixup offset!");
  // Used to point to big endian bytes.
  unsigned FullSizeBytes = 4;

  // For each byte of the fragment that the fixup touches, mask in the bits
  // from the fixup value. The Value has been "split up" into the appropriate
  // bitfields above.
  for (unsigned i = 0; i != NumBytes; ++i) {
    unsigned Idx = (FullSizeBytes - 1 - i);
    Data[Offset + Idx] |= (uint8_t)((Value >> (i * 8)) & 0xff);
  }
}

bool INODEAsmBackend::writeNopData(raw_ostream &OS, uint64_t Count,
                                   const MCSubtargetInfo *STI) const {
  // // return true;
  assert(Count == 0 && "Not implemented yet");
  return true;
}

MCAsmBackend *llvm::createINODEAsmBackend(const Target &T,
                                          const MCSubtargetInfo &STI,
                                          const MCRegisterInfo &MRI,
                                          const MCTargetOptions &Options) {
  const Triple &TT = STI.getTargetTriple();
  uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(TT.getOS());
  return new INODEAsmBackend(STI, OSABI, TT.isArch64Bit(), Options);
}

bool INODEAsmBackend::mayNeedRelaxation(const MCInst &Inst,
                                        const MCSubtargetInfo &STI) const {
  unsigned Opcode = Inst.getOpcode();
  return (Opcode == INODE::INODE_BNE) | (Opcode == INODE::INODE_BEQ) |
         (Opcode == INODE::INODE_BGE) | (Opcode == INODE::INODE_BLT) |
         (Opcode == INODE::INODE_BNE_UPDATE_INST);
};

bool INODEAsmBackend::fixupNeedsRelaxation(const MCFixup &Fixup,
                                           uint64_t Value) const {
  int64_t Offset = int64_t(Value);
  unsigned Kind = Fixup.getTargetKind();
  switch (Kind) {
  default:
    return false;
  case INODE::fixup_INODE_PC9:
    return Offset > 255 || Offset < -256;
  case INODE::fixup_INODE_PC20:
    return Offset > 1048575 || Offset < -1048576;
  }
};

void INODEAsmBackend::relaxInstruction(MCInst &Inst,
                                       const MCSubtargetInfo &STI) const {
  MCInst Res;
  switch (Inst.getOpcode()) {
  default:
    llvm_unreachable("Unexpected instruction to relax");
  case INODE::INODE_BNE: {
    Res.setOpcode(INODE::INODE_LONG_BNE);
    Res.addOperand(Inst.getOperand(0));
    Res.addOperand(Inst.getOperand(1));
    Res.addOperand(Inst.getOperand(2));
    Res.addOperand(Inst.getOperand(3));
    break;
  }
  case INODE::INODE_BEQ: {
    Res.setOpcode(INODE::INODE_LONG_BEQ);
    Res.addOperand(Inst.getOperand(0));
    Res.addOperand(Inst.getOperand(1));
    Res.addOperand(Inst.getOperand(2));
    break;
  }
  case INODE::INODE_BGE: {
    Res.setOpcode(INODE::INODE_LONG_BGE);
    Res.addOperand(Inst.getOperand(0));
    Res.addOperand(Inst.getOperand(1));
    Res.addOperand(Inst.getOperand(2));
    break;
  }
  case INODE::INODE_BLT: {
    Res.setOpcode(INODE::INODE_LONG_BLT);
    Res.addOperand(Inst.getOperand(0));
    Res.addOperand(Inst.getOperand(1));
    Res.addOperand(Inst.getOperand(2));
    break;
  }
  }
  Inst = std::move(Res);
};
