//===-- IMCEMCInstLower.h - Lower MachineInstr to MCInst -------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_IMCE_IMCEMCINSTLOWER_H
#define LLVM_LIB_TARGET_IMCE_IMCEMCINSTLOWER_H

#include "llvm/Support/Compiler.h"

namespace llvm {
class AsmPrinter;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineOperand;
class Mangler;

class LLVM_LIBRARY_VISIBILITY IMCEMCInstLower {
public:
  // Lower MachineInstr MI to MCInst OutMI.
  void lower(const MachineInstr *MI, MCInst &OutMI, AsmPrinter *Amp) const;

  // Return an MCOperand for MO.
  MCOperand lowerOperand(const MachineOperand &MO, const AsmPrinter *Amp) const;
};
} // end namespace llvm

#endif
