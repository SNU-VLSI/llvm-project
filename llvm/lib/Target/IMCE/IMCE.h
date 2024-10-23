//===-- IMCE.h - Top-level interface for IMCE representation ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// IMCE back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_IMCEK_IMCEK_H
#define LLVM_LIB_TARGET_IMCEK_IMCEK_H

#include "llvm/Support/CodeGen.h"

namespace llvm {
class IMCERegisterBankInfo;
class IMCESubtarget;
class IMCETargetMachine;
class FunctionPass;
class InstructionSelector;
class PassRegistry;

FunctionPass *createIMCEISelDag(IMCETargetMachine &TM, CodeGenOptLevel OptLevel);

void initializeIMCEDAGToDAGISelPass(PassRegistry &);
void initializeIMCEDAGToDAGISelLegacyPass(PassRegistry &);

void initializePrintFunctionsPass(PassRegistry &);
FunctionPass *createPrintFunctionsPass();

void initializeIMCELoopConversionPass(PassRegistry &);
FunctionPass *createIMCELoopConversionPass();

void initializePrintFunctionsPass(PassRegistry &);
FunctionPass *createPrintFunctionsPass(); 

void initializeIMCEPrintMachineFunctionPass(PassRegistry &);
FunctionPass *createIMCEPrintMachineFunctionPass(); 

// InstructionSelector *
// createIMCEInstructionSelector(const IMCETargetMachine &, const IMCESubtarget&,
//                               const IMCERegisterBankInfo &);

void initializeIMCECountedLoopMIRPass(PassRegistry &);
FunctionPass *createIMCECountedLoopMIRPass();

void initializeIMCECoreIDAssignPass(PassRegistry &);
FunctionPass *createIMCECoreIDAssignPass();

} // end namespace llvm
#endif
