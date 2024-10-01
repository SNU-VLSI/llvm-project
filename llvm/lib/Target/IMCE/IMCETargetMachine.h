//===-- IMCETargetMachine.h - Define TargetMachine for IMCE -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the IMCE specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_IMCE_IMCETARGETMACHINE_H
#define LLVM_LIB_TARGET_IMCE_IMCETARGETMACHINE_H

#include "IMCESubtarget.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class IMCETargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  mutable StringMap<std::unique_ptr<IMCESubtarget>> SubtargetMap;

public:
  IMCETargetMachine(const Target &T, const Triple &TT, StringRef CPU, StringRef FS,
                    const TargetOptions &Options, std::optional<Reloc::Model> RM,
                    std::optional<CodeModel::Model> CM, CodeGenOpt::Level OL, bool JIT);
  ~IMCETargetMachine() override;

  const IMCESubtarget *getSubtargetImpl(const Function &) const override;

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override { return TLOF.get(); }
};

} // end namespace llvm

#endif
