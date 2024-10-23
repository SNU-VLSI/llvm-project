//===-- IMCETargetMachine.cpp - Define TargetMachine for IMCE ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "IMCETargetMachine.h"
#include "IMCE.h"
#include "TargetInfo/IMCETargetInfo.h"
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

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeIMCETarget() {
  // Register the target.
  RegisterTargetMachine<IMCETargetMachine> X(getTheIMCETarget());
  auto &PR = *PassRegistry::getPassRegistry();
  initializeIMCEDAGToDAGISelPass(PR);
}

namespace {
// TODO: Check.
std::string computeDataLayout(const Triple &TT, StringRef CPU, StringRef FS) {
  std::string Ret;

  // big endian.
  Ret += "E";

  // Data mangling.
  Ret += DataLayout::getManglingComponent(TT);

  // Pointers are 32 bit. (Although we don't have pointers in IMCE)
  Ret += "-p:32:32";

  // scalar type i16 is 16 bit aligned.
  Ret += "-i16:16:16";

  // vector types are 256 bit.
  Ret += "-v256:256:256";

  return Ret;
}
} // namespace

/// Create an IMCE architecture model.
IMCETargetMachine::IMCETargetMachine(const Target &T, const Triple &TT, StringRef CPU, StringRef FS,
                                     const TargetOptions &Options, std::optional<Reloc::Model> RM,
                                     std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                                     bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, FS), TT, CPU, FS, Options,
                        !RM ? Reloc::Static : *RM, getEffectiveCodeModel(CM, CodeModel::Medium),
                        OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()) {
  initAsmInfo();
}

IMCETargetMachine::~IMCETargetMachine() {}

const IMCESubtarget *IMCETargetMachine::getSubtargetImpl(const Function &F) const {
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
    I = std::make_unique<IMCESubtarget>(TargetTriple, CPU, FS, *this);
  }

  return I.get();
}

namespace {
/// IMCE Code Generator Pass Configuration Options.
class IMCEPassConfig : public TargetPassConfig {
public:
  IMCEPassConfig(IMCETargetMachine &TM, PassManagerBase &PM) : TargetPassConfig(TM, PM) {}

  void addIRPasses() override;
  bool addPreISel() override;
  bool addInstSelector() override;
  void addPreRegAlloc() override;
  void addPreEmitPass() override;
  void addMachineLateOptimization() override;
};
} // namespace

TargetPassConfig *IMCETargetMachine::createPassConfig(PassManagerBase &PM) {
  return new IMCEPassConfig(*this, PM);
}

