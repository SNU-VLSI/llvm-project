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
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/Endian.h"
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

static unsigned getReg(const MCDisassembler *D, unsigned RC, unsigned RegNo) {
  const MCRegisterInfo *RegInfo = D->getContext().getRegisterInfo();
  return *(RegInfo->getRegClass(RC).begin() + RegNo);
}

static DecodeStatus decodeVGPRRegisterClass(MCInst &Inst, uint64_t RegNo, uint64_t Address,
                                            const MCDisassembler *Decoder) {
  if (RegNo > 31)
    return MCDisassembler::Fail;

  unsigned Register = getReg(Decoder, IMCE::VGPRRegClassID, RegNo);
  Inst.addOperand(MCOperand::createReg(Register));
  return MCDisassembler::Success;
}

static DecodeStatus decodeHWLRRegisterClass(MCInst &Inst, uint64_t RegNo, uint64_t Address,
                                            const MCDisassembler *Decoder) {
  // Ensure that RegNo falls within the range of hardware loop registers.
  if (RegNo < 48 || RegNo > 54)
    return MCDisassembler::Fail;
  else
    RegNo -= 48;

  unsigned Register = getReg(Decoder, IMCE::HWLRRegClassID, RegNo);
  Inst.addOperand(MCOperand::createReg(Register));
  return MCDisassembler::Success;
}

static DecodeStatus decodeQRegsRegisterClass(MCInst &Inst, uint64_t RegNo, uint64_t Address,
                                            const MCDisassembler *Decoder) {
  // Ensure that RegNo falls within the range of quantization registers.
  if (RegNo < 36 || RegNo > 40)
    return MCDisassembler::Fail;
  else
    RegNo -= 36;

  unsigned Register = getReg(Decoder, IMCE::QRegsRegClassID, RegNo);
  Inst.addOperand(MCOperand::createReg(Register));
  return MCDisassembler::Success;
}

static DecodeStatus decodeCRegsRegisterClass(MCInst &Inst, uint64_t RegNo, uint64_t Address,
                                            const MCDisassembler *Decoder) {
  // Ensure that RegNo falls within the range of compute registers.
  if (RegNo < 32 || RegNo > 36)
    return MCDisassembler::Fail;
  else
    RegNo -= 32;

  unsigned Register = getReg(Decoder, IMCE::CRegsRegClassID, RegNo);
  Inst.addOperand(MCOperand::createReg(Register));
  return MCDisassembler::Success;
}

template <unsigned N>
static DecodeStatus decodeUImmOperand(MCInst &Inst, uint32_t Imm,
                                      int64_t Address,
                                      const MCDisassembler *Decoder) {
  assert(isUInt<N>(Imm) && "Invalid immediate");
  Inst.addOperand(MCOperand::createImm(Imm));
  return MCDisassembler::Success;
}

template <unsigned N>
static DecodeStatus decodeSImmOperand(MCInst &Inst, uint32_t Imm,
                                      int64_t Address,
                                      const MCDisassembler *Decoder) {
  assert(isUInt<N>(Imm) && "Invalid immediate");
  // Sign-extend the number in the bottom N bits of Imm
  Inst.addOperand(MCOperand::createImm(SignExtend64<N>(Imm)));
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
  uint32_t insn = support::endian::read32be(Bytes.data());

  if (decodeInstruction(DecoderTableIMCE32, MI, insn, Address, this, STI) !=
      MCDisassembler::Success) {
    return MCDisassembler::Fail;
  }
  return MCDisassembler::Success;
}
