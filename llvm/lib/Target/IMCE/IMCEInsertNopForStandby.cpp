//===-- IMCEInsertNopForStandby.cpp - Insert NOP before STANDBY -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements a machine function pass that inserts a NOP instruction
// between vector immediate instructions and STANDBY instructions to work around
// a hardware bug.
//
// Hardware Bug: The IMCE hardware has a bug where executing STANDBY immediately
// after a vector immediate instruction (VADDI, VSUBI, etc.) that writes to the
// register used by STANDBY causes incorrect behavior. A NOP must be inserted
// between these instructions.
//
//===----------------------------------------------------------------------===//

#include "IMCE.h"
#include "IMCEInstrInfo.h"
#include "IMCESubtarget.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

#define GET_INSTRINFO_ENUM
#include "IMCEGenInstrInfo.inc"

using namespace llvm;

#define DEBUG_TYPE "imce-insert-nop-for-standby"
#define PASS_NAME "IMCE Insert NOP for STANDBY Hardware Bug"

namespace {

class IMCEInsertNopForStandby : public MachineFunctionPass {
  const IMCEInstrInfo *TII;

public:
  static char ID;

  IMCEInsertNopForStandby() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  StringRef getPassName() const override { return PASS_NAME; }

private:
  bool insertNopForStandbyHazard(MachineBasicBlock &MBB);
  bool isVectorImmediateInst(unsigned Opcode) const;
};

} // end anonymous namespace

char IMCEInsertNopForStandby::ID = 0;

INITIALIZE_PASS(IMCEInsertNopForStandby, DEBUG_TYPE, PASS_NAME, false, false)

FunctionPass *llvm::createIMCEInsertNopForStandbyPass() {
  return new IMCEInsertNopForStandby();
}

bool IMCEInsertNopForStandby::isVectorImmediateInst(unsigned Opcode) const {
  // Check if the instruction is a vector immediate instruction
  // that could trigger the STANDBY hardware bug
  switch (Opcode) {
  case IMCE::IMCE_VADDI_INST:
  case IMCE::IMCE_VSUBI_INST:
  case IMCE::IMCE_VANDI_INST:
  case IMCE::IMCE_VORI_INST:
  case IMCE::IMCE_VXORI_INST:
  case IMCE::IMCE_VSRLI_INST:
  case IMCE::IMCE_VSLLI_INST:
  case IMCE::IMCE_VSRAI_INST:
  case IMCE::IMCE_VMAXI_INST:
  case IMCE::IMCE_VMINI_INST:
  case IMCE::IMCE_VMULTLI_INST:
  case IMCE::IMCE_VMULTHI_INST:
    return true;
  default:
    return false;
  }
}

bool IMCEInsertNopForStandby::insertNopForStandbyHazard(MachineBasicBlock &MBB) {
  bool Modified = false;
  MachineInstr *PrevInstr = nullptr;

  for (auto I = MBB.begin(), E = MBB.end(); I != E; ++I) {
    MachineInstr &MI = *I;

    // Check if current instruction is STANDBY
    if (MI.getOpcode() == IMCE::IMCE_STANDBY_INST) {
      // Check if previous instruction is a vector immediate instruction
      if (PrevInstr && isVectorImmediateInst(PrevInstr->getOpcode())) {
        // Check if the vector immediate instruction writes to a register
        // that STANDBY reads (checking for data dependency)
        if (PrevInstr->getNumOperands() > 0 &&
            PrevInstr->getOperand(0).isReg() &&
            MI.getNumOperands() > 1 &&
            MI.getOperand(1).isReg()) {

          Register VImmDest = PrevInstr->getOperand(0).getReg();
          Register StandbySource = MI.getOperand(1).getReg();

          if (VImmDest == StandbySource) {
            // Insert NOP before STANDBY
            BuildMI(MBB, I, MI.getDebugLoc(), TII->get(IMCE::IMCE_NOP_INST));
            Modified = true;

            LLVM_DEBUG(dbgs() << "Inserted NOP before STANDBY due to hazard with "
                              << "previous vector immediate instruction\n");
          }
        }
      }
    }

    // Track the previous non-debug instruction
    if (!MI.isDebugInstr())
      PrevInstr = &MI;
  }

  return Modified;
}

bool IMCEInsertNopForStandby::runOnMachineFunction(MachineFunction &MF) {
  TII = static_cast<const IMCEInstrInfo *>(MF.getSubtarget().getInstrInfo());
  bool Modified = false;

  for (MachineBasicBlock &MBB : MF) {
    Modified |= insertNopForStandbyHazard(MBB);
  }

  return Modified;
}
