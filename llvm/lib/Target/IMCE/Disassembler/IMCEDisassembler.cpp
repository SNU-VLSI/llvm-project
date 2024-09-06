//===-- IMCEDisassembler.cpp - Disassembler for IMCE ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// #include "IMCE.h"
#include "MCTargetDesc/IMCEMCTargetDesc.h"
#include "TargetInfo/IMCETargetInfo.h"
#include "llvm/MC/MCDecoderOps.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/MathExtras.h"
#include <cassert>
#include <cstdint>

using namespace llvm;

#define DEBUG_TYPE "IMCE-disassembler"

using DecodeStatus = MCDisassembler::DecodeStatus;

namespace {

class IMCEDisassembler : public MCDisassembler {
public:
  IMCEDisassembler(const MCSubtargetInfo &STI, MCContext &Ctx) : MCDisassembler(STI, Ctx) {}
  ~IMCEDisassembler() override = default;

  DecodeStatus getInstruction(MCInst &instr, uint64_t &Size, ArrayRef<uint8_t> Bytes,
                              uint64_t Address, raw_ostream &CStream) const override;
};

} // end anonymous namespace

static MCDisassembler *createIMCEDisassembler(const Target &T, const MCSubtargetInfo &STI,
                                              MCContext &Ctx) {
  return new IMCEDisassembler(STI, Ctx);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeIMCEDisassembler() {
  // Register the disassembler.
  TargetRegistry::RegisterMCDisassembler(getTheIMCETarget(), createIMCEDisassembler);
}

static const uint16_t GPRDecoderTable[] = {
    IMCE::V0,  IMCE::V1,  IMCE::V2,  IMCE::V3,  IMCE::V4,  IMCE::V5,  IMCE::V6,  IMCE::V7,
    IMCE::V8,  IMCE::V9,  IMCE::V10, IMCE::V11, IMCE::V12, IMCE::V13, IMCE::V14, IMCE::V15,

    IMCE::V16, IMCE::V17, IMCE::V18, IMCE::V19, IMCE::V20, IMCE::V21, IMCE::V22, IMCE::V23,
    IMCE::V24, IMCE::V25, IMCE::V26, IMCE::V27, IMCE::V28, IMCE::V29, IMCE::V30, IMCE::V31,
};

static DecodeStatus decodeVGPRRegisterClass(MCInst &Inst, uint64_t RegNo, uint64_t Address,
                                            const void *Decoder) {
  if (RegNo > 31)
    return MCDisassembler::Fail;

  unsigned Register = GPRDecoderTable[RegNo];
  Inst.addOperand(MCOperand::createReg(Register));
  return MCDisassembler::Success;
}

static DecodeStatus decodeSGPRRegisterClass(MCInst &Inst, uint64_t RegNo, uint64_t Address,
                                            const void *Decoder) {
  if (RegNo > 31)
    return MCDisassembler::Fail;

  unsigned Register = GPRDecoderTable[RegNo];
  Inst.addOperand(MCOperand::createReg(Register));
  return MCDisassembler::Success;
}

static DecodeStatus decodeUImmOperand6(MCInst &Inst, uint32_t Imm,
                                      int64_t Address,
                                      const MCDisassembler *Decoder) {
  Inst.addOperand(MCOperand::createImm(Imm));
  return MCDisassembler::Success;
}

#include "IMCEGenDisassemblerTables.inc"

DecodeStatus IMCEDisassembler::getInstruction(MCInst &MI, uint64_t &Size, ArrayRef<uint8_t> Bytes,
                                              uint64_t Address, raw_ostream &CS) const {
  // Instruction size is always 32 bit.
  if (Bytes.size() < 4) {
    Size = 0;
    return MCDisassembler::Fail;
  }
  Size = 4;

  // Construct the instruction.
  uint32_t Inst = 0;
  for (uint32_t I = 0; I < Size; ++I)
    Inst = (Inst << 8) | Bytes[I];

  if (decodeInstruction(DecoderTableIMCE32, MI, Inst, Address, this, STI) !=
      MCDisassembler::Success) {
    //    if (STI.getFeatureBits()[IMCE::Proc88110])
    //      return decodeInstruction(DecoderTableMC8811032, MI, Inst, Address, this,
    //                               STI);
    return MCDisassembler::Fail;
  }
  return MCDisassembler::Success;
}
