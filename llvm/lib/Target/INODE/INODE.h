//===-- INODE.h - Top-level interface for INODE representation ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// INODE back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_INODEK_INODEK_H
#define LLVM_LIB_TARGET_INODEK_INODEK_H

#include "llvm/Support/CodeGen.h"

namespace llvm {
class INODERegisterBankInfo;
class INODESubtarget;
class INODETargetMachine;
class FunctionPass;
class InstructionSelector;
class PassRegistry;

FunctionPass *createINODEISelDag(INODETargetMachine &TM, CodeGenOptLevel OptLevel);

void initializeINODEDAGToDAGISelPass(PassRegistry &);
void initializeINODEDAGToDAGISelLegacyPass(PassRegistry &);

void initializePrintFunctionsPass(PassRegistry &);
FunctionPass *createPrintFunctionsPass();

void initializeINODELoopConversionPass(PassRegistry &);
FunctionPass *createINODELoopConversionPass();

void initializePrintFunctionsPass(PassRegistry &);
FunctionPass *createPrintFunctionsPass(); 

void initializeINODEPrintMachineFunctionPass(PassRegistry &);
FunctionPass *createINODEPrintMachineFunctionPass(); 

// InstructionSelector *
// createINODEInstructionSelector(const INODETargetMachine &, const INODESubtarget&,
//                               const INODERegisterBankInfo &);

void initializeINODECountedLoopMIRPass(PassRegistry &);
FunctionPass *createINODECountedLoopMIRPass();

void initializeINODECoreIDAssignPass(PassRegistry &);
FunctionPass *createINODECoreIDAssignPass();

void initializeSCCPLegacyPass(PassRegistry &);
FunctionPass *createSCCPLegacyPass();

} // end namespace llvm
#endif
