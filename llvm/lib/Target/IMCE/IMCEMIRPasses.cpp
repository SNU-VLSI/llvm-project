#include "IMCEIRPasses.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Function.h"
#include "llvm/InitializePasses.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/IntrinsicsIMCE.h"
#include "llvm/Support/Casting.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "IMCE.h"

using namespace llvm;

#define PMF_DEBUG_TYPE "_"
#define PMF_PASS_NAME "print-machine-function"

namespace {
  class IMCEPrintMachineFunction: public MachineFunctionPass {
    public:
      static char ID;
      static unsigned int count;
      explicit IMCEPrintMachineFunction() : MachineFunctionPass(ID) {
        initializeIMCEPrintMachineFunctionPass(*PassRegistry::getPassRegistry());
      }
      bool runOnMachineFunction(MachineFunction &MF) override;
  };
}
char IMCEPrintMachineFunction::ID = 0;
unsigned int IMCEPrintMachineFunction::count = 0;
INITIALIZE_PASS_BEGIN(IMCEPrintMachineFunction, PMF_DEBUG_TYPE, PMF_PASS_NAME, false, false)
INITIALIZE_PASS_END(IMCEPrintMachineFunction, PMF_DEBUG_TYPE, PMF_PASS_NAME, false, false)
FunctionPass *llvm::createIMCEPrintMachineFunctionPass() {
  return new IMCEPrintMachineFunction();
}

bool IMCEPrintMachineFunction::runOnMachineFunction(MachineFunction &MF) {
  errs() << "===================================================" << "\n";
  errs() << "Machine Function: " << MF.getName() << " " << "iteration " << count << "\n";
  errs() << "===================================================" << "\n";
  MF.dump();
  return false;
}