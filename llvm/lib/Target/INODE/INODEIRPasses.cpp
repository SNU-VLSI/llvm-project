// #include "INODEIRPasses.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Function.h"
#include "llvm/InitializePasses.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/IntrinsicsINODE.h"
#include "llvm/Support/Casting.h"
#include "INODE.h"

using namespace llvm;

#define LOOP_CONV_DEBUG_TYPE "INODE-loop-conversion"
#define LOOP_CONV_PASS_NAME "INODE loop conversion"
namespace {
class INODELoopConversion : public FunctionPass {
  LoopInfo *LI;
  LLVMContext *ctx;
  Module *M;

public:
  static char ID;
  INODELoopConversion() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override;
  StringRef getPassName() const override { return LOOP_CONV_PASS_NAME; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LoopInfoWrapperPass>();
  }

private:
  Value *processIterIntr(BasicBlock *BB, IntrinsicInst *II);
  bool processIntr(BasicBlock *BB, IntrinsicInst *II);
  void processSetIntr(BasicBlock *Preheader, IntrinsicInst *II);
};
} // namespace
char INODELoopConversion::ID = 0;

INITIALIZE_PASS_BEGIN(INODELoopConversion, LOOP_CONV_DEBUG_TYPE, LOOP_CONV_PASS_NAME, false, false)
INITIALIZE_PASS_DEPENDENCY(HardwareLoopsLegacy)
INITIALIZE_PASS_DEPENDENCY(LoopInfoWrapperPass)
INITIALIZE_PASS_END(INODELoopConversion, LOOP_CONV_DEBUG_TYPE, LOOP_CONV_PASS_NAME, false, false)

FunctionPass *llvm::createINODELoopConversionPass() {
  return new INODELoopConversion();
}

Value *INODELoopConversion::processIterIntr(BasicBlock *BB, IntrinsicInst *II) {
  auto I32Ty = Type::getInt32Ty(*ctx);

  IRBuilder<> SLIIBuilder(II);
  Value *counter = II->getOperand(0);
  Value *zextTruncCounter = SLIIBuilder.CreateZExtOrTrunc(counter, I32Ty);
  Function *func =
      Intrinsic::getDeclaration(M, Intrinsic::INODE_cloop_begin);
  CallInst *cloopBeginCall =
      SLIIBuilder.CreateCall(func, {zextTruncCounter}, "cloop.begin");
  Value *cloopBegin = cloopBeginCall;
  return cast<Value>(cloopBegin);
}

bool INODELoopConversion::processIntr(BasicBlock *BB, IntrinsicInst *II) {
  switch (II->getIntrinsicID()) {
  default:
    llvm_unreachable("Unsupported intrinsic.");
  case Intrinsic::test_start_loop_iterations:
    // processTestStartIntr(BB, II);
    llvm_unreachable("Unsupported intrinsic test.start.loop");
    break;
  case Intrinsic::test_set_loop_iterations:
    // processTestSetIntr(BB, II);
    llvm_unreachable("Unsupported intrinsic test.set.loop");
    break;
  case Intrinsic::set_loop_iterations:
    processSetIntr(BB, II);
    break;
  case Intrinsic::start_loop_iterations:
    // processStartIntr(BB, II);
    llvm_unreachable("Unsupported intrinsic start.loop");
    break;
  case Intrinsic::loop_decrement_reg:
    // processDecRegIntr(BB, II);
    llvm_unreachable("Unsupported intrinsic loop.decrement.reg");
    break;
  }
  return true;
}

bool isINODECallIntrID(const CallInst *CI, Intrinsic::ID IID) {
  return CI != nullptr && CI->getIntrinsicID() == IID;
}

Instruction *findINODEHwLoopIntrinsic(BasicBlock *BB, Intrinsic::ID IID) {
  auto isIntIDInst = [&IID](Instruction &I) {
    // return dyn_cast<CallInst>(&I) != nullptr && CI->getIntrinsicID() == IID;
    return isINODECallIntrID(dyn_cast<CallInst>(&I), IID);
  };
  auto Inst = std::find_if(BB->begin(), BB->end(), isIntIDInst);
  return Inst != BB->end() ? &(*Inst) : nullptr;
}

