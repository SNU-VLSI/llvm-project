//===-- IMCERegisterInfo.h - IMCE Register Information
// Impl -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License
// v2.0 with LLVM Exceptions. See
// https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH
// LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the IMCE implementation of the
// TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_IMCE_IMCEREGISTERINFO_H
#define LLVM_LIB_TARGET_IMCE_IMCEREGISTERINFO_H

#define GET_REGINFO_HEADER
#include "IMCEGenRegisterInfo.inc"

namespace llvm {

struct IMCERegisterInfo : public IMCEGenRegisterInfo {
  IMCERegisterInfo();

  /// Code Generation virtual methods...
  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj, unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  Register getFrameRegister(const MachineFunction &MF) const override;
};

} // end namespace llvm

#endif
