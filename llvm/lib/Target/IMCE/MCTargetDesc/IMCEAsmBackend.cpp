//===-- IMCEAsmBackend.cpp - IMCE Assembler Backend ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "IMCEAsmBackend.h"
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

std::unique_ptr<MCObjectTargetWriter> IMCEAsmBackend::createObjectTargetWriter() const {
  return createIMCEELFObjectWriter(OSABI, Is64Bit);
}

const MCFixupKindInfo &IMCEAsmBackend::getFixupKindInfo(MCFixupKind Kind) const {
  const static MCFixupKindInfo Infos[] = {
      // This table *must* be in the order that the fixup_* kinds are defined in
      // RISCVFixupKinds.h.
      //
      // name                      offset bits  flags
      {"fixup_imce_PC6", 0, 32, MCFixupKindInfo::FKF_IsPCRel},
  };

  if (Kind < FirstTargetFixupKind)
    return MCAsmBackend::getFixupKindInfo(Kind);

  assert(unsigned(Kind - FirstTargetFixupKind) < getNumFixupKinds() &&
         "Invalid kind!");
  return Infos[Kind - FirstTargetFixupKind];
}

void IMCEAsmBackend::applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                                const MCValue &Target,
                                MutableArrayRef<char> Data, uint64_t Value,
                                bool IsResolved,
                                const MCSubtargetInfo *STI) const {
}

bool IMCEAsmBackend::writeNopData(raw_ostream &OS, uint64_t Count,
                                  const MCSubtargetInfo *STI) const {
  return true;
}

MCAsmBackend *llvm::createIMCEAsmBackend(const Target &T,
                                         const MCSubtargetInfo &STI,
                                         const MCRegisterInfo &MRI,
                                         const MCTargetOptions &Options) {
  const Triple &TT = STI.getTargetTriple();
  uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(TT.getOS());
  return new IMCEAsmBackend(STI, OSABI, TT.isArch64Bit(), Options);
}

bool IMCEAsmBackend::mayNeedRelaxation(const MCInst &Inst,
                        const MCSubtargetInfo &STI) const {
  unsigned Opcode = Inst.getOpcode();
  return Opcode == IMCE::IMCE_BNE_INST;
};

bool IMCEAsmBackend::fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value) const {
  int64_t Offset = int64_t(Value);
  unsigned Kind = Fixup.getTargetKind();
  switch (Kind) {
  default:
    return false;
  case IMCE::fixup_imce_PC6:
    // For BNE instruction the immediate (simm6) must be
    // in the range [-32, 31].
    return Offset > 31 || Offset < -32;
  }
};

void IMCEAsmBackend::relaxInstruction(MCInst &Inst,
                      const MCSubtargetInfo &STI) const {
  switch (Inst.getOpcode()) {
    default:
      llvm_unreachable("Unexpected instruction to relax");
    case IMCE::IMCE_BNE_INST: {
      llvm_unreachable("not yet implemented");
    }
  }
};

// void IMCEAsmBackend::relaxInstruction(const MCInst &Inst, MCInst &Res) const {
//     assert(Inst.getOpcode() == IMCE::BNE && "Only relaxing BNE instructions is supported");

//     // Extract the operands from the original BNE instruction
//     // Assuming BNE format: BNE $reg1, $reg2, offset
//     auto Reg1 = Inst.getOperand(0).getReg();
//     auto Reg2 = Inst.getOperand(1).getReg();
//     int64_t TargetOffset = Inst.getOperand(2).getImm();

//     // Determine if the target offset is within the signed 6-bit range (-32 to +31)
//     if (TargetOffset >= -32 && TargetOffset <= 31) {
//         // If within range, no relaxation is needed
//         Res = Inst;
//         return;
//     }

//     // Otherwise, we need to relax the instruction into a BNE + JMP sequence
//     // Step 1: Create a BNE instruction that branches to a "nearby" location
//     MCInst BNE_Near;
//     BNE_Near.setOpcode(IMCE::BNE);
//     BNE_Near.addOperand(MCOperand::createReg(Reg1));
//     BNE_Near.addOperand(MCOperand::createReg(Reg2));
//     BNE_Near.addOperand(MCOperand::createImm(2)); // BNE to the next instruction (JMP long_target)

//     // Step 2: Create a JMP instruction that skips over the long jump
//     MCInst JMP_End;
//     JMP_End.setOpcode(IMCE::JMP);
//     JMP_End.addOperand(MCOperand::createImm(2)); // Skip over the long_target JMP

//     // Step 3: Create the JMP instruction to the actual long target
//     MCInst JMP_LongTarget;
//     JMP_LongTarget.setOpcode(IMCE::JMP);
//     JMP_LongTarget.addOperand(MCOperand::createImm(TargetOffset));

//     // Step 4: Combine the instructions in order: BNE_Near, JMP_End, JMP_LongTarget
//     Res = MCInst();
//     Res.setOpcode(IMCE::SEQUENCE); // This is a pseudo-op to represent a sequence of instructions
//     Res.addOperand(MCOperand::createInst(BNE_Near));
//     Res.addOperand(MCOperand::createInst(JMP_End));
//     Res.addOperand(MCOperand::createInst(JMP_LongTarget));
// }