void INODELoopConversion::processSetIntr(BasicBlock *Preheader,
                                            IntrinsicInst *II) {
  auto I32Ty = Type::getInt32Ty(*ctx);
  Value *counter = processIterIntr(Preheader, II);
  II->eraseFromParent();

  // The preheader successor will be the loop's header.
  Loop *L = LI->getLoopFor(Preheader->getSingleSuccessor());

  SmallVector<BasicBlock*, 4> ExitingBlocks;
  L->getExitingBlocks(ExitingBlocks);

  auto *LoopDecInstr = [&]() -> Instruction* {
    for (BasicBlock *LoopBB : ExitingBlocks) {
      if (auto *Inst = findINODEHwLoopIntrinsic(LoopBB, Intrinsic::loop_decrement))
        return Inst;
    }
    return nullptr;
  }();

  BasicBlock *Header = L->getHeader();
  BasicBlock *Latch = L->getLoopLatch();

  assert(LoopDecInstr && "Latch must contain loop_decrement intrinsic.");

  // Put the PHI in the loop header.
  IRBuilder<> HeaderBuilder(Header->getFirstNonPHI());
  PHINode *loopPhi =
      HeaderBuilder.CreatePHI(LoopDecInstr->getOperand(0)->getType(), 2);
  loopPhi->addIncoming(counter, Preheader);

  // Replace loop_decrement intrinsic w/ Colossus equivalent.
  IRBuilder<> DecBuilder(LoopDecInstr);
  Value *zextTruncPhi = DecBuilder.CreateZExtOrTrunc(loopPhi, I32Ty);
  CallInst *cloopEnd = DecBuilder.CreateCall(
      Intrinsic::getDeclaration(M, Intrinsic::INODE_cloop_end),
      {zextTruncPhi}, "cloop.end");

  Value *indVar =
      DecBuilder.CreateExtractValue(cloopEnd, 0, "cloop.end.iv");
  Value *cc = DecBuilder.CreateExtractValue(cloopEnd, 1, "cloop.end.cc");

  Value *truncCC =
      DecBuilder.CreateTrunc(cc, Type::getInt1Ty(*ctx), "cloop.end.cc.trunc");

  LoopDecInstr->replaceAllUsesWith(truncCC);
  loopPhi->addIncoming(indVar, Latch);
  LoopDecInstr->eraseFromParent();
}

bool INODELoopConversion::runOnFunction(Function &F) {
  SmallVector<IntrinsicInst *, 4> Intrinsics;
  LI = &getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
  bool changed = false;

  // Filter to find the intrinsics.
  auto MatchIntr = [](const Instruction &I) -> bool {
    auto Intr = dyn_cast<IntrinsicInst>(&I);
    if (!Intr)
      return false;
    switch (Intr->getIntrinsicID()) {
    default:
      return false;
    case Intrinsic::test_set_loop_iterations:
    case Intrinsic::test_start_loop_iterations:
    case Intrinsic::set_loop_iterations:
    case Intrinsic::start_loop_iterations:
    case Intrinsic::loop_decrement_reg:
      return true;
    }
  };

  // Search for the hardware loop intrinsics as emitted by HardwareLoops.
  for (auto BBi = F.begin(); BBi != F.end(); ++BBi) {
    BasicBlock *BB = &(*BBi);
    auto Intrs =
        llvm::make_filter_range(make_range(BB->begin(), BB->end()), MatchIntr);
    for (auto Inst = Intrs.begin(); Inst != Intrs.end(); ++Inst) {
      IntrinsicInst *II = cast<IntrinsicInst>(&(*Inst));
      Intrinsics.push_back(II);
    }
  }

  ctx = &F.getContext();
  M = F.getParent();

  for (auto Intr : Intrinsics) {
    changed |= processIntr(Intr->getParent(), Intr);
  }

  return changed;
}