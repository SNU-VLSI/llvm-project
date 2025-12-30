//===- INODETargetTransformInfo.h - INODE specific TTI ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file
/// This file defines a TargetTransformInfo::Concept conforming object specific
/// to the INODE target machine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_INODE_INODETARGETTRANSFORMINFO_H
#define LLVM_LIB_TARGET_INODE_INODETARGETTRANSFORMINFO_H

#include "INODESubtarget.h"
#include "INODETargetMachine.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/BasicTTIImpl.h"

namespace llvm {

class INODETTIImpl : public BasicTTIImplBase<INODETTIImpl> {
  using BaseT = BasicTTIImplBase<INODETTIImpl>;
  friend BaseT;

  const INODESubtarget *ST;
  const INODETargetLowering *TLI;

  const INODESubtarget *getST() const { return ST; }
  const INODETargetLowering *getTLI() const { return TLI; }

public:
  explicit INODETTIImpl(const INODETargetMachine *TM, const Function &F)
      : BaseT(TM, F.getDataLayout()), ST(TM->getSubtargetImpl(F)),
        TLI(ST->getTargetLowering()) {}

  bool isHardwareLoopProfitable(Loop *L, ScalarEvolution &SE,
                                AssumptionCache &AC,
                                TargetLibraryInfo *LibInfo,
                                HardwareLoopInfo &HWLoopInfo);
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_INODE_INODETARGETTRANSFORMINFO_H
