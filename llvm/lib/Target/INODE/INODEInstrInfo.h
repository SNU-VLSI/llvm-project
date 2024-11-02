// Part of the LLVM Project, under the Apache License
// v2.0 with LLVM Exceptions. See
// https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH
// LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the INODE implementation of the
// TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_INODE_INODEINSTRINFO_H
#define LLVM_LIB_TARGET_INODE_INODEINSTRINFO_H

#include "INODERegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "INODEGenInstrInfo.inc"

namespace llvm {

class INODESubtarget;

class INODEInstrInfo : public INODEGenInstrInfo {
  const INODERegisterInfo RI;
  INODESubtarget &STI;

  virtual void anchor();

public:
  explicit INODEInstrInfo(INODESubtarget &STI);

  // Return the INODERegisterInfo, which this class owns.
  const INODERegisterInfo &getRegisterInfo() const { return RI; }

  bool expandPostRAPseudo(MachineInstr &MI) const override;

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I, const DebugLoc &DL,
                   MCRegister DestReg, MCRegister SrcReg, bool KillSrc) const override;
  bool expandQInst(MachineBasicBlock &MBB, MachineInstr &MI, unsigned int Pop, unsigned int Op) const;
  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MBBI, Register SrcReg,
                           bool IsKill, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI,
                           Register VReg) const override;
  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MBBI, Register DstReg,
                            int FrameIndex, const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI,
                            Register VReg) const override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_INODE_INODEINSTRINFO_H
