//===-- INODEBaseInfo.h - Top level definitions for RISC-V MC ---*- C++ -*-===//
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
#ifndef LLVM_LIB_TARGET_INODE_MCTARGETDESC_INODEBASEINFO_H
#define LLVM_LIB_TARGET_INODE_MCTARGETDESC_INODEBASEINFO_H

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/TargetParser/SubtargetFeature.h"

namespace llvm {

namespace INODEOp {
enum OperandType : unsigned {
  OPERAND_FIRST_INODE_IMM = MCOI::OPERAND_FIRST_TARGET,
  OPERAND_UIMM1 = OPERAND_FIRST_INODE_IMM,
  OPERAND_UIMM2,
  OPERAND_UIMM3,
  OPERAND_UIMM4,
  OPERAND_UIMM4_ALLONE,
  OPERAND_UIMM6,
  OPERAND_UIMM7,
  OPERAND_UIMM8,
  OPERAND_UIMM12,
  OPERAND_UIMM13,
  OPERAND_UIMM14,
  OPERAND_UIMM16,
  OPERAND_UIMM26,
  OPERAND_UIMM32,
  OPERAND_SIMM6,
  OPERAND_SIMM13,
  OPERAND_SIMM14,
  OPERAND_SIMM20,
  OPERAND_SIMM32,
};
} // namespace INODEOp
} // namespace llvm

#endif
