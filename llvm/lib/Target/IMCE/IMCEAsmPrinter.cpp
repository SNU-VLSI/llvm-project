//===-- IMCEAsmPrinter.cpp - IMCE LLVM assembly writer ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to GAS-format IMCE assembly language.
//
//===----------------------------------------------------------------------===//

#include "IMCEMCInstLower.h"
#include "MCTargetDesc/IMCEMCTargetDesc.h"
#include "TargetInfo/IMCETargetInfo.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"
#include <memory>

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

namespace {
class IMCEAsmPrinter : public AsmPrinter {
public:
  explicit IMCEAsmPrinter(TargetMachine &TM, std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "IMCE Assembly Printer"; }

  void emitInstruction(const MachineInstr *MI) override;
};
} // end of anonymous namespace

void IMCEAsmPrinter::emitInstruction(const MachineInstr *MI) {
  MCInst LoweredMI;
  IMCEMCInstLower Lower;
  Lower.lower(MI, LoweredMI, this);
  EmitToStreamer(*OutStreamer, LoweredMI);
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeIMCEAsmPrinter() {
  RegisterAsmPrinter<IMCEAsmPrinter> X(getTheIMCETarget());
}
