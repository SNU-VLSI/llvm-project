#include "INODEIRPasses.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Function.h"
#include "llvm/InitializePasses.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/IntrinsicsINODE.h"
#include "llvm/Support/Casting.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "INODE.h"

using namespace llvm;

#define PMF_DEBUG_TYPE "_"
#define PMF_PASS_NAME "print-machine-function"

namespace {
  class INODEPrintMachineFunction: public MachineFunctionPass {
    public:
      static char ID;
      static unsigned int count;
      explicit INODEPrintMachineFunction() : MachineFunctionPass(ID) {
        initializeINODEPrintMachineFunctionPass(*PassRegistry::getPassRegistry());
      }
      bool runOnMachineFunction(MachineFunction &MF) override;
  };
}
char INODEPrintMachineFunction::ID = 0;
unsigned int INODEPrintMachineFunction::count = 0;
INITIALIZE_PASS_BEGIN(INODEPrintMachineFunction, PMF_DEBUG_TYPE, PMF_PASS_NAME, false, false)
INITIALIZE_PASS_END(INODEPrintMachineFunction, PMF_DEBUG_TYPE, PMF_PASS_NAME, false, false)
FunctionPass *llvm::createINODEPrintMachineFunctionPass() {
  return new INODEPrintMachineFunction();
}

bool INODEPrintMachineFunction::runOnMachineFunction(MachineFunction &MF) {
  errs() << "===================================================" << "\n";
  errs() << "Machine Function: " << MF.getName() << " " << "iteration " << count << "\n";
  errs() << "===================================================" << "\n";
  MF.dump();
  return false;
}