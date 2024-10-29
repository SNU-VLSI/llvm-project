//===-- INODETargetMachine.h - Define TargetMachine for INODE -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the INODE specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_INODE_INODETARGETMACHINE_H
#define LLVM_LIB_TARGET_INODE_INODETARGETMACHINE_H

#include "INODESubtarget.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class INODETargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  mutable StringMap<std::unique_ptr<INODESubtarget>> SubtargetMap;

public:
  INODETargetMachine(const Target &T, const Triple &TT, StringRef CPU, StringRef FS,
                    const TargetOptions &Options, std::optional<Reloc::Model> RM,
                    std::optional<CodeModel::Model> CM, CodeGenOptLevel OL, bool JIT);
  ~INODETargetMachine() override;

  const INODESubtarget *getSubtargetImpl(const Function &) const override;

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override { return TLOF.get(); }

  void registerPassBuilderCallbacks(PassBuilder &PB) override;
};

} // end namespace llvm

#endif
