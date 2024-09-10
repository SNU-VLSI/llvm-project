//===- llvm/unittest/CodeGen/SelectionDAGAddressAnalysisTest.cpp  ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RISCVInstrInfo.h"
#include "RISCVRegisterInfo.h"
#include "RISCVSubtarget.h"
#include "RISCVTargetInfo.h"
#include "RISCVTargetMachine.h"
#include "llvm/Analysis/MemoryLocation.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/SelectionDAGAddressAnalysis.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "gtest/gtest.h"
#include <cstdio>

namespace llvm {

class SelectionDAGAddressAnalysisTest : public testing::Test {
protected:
  static void SetUpTestCase() {
    InitializeAllTargets();
    InitializeAllTargetMCs();
    // LLVMInitializeRISCVTarget();
    // LLVMInitializeRISCVTargetMC();
  }

  void SetUp() override {
    StringRef Assembly = "@g = global i32 0\n"
                         "@g_alias = alias i32, i32* @g\n"
                         "define i32 @f() {\n"
                         "  %1 = load i32, i32* @g\n"
                         "  ret i32 %1\n"
                         "}";

    Triple TargetTriple("riscv64-unknown-linux-gnu");
    std::string Error;
    const Target *T = TargetRegistry::lookupTarget("", TargetTriple, Error);
    // FIXME: These tests do not depend on AArch64 specifically, but we have to
    // initialize a target. A skeleton Target for unittests would allow us to
    // always run these tests.
    if (!T)
      GTEST_SKIP();

    TargetOptions Options;
    TM = std::unique_ptr<LLVMTargetMachine>(static_cast<LLVMTargetMachine *>(
        T->createTargetMachine("riscv64-unknown-linux-gnu", "", "", Options, std::nullopt,
                               std::nullopt, CodeGenOpt::Aggressive)));
    if (!TM)
      GTEST_SKIP();

    SMDiagnostic SMError;
    M = parseAssemblyString(Assembly, SMError, Context);
    if (!M)
      report_fatal_error(SMError.getMessage());
    M->setDataLayout(TM->createDataLayout());

    F = M->getFunction("f");
    if (!F)
      report_fatal_error("F?");
    G = M->getGlobalVariable("g");
    if (!G)
      report_fatal_error("G?");
    AliasedG = M->getNamedAlias("g_alias");
    if (!AliasedG)
      report_fatal_error("AliasedG?");

    MachineModuleInfo MMI(TM.get());

    MF = std::make_unique<MachineFunction>(*F, *TM, *TM->getSubtargetImpl(*F), 0, MMI);

    DAG = std::make_unique<SelectionDAG>(*TM, CodeGenOpt::None);
    if (!DAG)
      report_fatal_error("DAG?");
    OptimizationRemarkEmitter ORE(F);
    DAG->init(*MF, ORE, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
  }

  TargetLoweringBase::LegalizeTypeAction getTypeAction(EVT VT) {
    return DAG->getTargetLoweringInfo().getTypeAction(Context, VT);
  }

  EVT getTypeToTransformTo(EVT VT) {
    return DAG->getTargetLoweringInfo().getTypeToTransformTo(Context, VT);
  }

  LLVMContext Context;
  std::unique_ptr<LLVMTargetMachine> TM;
  std::unique_ptr<Module> M;
  Function *F;
  GlobalVariable *G;
  GlobalAlias *AliasedG;
  std::unique_ptr<MachineFunction> MF;
  std::unique_ptr<SelectionDAG> DAG;
};

TEST_F(SelectionDAGAddressAnalysisTest, test1) {
  SDLoc Loc;
  auto Int8VT = EVT::getIntegerVT(Context, 8);
  auto Int32VT = EVT::getIntegerVT(Context, 32);
  auto VecVT = EVT::getVectorVT(Context, Int8VT, 4);

  // initial DAG
  DAG->dump();

  SDValue reg1 = DAG->getCopyFromReg(DAG->getEntryNode(), Loc, RISCV::X3, Int32VT);
  SDValue reg2 = DAG->getCopyFromReg(DAG->getEntryNode(), Loc, RISCV::X4, Int32VT);
  DAG->dump();

  SDValue add_node = DAG->getNode(ISD::ADD, Loc, Int32VT, reg1, reg2);
  SDValue chain = DAG->getCopyToReg(reg1.getValue(1), Loc, RISCV::X5, add_node);
  DAG->setRoot(chain);
  DAG->dump();

  DAG->dumpDotGraph("dag.dot", "dag");
}

} // end namespace llvm
