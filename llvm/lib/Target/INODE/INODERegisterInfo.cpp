//===-- INODERegisterInfo.cpp - INODE Register Information
//------------------===//
//
// Part of the LLVM Project, under the Apache License
// v2.0 with LLVM Exceptions. See
// https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH
// LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the INODE implementation of the
// TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "INODERegisterInfo.h"
#include "INODEFrameLowering.h"
#include "INODESubtarget.h"
#include "MCTargetDesc/INODEMCTargetDesc.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/Register.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/MC/MCRegister.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "INODEGenRegisterInfo.inc"

INODERegisterInfo::INODERegisterInfo() : INODEGenRegisterInfo(INODE::SReg1) {}

const MCPhysReg *INODERegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_INODE_SaveList;
}

BitVector INODERegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  // SReg7 is the stack pointer.
  Reserved.set(INODE::SReg7);
  // SReg6 is the frame pointer.
  Reserved.set(INODE::SReg6);
  // markSuperRegs(Reserved, INODE::S0); // zero
  markSuperRegs(Reserved, INODE::SReg0); // zero

  return Reserved;
}

bool INODERegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                                           unsigned FIOperandNum, RegScavenger *RS) const {
  MachineInstr &Instr = *MI;
  MachineFunction &MF = *Instr.getParent()->getParent();
  const TargetFrameLowering *TFI =
      MF.getSubtarget<INODESubtarget>().getFrameLowering();

  int FrameIndex = Instr.getOperand(FIOperandNum).getIndex();
  Register FrameReg;
  auto StackOff = TFI->getFrameIndexReference(MF, FrameIndex, FrameReg);
  int Offset = (int)StackOff.getFixed();

  // INODE addressing forms differ for load/store.
  // - Store:  (outs), (ins SGPR:$rd, simm20:$imm, SGPR:$rs1)  => "$rd, $imm, $rs1"
  //   Here the FrameIndex is used in the imm position. Replace FI with the
  //   computed immediate offset and set the base register operand to FrameReg.
  // - Others (e.g., loads/addi-like I-type): (rd, rs1, imm) order. Replace FI
  //   operand with FrameReg and fold the offset into the following imm.

  unsigned Opc = Instr.getOpcode();
  const unsigned NumOps = Instr.getNumOperands();

  if (Opc == INODE::INODE_STORE_INST) {
    // Expect: FI is the imm operand, base register is next operand.
    unsigned ImmIdx = FIOperandNum;
    unsigned BaseIdx = FIOperandNum + 1;
    // Be defensive about indices.
    if (BaseIdx < NumOps) {
      Instr.getOperand(ImmIdx).ChangeToImmediate(Offset);
      Instr.getOperand(BaseIdx).ChangeToRegister(FrameReg, false /*isDef*/);
    } else {
      // Fallback to default pattern if unexpected layout.
      Instr.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false /*isDef*/);
      if (FIOperandNum + 1 < NumOps) {
        Instr.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
      }
    }
  } else {
    // Default: [base, imm] order with FI representing the base.
    // If a literal imm already follows, fold the frame offset into it.
    int NewImm = Offset;
    if (FIOperandNum + 1 < NumOps && Instr.getOperand(FIOperandNum + 1).isImm()) {
      NewImm += Instr.getOperand(FIOperandNum + 1).getImm();
    }
    Instr.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false /*isDef*/);
    if (FIOperandNum + 1 < NumOps) {
      Instr.getOperand(FIOperandNum + 1).ChangeToImmediate(NewImm);
    }
  }

  return false;
}

Register INODERegisterInfo::getFrameRegister(const MachineFunction &MF) const { return INODE::SReg6; }
