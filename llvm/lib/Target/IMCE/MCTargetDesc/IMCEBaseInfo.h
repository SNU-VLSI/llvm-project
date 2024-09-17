//===-- IMCEBaseInfo.h - Top level definitions for RISC-V MC ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone enum definitions for the RISC-V target
// useful for the compiler back-end and the MC libraries.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_LIB_TARGET_IMCE_MCTARGETDESC_IMCEBASEINFO_H
#define LLVM_LIB_TARGET_IMCE_MCTARGETDESC_IMCEBASEINFO_H

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/TargetParser/SubtargetFeature.h"

namespace llvm {

namespace IMCEOp {
enum OperandType : unsigned {
  OPERAND_FIRST_IMCE_IMM = MCOI::OPERAND_FIRST_TARGET,
  OPERAND_UIMM1 = OPERAND_FIRST_IMCE_IMM,
  OPERAND_UIMM2,
  OPERAND_UIMM3,
  OPERAND_UIMM4,
  OPERAND_UIMM6,
  OPERAND_UIMM8,
  OPERAND_UIMM14,
  OPERAND_UIMM26,
  OPERAND_SIMM14,
};
} // namespace IMCEOp
} // namespace llvm

#endif
