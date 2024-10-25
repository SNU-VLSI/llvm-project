//===- IMCEInstPrinter.cpp - Convert IMCE MCInst to assembly syntax -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "IMCEInstPrinter.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"
#include <cassert>
#include <cstdint>

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#include "IMCEGenAsmWriter.inc"

void IMCEInstPrinter::printOperand(const MCInst *MI, int OpNum,
                                   const MCSubtargetInfo &STI, raw_ostream &O) {
  const MCOperand &MO = MI->getOperand(OpNum);
  if (MO.isReg()) {
    if (!MO.getReg())
      O << '0';
    else
      O << '%' << getRegisterName(MO.getReg());
  } else if (MO.isImm())
    O << MO.getImm();
  else if (MO.isExpr())
    MO.getExpr()->print(O, &MAI);
  else
    llvm_unreachable("Invalid operand");
}

void IMCEInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                StringRef Annot, const MCSubtargetInfo &STI,
                                raw_ostream &O) {
  printInstruction(MI, Address, STI, O);
  printAnnotation(O, Annot);
}

void IMCEInstPrinter::printJumpOperand(const MCInst *MI, unsigned OpNo,
                                       const MCSubtargetInfo &STI,
                                       raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (!Op.isImm())
    return printOperand(MI, OpNo, STI, O);

  if (PrintBranchImmAsAddress)
    markup(O, Markup::Immediate) << formatHex(Op.getImm());
  else
    markup(O, Markup::Immediate) << formatImm(Op.getImm());
}

void IMCEInstPrinter::printBranchOperand(const MCInst *MI, uint64_t Address,
                                         unsigned OpNo,
                                         const MCSubtargetInfo &STI,
                                         raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (!Op.isImm())
    return printOperand(MI, OpNo, STI, O);

  if (PrintBranchImmAsAddress) {
    uint64_t Target = Address + Op.getImm();
    markup(O, Markup::Immediate) << formatHex(Target);
  } else {
    markup(O, Markup::Immediate) << formatImm(Op.getImm());
  }
}
