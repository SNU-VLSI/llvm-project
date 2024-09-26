#include "IMCEIRPasses.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Function.h"
#include "llvm/InitializePasses.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/IntrinsicsIMCE.h"
#include "llvm/Support/Casting.h"
#include "IMCE.h"

using namespace llvm;

#define PRINT_FUNCTIONS_DEBUG_TYPE "hardware-loops"
#define PRINT_FUNCTIONS_NAME "print functions"

namespace {
  class PrintFunctions : public FunctionPass {
    public:
      static char ID;

      PrintFunctions() : FunctionPass(ID) {
        initializePrintFunctionsPass(*PassRegistry::getPassRegistry());
      }

      bool runOnFunction(Function &F) override;
  };
}
bool PrintFunctions::runOnFunction(Function &F) {
  // errs() << "Function: " << F.getName() << "\n";

  // // print basic blocks of functions
  // for (auto &BB : F) {
  //   errs() << "Basic Block: " << BB.getName() << "\n";
  //   // print instructions of basic blocks
  //   for (auto &I : BB) {
  //     errs() << "Instruction: " << I.getOpcodeName() << "\n";
  //   }
  // }
  errs() << "Print Function Pass Run" << "\n";
  F.dump();
  return false;
}

char PrintFunctions::ID = 0;
INITIALIZE_PASS_BEGIN(PrintFunctions, PRINT_FUNCTIONS_DEBUG_TYPE, PRINT_FUNCTIONS_NAME, false, false)
INITIALIZE_PASS_END(PrintFunctions, PRINT_FUNCTIONS_DEBUG_TYPE, PRINT_FUNCTIONS_NAME, false, false)

FunctionPass *llvm::createPrintFunctionsPass() { return new PrintFunctions();}

#define LOOP_CONV_DEBUG_TYPE "imce-loop-conversion"
#define LOOP_CONV_PASS_NAME "imce loop conversion"
namespace {
enum class metadata : uint16_t {
  none = 0,
  tripCountOKForRpt = 1,
};
using md = metadata;
class IMCELoopConversion : public FunctionPass {
  LoopInfo *LI;
  LLVMContext *ctx;
  Module *M;

public:
  static char ID;
  IMCELoopConversion() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override;
  StringRef getPassName() const override { return LOOP_CONV_PASS_NAME; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LoopInfoWrapperPass>();
  }

private:
  Value *processIterIntr(BasicBlock *BB, IntrinsicInst *II, md Metadata);
  bool processIntr(BasicBlock *BB, IntrinsicInst *II);
  // BasicBlock *processLoopGuardIntr(BasicBlock *Predecessor, IntrinsicInst *II);
  // void processTestSetIntr(BasicBlock *Predecessor, IntrinsicInst *II);
  // void processTestStartIntr(BasicBlock *Predecessor, IntrinsicInst *II);
  void processSetIntr(BasicBlock *Preheader, IntrinsicInst *II);
  // void processStartIntr(BasicBlock *BB, IntrinsicInst *II);
  // void processDecRegIntr(BasicBlock *BB, IntrinsicInst *II);
};
} // namespace
char IMCELoopConversion::ID = 0;

INITIALIZE_PASS_BEGIN(IMCELoopConversion, LOOP_CONV_DEBUG_TYPE, LOOP_CONV_PASS_NAME, false, false)
INITIALIZE_PASS_DEPENDENCY(HardwareLoopsLegacy)
INITIALIZE_PASS_DEPENDENCY(LoopInfoWrapperPass)
INITIALIZE_PASS_END(IMCELoopConversion, LOOP_CONV_DEBUG_TYPE, LOOP_CONV_PASS_NAME, false, false)

FunctionPass *llvm::createIMCELoopConversionPass() {
  return new IMCELoopConversion();
}

Value *IMCELoopConversion::processIterIntr(BasicBlock *BB,
                                               IntrinsicInst *II, md Metadata) {
  auto I32Ty = Type::getInt32Ty(*ctx);
  auto I64Ty = Type::getInt64Ty(*ctx);
  auto metadata = ConstantInt::get(I32Ty, static_cast<uint16_t>(Metadata));

  IRBuilder<> SLIIBuilder(II);
  Value *counter = II->getOperand(0);
  bool isi64TC = counter->getType()->getIntegerBitWidth() == 64;
  Value *zextTruncCounter = SLIIBuilder.CreateZExtOrTrunc(counter, I32Ty);
  Function *func =
      Intrinsic::getDeclaration(M, Intrinsic::IMCE_cloop_begin);
  CallInst *cloopBeginCall =
      SLIIBuilder.CreateCall(func, {zextTruncCounter, metadata}, "cloop.begin");
  Value *cloopBegin = isi64TC
                          ? SLIIBuilder.CreateZExtOrTrunc(cloopBeginCall, I64Ty)
                          : cloopBeginCall;
  return cast<Value>(cloopBegin);
}

bool IMCELoopConversion::processIntr(BasicBlock *BB, IntrinsicInst *II) {
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

bool isCallIntrID(const CallInst *CI, Intrinsic::ID IID) {
  return CI != nullptr && CI->getIntrinsicID() == IID;
}

Instruction *findHwLoopIntrinsic(BasicBlock *BB, Intrinsic::ID IID) {
  auto isIntIDInst = [&IID](Instruction &I) {
    return isCallIntrID(dyn_cast<CallInst>(&I), IID);
  };
  auto Inst = std::find_if(BB->begin(), BB->end(), isIntIDInst);
  return Inst != BB->end() ? &(*Inst) : nullptr;
}

void IMCELoopConversion::processSetIntr(BasicBlock *Preheader,
                                            IntrinsicInst *II) {
  auto I32Ty = Type::getInt32Ty(*ctx);
  Value *counter = processIterIntr(Preheader, II, md::tripCountOKForRpt);
  auto metadata =
      ConstantInt::get(I32Ty, static_cast<uint16_t>(md::tripCountOKForRpt));
  II->eraseFromParent();

  // The preheader successor will be the loop's header.
  Loop *L = LI->getLoopFor(Preheader->getSingleSuccessor());

  SmallVector<BasicBlock*, 4> ExitingBlocks;
  L->getExitingBlocks(ExitingBlocks);

  auto *LoopDecInstr = [&]() -> Instruction* {
    for (BasicBlock *LoopBB : ExitingBlocks) {
      if (auto *Inst = findHwLoopIntrinsic(LoopBB, Intrinsic::loop_decrement))
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
      Intrinsic::getDeclaration(M, Intrinsic::IMCE_cloop_end),
      {zextTruncPhi, metadata}, "cloop.end");

  Value *indVar =
      DecBuilder.CreateExtractValue(cloopEnd, 0, "cloop.end.iv");
  Value *cc = DecBuilder.CreateExtractValue(cloopEnd, 1, "cloop.end.cc");

  Value *truncCC =
      DecBuilder.CreateTrunc(cc, Type::getInt1Ty(*ctx), "cloop.end.cc.trunc");

  // If counter is 64b, make indVar 64b as well so phi gets arguments of the
  // same type.
  auto I64Ty = Type::getInt64Ty(*ctx);
  bool isi64TC = counter->getType()->getIntegerBitWidth() == 64;
  indVar = isi64TC ? DecBuilder.CreateZExtOrTrunc(indVar, I64Ty) : indVar;

  LoopDecInstr->replaceAllUsesWith(truncCC);
  loopPhi->addIncoming(indVar, Latch);
  LoopDecInstr->eraseFromParent();
}

bool IMCELoopConversion::runOnFunction(Function &F) {
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