void IMCETargetMahcine::registerPassBuilderCallbacks(PassBuilder &PB) {

#define GET_PASS_REGISTRY "IMCEPassRegistry.def"
#include "llvm/Passes/TargetPassRegistry.inc"

  PB.registerPipelineStartEPCallback(
      [](ModulePassManager &PM, OptimizationLevel Level) {
        FunctionPassManager FPM;
        PM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
        if (EnableHipStdPar)
          PM.addPass(HipStdParAcceleratorCodeSelectionPass());
      });

  PB.registerPipelineEarlySimplificationEPCallback(
      [](ModulePassManager &PM, OptimizationLevel Level) {
        PM.addPass(AMDGPUPrintfRuntimeBindingPass());

        if (Level == OptimizationLevel::O0)
          return;

        PM.addPass(AMDGPUUnifyMetadataPass());

        if (InternalizeSymbols) {
          PM.addPass(InternalizePass(mustPreserveGV));
          PM.addPass(GlobalDCEPass());
        }

        if (EarlyInlineAll && !EnableFunctionCalls)
          PM.addPass(AMDGPUAlwaysInlinePass());
      });

  PB.registerPeepholeEPCallback(
      [](FunctionPassManager &FPM, OptimizationLevel Level) {
        if (Level == OptimizationLevel::O0)
          return;

        FPM.addPass(AMDGPUUseNativeCallsPass());
        if (EnableLibCallSimplify)
          FPM.addPass(AMDGPUSimplifyLibCallsPass());
      });

  PB.registerCGSCCOptimizerLateEPCallback(
      [this](CGSCCPassManager &PM, OptimizationLevel Level) {
        if (Level == OptimizationLevel::O0)
          return;

        FunctionPassManager FPM;

        // Add promote kernel arguments pass to the opt pipeline right before
        // infer address spaces which is needed to do actual address space
        // rewriting.
        if (Level.getSpeedupLevel() > OptimizationLevel::O1.getSpeedupLevel() &&
            EnablePromoteKernelArguments)
          FPM.addPass(AMDGPUPromoteKernelArgumentsPass());

        // Add infer address spaces pass to the opt pipeline after inlining
        // but before SROA to increase SROA opportunities.
        FPM.addPass(InferAddressSpacesPass());

        // This should run after inlining to have any chance of doing
        // anything, and before other cleanup optimizations.
        FPM.addPass(AMDGPULowerKernelAttributesPass());

        if (Level != OptimizationLevel::O0) {
          // Promote alloca to vector before SROA and loop unroll. If we
          // manage to eliminate allocas before unroll we may choose to unroll
          // less.
          FPM.addPass(AMDGPUPromoteAllocaToVectorPass(*this));
        }

        PM.addPass(createCGSCCToFunctionPassAdaptor(std::move(FPM)));
      });

  // FIXME: Why is AMDGPUAttributor not in CGSCC?
  PB.registerOptimizerLastEPCallback(
      [this](ModulePassManager &MPM, OptimizationLevel Level) {
        if (Level != OptimizationLevel::O0) {
          MPM.addPass(AMDGPUAttributorPass(*this));
        }
      });

  PB.registerFullLinkTimeOptimizationLastEPCallback(
      [this](ModulePassManager &PM, OptimizationLevel Level) {
        // We want to support the -lto-partitions=N option as "best effort".
        // For that, we need to lower LDS earlier in the pipeline before the
        // module is partitioned for codegen.
        if (EnableLowerModuleLDS)
          PM.addPass(AMDGPULowerModuleLDSPass(*this));
      });

  PB.registerRegClassFilterParsingCallback(
      [](StringRef FilterName) -> RegAllocFilterFunc {
        if (FilterName == "sgpr")
          return onlyAllocateSGPRs;
        if (FilterName == "vgpr")
          return onlyAllocateVGPRs;
        return nullptr;
      });
}

// void IMCEPassConfig::addIRPasses() {
//   addPass(createIMCECoreIDAssignPass());
//   addPass(createCFGSimplificationPass());
//   TargetPassConfig::addIRPasses();
// }

bool IMCEPassConfig::addPreISel() {
  addPass(createHardwareLoopsLegacyPass());
  addPass(createPrintFunctionsPass());
  addPass(createIMCELoopConversionPass());
  addPass(createPrintFunctionsPass());
  return false;
}

bool IMCEPassConfig::addInstSelector() {
  addPass(createIMCEISelDag(getTM<IMCETargetMachine>(), getOptLevel()));
  return false;
}

void IMCEPassConfig::addPreRegAlloc() {
  addPass(createIMCEPrintMachineFunctionPass());
  TargetPassConfig::addPreRegAlloc();
  // addPass(&MachineCopyPropagationID);
  // addPass(createIMCEPrintMachineFunctionPass());
}

void IMCEPassConfig::addPreEmitPass() {
  addPass(createIMCECountedLoopMIRPass());
  if (getOptLevel() != CodeGenOptLevel::None) {
    addPass(&FinalizeMachineBundlesID);
  }
  addPass(createIMCEPrintMachineFunctionPass());
}

void IMCEPassConfig::addMachineLateOptimization() {
  addPass(createIMCEPrintMachineFunctionPass());
  TargetPassConfig::addMachineLateOptimization();
  addPass(createIMCEPrintMachineFunctionPass());
}