#include "IMCE.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/DomTreeUpdater.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/User.h"
#include "llvm/IR/Value.h"
#include "llvm/Pass.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/SCCP.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/IntrinsicsIMCE.h"
#include "llvm/Transforms/Utils/SCCPSolver.h"
#include <utility>

using namespace llvm;

#define DEBUG_TYPE "SCCP_Legacy"
#define PASS_NAME "SCCP_Legacy"

STATISTIC(NumInstRemoved, "Number of instructions removed");
STATISTIC(NumDeadBlocks , "Number of basic blocks unreachable");
STATISTIC(NumInstReplaced,
          "Number of instructions replaced with (simpler) instruction");

namespace {

class SCCPLegacy : public FunctionPass {

public:
  static char ID;

  SCCPLegacy() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override;

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<TargetLibraryInfoWrapperPass>();
    AU.addRequired<DominatorTreeWrapperPass>();
    FunctionPass::getAnalysisUsage(AU);
  }


  StringRef getPassName() const override { return PASS_NAME; }
};

} // end anonymous namespace

INITIALIZE_PASS_BEGIN(SCCPLegacy, DEBUG_TYPE, PASS_NAME, false, false)
INITIALIZE_PASS_END(SCCPLegacy, DEBUG_TYPE, PASS_NAME, false, false)

static bool runSCCP(Function &F, const DataLayout &DL,
                    const TargetLibraryInfo *TLI, DomTreeUpdater &DTU) {
  LLVM_DEBUG(dbgs() << "SCCP on function '" << F.getName() << "'\n");
  SCCPSolver Solver(
      DL, [TLI](Function &F) -> const TargetLibraryInfo & { return *TLI; },
      F.getContext());

  // Mark the first block of the function as being executable.
  Solver.markBlockExecutable(&F.front());

  // Mark all arguments to the function as being overdefined.
  for (Argument &AI : F.args())
    Solver.markOverdefined(&AI);

  // Solve for constants.
  bool ResolvedUndefs = true;
  while (ResolvedUndefs) {
    Solver.solve();
    LLVM_DEBUG(dbgs() << "RESOLVING UNDEFs\n");
    ResolvedUndefs = Solver.resolvedUndefsIn(F);
  }

  bool MadeChanges = false;

  // If we decided that there are basic blocks that are dead in this function,
  // delete their contents now.  Note that we cannot actually delete the blocks,
  // as we cannot modify the CFG of the function.

  SmallPtrSet<Value *, 32> InsertedValues;
  SmallVector<BasicBlock *, 8> BlocksToErase;
  for (BasicBlock &BB : F) {
    if (!Solver.isBlockExecutable(&BB)) {
      LLVM_DEBUG(dbgs() << "  BasicBlock Dead:" << BB);
      ++NumDeadBlocks;
      BlocksToErase.push_back(&BB);
      MadeChanges = true;
      continue;
    }

    MadeChanges |= Solver.simplifyInstsInBlock(BB, InsertedValues,
                                               NumInstRemoved, NumInstReplaced);
  }

  // Remove unreachable blocks and non-feasible edges.
  for (BasicBlock *DeadBB : BlocksToErase)
    NumInstRemoved += changeToUnreachable(DeadBB->getFirstNonPHI(),
                                          /*PreserveLCSSA=*/false, &DTU);

  BasicBlock *NewUnreachableBB = nullptr;
  for (BasicBlock &BB : F)
    MadeChanges |= Solver.removeNonFeasibleEdges(&BB, DTU, NewUnreachableBB);

  for (BasicBlock *DeadBB : BlocksToErase)
    if (!DeadBB->hasAddressTaken())
      DTU.deleteBB(DeadBB);

  return MadeChanges;
}

bool SCCPLegacy::runOnFunction(Function &F) {
  const DataLayout &DL = F.getDataLayout();
  TargetLibraryInfo &TLI = getAnalysis<TargetLibraryInfoWrapperPass>().getTLI(F);
  DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  DomTreeUpdater DTU(DT, DomTreeUpdater::UpdateStrategy::Lazy);
  bool changed = runSCCP(F, DL, &TLI, DTU);
  return changed;
}

char SCCPLegacy::ID = 0;

FunctionPass *llvm::createSCCPLegacyPass() {
  return new SCCPLegacy();
}