//===- INODETargetTransformInfo.cpp - INODE specific TTI ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "INODETargetTransformInfo.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Support/Debug.h"

using namespace llvm;

#define DEBUG_TYPE "inode-tti"

bool INODETTIImpl::isHardwareLoopProfitable(Loop *L, ScalarEvolution &SE,
                                            AssumptionCache &AC,
                                            TargetLibraryInfo *LibInfo,
                                            HardwareLoopInfo &HWLoopInfo) {
  // Check if loop has a computable backedge taken count
  if (!SE.hasLoopInvariantBackedgeTakenCount(L)) {
    LLVM_DEBUG(dbgs() << "INODEHWLoops: No loop-invariant backedge taken count\n");
    return false;
  }

  const SCEV *BackedgeTakenCount = SE.getBackedgeTakenCount(L);
  if (isa<SCEVCouldNotCompute>(BackedgeTakenCount)) {
    LLVM_DEBUG(dbgs() << "INODEHWLoops: Cannot compute backedge taken count\n");
    return false;
  }

  const SCEV *TripCountSCEV =
      SE.getAddExpr(BackedgeTakenCount,
                    SE.getOne(BackedgeTakenCount->getType()));

  // INODE hardware loops only support constant trip counts.
  // Check if the trip count is a constant.
  if (!isa<SCEVConstant>(TripCountSCEV)) {
    LLVM_DEBUG(dbgs() << "INODEHWLoops: Trip count is not constant: "
                      << *TripCountSCEV << "\n");
    return false;
  }

  // Trip count must fit in 32 bits
  if (SE.getUnsignedRangeMax(TripCountSCEV).getBitWidth() > 32) {
    LLVM_DEBUG(dbgs() << "INODEHWLoops: Trip count does not fit in 32 bits\n");
    return false;
  }

  LLVMContext &C = L->getHeader()->getContext();
  HWLoopInfo.CounterInReg = false;  // Use set_loop_iterations + loop_decrement
  HWLoopInfo.IsNestingLegal = true;
  HWLoopInfo.PerformEntryTest = false;
  HWLoopInfo.CountType = Type::getInt32Ty(C);
  HWLoopInfo.LoopDecrement = ConstantInt::get(HWLoopInfo.CountType, 1);

  LLVM_DEBUG(dbgs() << "INODEHWLoops: Loop is profitable with constant trip count: "
                    << *TripCountSCEV << "\n");
  return true;
}
