#include "llvm/Pass.h"
#include "llvm/PassRegistry.h"
#include "llvm/IR/Function.h"

namespace llvm {
  void initializePrintFunctionsPass(PassRegistry &);
  FunctionPass *createPrintFunctionsPass(); 
}