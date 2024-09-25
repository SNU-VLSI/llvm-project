#include "IMCEIRPasses.h"

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