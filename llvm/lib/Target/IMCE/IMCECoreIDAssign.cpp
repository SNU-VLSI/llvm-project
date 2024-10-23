#include "IMCE.h"
#include "IMCECoreIDAssign.h"
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
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/IntrinsicsIMCE.h"
#include "llvm/IR/PassManager.h"
#include <utility>

using namespace llvm;

static cl::opt<unsigned int> CoreHID("imce_hid", cl::desc("IMCE core height ID"), cl::init(0u));
static cl::opt<unsigned int> CoreWID("imce_wid", cl::desc("IMCE core width  ID"), cl::init(0u));

#define DEBUG_TYPE "imce_core_id_assign"
#define PASS_NAME "IMCE_CORE_ID_ASSIGN"

namespace {

class IMCECoreIDAssign : public FunctionPass {

public:
  static char ID;

  IMCECoreIDAssign() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override;

  StringRef getPassName() const override { return PASS_NAME; }
};

} // end anonymous namespace

INITIALIZE_PASS_BEGIN(IMCECoreIDAssign, DEBUG_TYPE, PASS_NAME, false, false)
INITIALIZE_PASS_END(IMCECoreIDAssign, DEBUG_TYPE, PASS_NAME, false, false)

// return true if the function was modified.
static bool runCoreAssign(Function &F) {
  LLVM_DEBUG(dbgs() << "Core Assign on function '" << F.getName() << "'\n");

  // find llvm.IMCE.HID and llvm.IMCE.WID intrinsics and replace the result with core hid and core wid
  // from the command line arguments
  std::vector<Instruction *> instructionsToDelete;
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (auto *II = dyn_cast<IntrinsicInst>(&I)) {
        if (II->getIntrinsicID() == Intrinsic::IMCE_HID) {
          II->replaceAllUsesWith(ConstantInt::get(Type::getInt16Ty(F.getContext()), CoreHID));
          instructionsToDelete.push_back(II);
        } else if (II->getIntrinsicID() == Intrinsic::IMCE_WID) {
          // replace the result with the core wid
          II->replaceAllUsesWith(ConstantInt::get(Type::getInt16Ty(F.getContext()), CoreWID));
          instructionsToDelete.push_back(II);
        }
      }
    }
  }

  for(Instruction *I : instructionsToDelete) {
    I->eraseFromParent();
  }

  return instructionsToDelete.size() > 0;
}

bool IMCECoreIDAssign::runOnFunction(Function &F) {
  bool changed = runCoreAssign(F);
  LLVM_DEBUG(F.print(dbgs()));
  return runCoreAssign(F);
}

char IMCECoreIDAssign::ID = 0;

FunctionPass *llvm::createIMCECoreIDAssignPass() {
  return new IMCECoreIDAssign();
}

// ====================================================
// new pass manager wrapper
// ====================================================
PreservedAnalyses IMCECoreIDAssignWrapPass::run(Function &F, FunctionAnalysisManager &AM) {
  bool changed = runCoreAssign(F);
  LLVM_DEBUG(F.print(dbgs()));

  if (!changed)
    return PreservedAnalyses::all();

  auto PA = PreservedAnalyses();
  return PA;
}