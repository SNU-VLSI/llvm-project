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

  // little endian.
  Ret += "e";

  // Data mangling.
  Ret += DataLayout::getManglingComponent(TT);

  // Pointers are 32 bit.
  Ret += "-p:32:32:32";

  // All scalar types are naturally aligned.
  Ret += "-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64";

  // Floats and doubles are also naturally aligned.
  Ret += "-f32:32:32-f64:64:64";

  // We prefer 16 bits of aligned for all globals; see
  // above.
  Ret += "-a:8:16";

  // Integer registers are 32bits.
  Ret += "-n32";

  return Ret;
}
} // namespace

/// Create an IMCE architecture model.
IMCETargetMachine::IMCETargetMachine(const Target &T, const Triple &TT, StringRef CPU, StringRef FS,
                                     const TargetOptions &Options, std::optional<Reloc::Model> RM,
                                     std::optional<CodeModel::Model> CM, CodeGenOpt::Level OL,
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
}

void IMCEPassConfig::addPreEmitPass() {
  addPass(createIMCECountedLoopMIRPass());
  if (getOptLevel() != CodeGenOpt::None) {
    addPass(&FinalizeMachineBundlesID);
  }
  addPass(createIMCEPrintMachineFunctionPass());
}

void IMCEPassConfig::addMachineLateOptimization() {
  addPass(createIMCEPrintMachineFunctionPass());
  TargetPassConfig::addMachineLateOptimization();
  addPass(createIMCEPrintMachineFunctionPass());
}