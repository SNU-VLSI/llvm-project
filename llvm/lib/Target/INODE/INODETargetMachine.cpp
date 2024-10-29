//===-- INODETargetMachine.cpp - Define TargetMachine for INODE ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "INODE.h"
#include "INODETargetMachine.h"
#include "INODECoreIDAssign.h"
#include "TargetInfo/INODETargetInfo.h"
#include "llvm/CodeGen/GlobalISel/IRTranslator.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelect.h"
#include "llvm/CodeGen/GlobalISel/Legalizer.h"
#include "llvm/CodeGen/GlobalISel/RegBankSelect.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/SCCP.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeINODETarget() {
  // Register the target.
  RegisterTargetMachine<INODETargetMachine> X(getTheINODETarget());
  auto &PR = *PassRegistry::getPassRegistry();
  initializeINODEDAGToDAGISelPass(PR);
}

namespace {
// TODO: Check.
std::string computeDataLayout(const Triple &TT, StringRef CPU, StringRef FS) {
  std::string Ret;

  // big endian.
  Ret += "E";

  // Data mangling.
  Ret += DataLayout::getManglingComponent(TT);

  // Pointers are 32 bit. (Although we don't have pointers in INODE)
  Ret += "-p:32:32";

  // scalar type i16 is 16 bit aligned.
  Ret += "-i16:16:16";

  // vector types are 256 bit.
  Ret += "-v256:256:256";

  return Ret;
}
} // namespace

/// Create an INODE architecture model.
INODETargetMachine::INODETargetMachine(const Target &T, const Triple &TT, StringRef CPU, StringRef FS,
                                     const TargetOptions &Options, std::optional<Reloc::Model> RM,
                                     std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                                     bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, FS), TT, CPU, FS, Options,
                        !RM ? Reloc::Static : *RM, getEffectiveCodeModel(CM, CodeModel::Medium),
                        OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()) {
  initAsmInfo();
}

INODETargetMachine::~INODETargetMachine() {}

const INODESubtarget *INODETargetMachine::getSubtargetImpl(const Function &F) const {
  Attribute CPUAttr = F.getFnAttribute("target-cpu");
  Attribute FSAttr = F.getFnAttribute("target-features");

  std::string CPU =
      !CPUAttr.hasAttribute(Attribute::None) ? CPUAttr.getValueAsString().str() : TargetCPU;
  std::string FS =
      !FSAttr.hasAttribute(Attribute::None) ? FSAttr.getValueAsString().str() : TargetFS;

  auto &I = SubtargetMap[CPU + FS];
  if (!I) {
    // This needs to be done before we create a new
    // subtarget since any creation will depend on the
    // TM and the code generation flags on the function
    // that reside in TargetOptions.
    resetTargetOptions(F);
    I = std::make_unique<INODESubtarget>(TargetTriple, CPU, FS, *this);
  }

  return I.get();
}

namespace {
/// INODE Code Generator Pass Configuration Options.
class INODEPassConfig : public TargetPassConfig {
public:
  INODEPassConfig(INODETargetMachine &TM, PassManagerBase &PM) : TargetPassConfig(TM, PM) {}

  void addIRPasses() override;
  bool addPreISel() override;
  bool addInstSelector() override;
  void addPreRegAlloc() override;
  void addPreEmitPass() override;
  void addMachineLateOptimization() override;
};
} // namespace

TargetPassConfig *INODETargetMachine::createPassConfig(PassManagerBase &PM) {
  return new INODEPassConfig(*this, PM);
}

void INODETargetMachine::registerPassBuilderCallbacks(PassBuilder &PB) {
#define GET_PASS_REGISTRY "INODEPassRegistry.def"
#include "llvm/Passes/TargetPassRegistry.inc"
  PB.registerPipelineStartEPCallback(
      [](ModulePassManager &PM, OptimizationLevel Level) {
        FunctionPassManager FPM;
        FPM.addPass(INODECoreIDAssignWrapPass());
        FPM.addPass(SCCPPass());
        PM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
      });
}

void INODEPassConfig::addIRPasses() {
  addPass(createINODECoreIDAssignPass());
  addPass(createSCCPLegacyPass());
  TargetPassConfig::addIRPasses();
}

bool INODEPassConfig::addPreISel() {
  addPass(createHardwareLoopsLegacyPass());
  // addPass(createPrintFunctionsPass());
  addPass(createINODELoopConversionPass());
  // addPass(createPrintFunctionsPass());
  return false;
}

bool INODEPassConfig::addInstSelector() {
  addPass(createINODEISelDag(getTM<INODETargetMachine>(), getOptLevel()));
  return false;
}

void INODEPassConfig::addPreRegAlloc() {
  // addPass(createINODEPrintMachineFunctionPass());
  TargetPassConfig::addPreRegAlloc();
  // addPass(&MachineCopyPropagationID);
  // addPass(createINODEPrintMachineFunctionPass());
}

void INODEPassConfig::addPreEmitPass() {
  addPass(createINODECountedLoopMIRPass());
  if (getOptLevel() != CodeGenOptLevel::None) {
    addPass(&FinalizeMachineBundlesID);
  }
  // addPass(createINODEPrintMachineFunctionPass());
}

void INODEPassConfig::addMachineLateOptimization() {
  // addPass(createINODEPrintMachineFunctionPass());
  TargetPassConfig::addMachineLateOptimization();
  // addPass(createINODEPrintMachineFunctionPass());
}