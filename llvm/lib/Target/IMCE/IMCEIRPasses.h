#include "llvm/Pass.h"
#include "llvm/InitializePasses.h"
#include "llvm/PassRegistry.h"

namespace llvm {
  void initializePrintFunctionsPass(PassRegistry &);
  FunctionPass *createPrintFunctionsPass(); 
}