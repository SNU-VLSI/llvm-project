// Part of the LLVM Project, under the Apache License
// v2.0 with LLVM Exceptions. See
// https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH
// LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the IMCE implementation of the
// TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_IMCE_IMCEINSTRINFO_H
#define LLVM_LIB_TARGET_IMCE_IMCEINSTRINFO_H

#include "IMCERegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "IMCEGenInstrInfo.inc"

namespace llvm {

class IMCESubtarget;

class IMCEInstrInfo : public IMCEGenInstrInfo {
  const IMCERegisterInfo RI;
  [[maybe_unused]] IMCESubtarget &STI;

  virtual void anchor();

public:
  explicit IMCEInstrInfo(IMCESubtarget &STI);

  // Return the IMCERegisterInfo, which this class owns.
  const IMCERegisterInfo &getRegisterInfo() const { return RI; }

  bool expandPostRAPseudo(MachineInstr &MI) const override;

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I, const DebugLoc &DL,
                   MCRegister DestReg, MCRegister SrcReg, bool KillSrc) const override;
  bool expandQInst(MachineBasicBlock &MBB, MachineInstr &MI, unsigned int Pop, unsigned int Op) const;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_IMCE_IMCEINSTRINFO_H
