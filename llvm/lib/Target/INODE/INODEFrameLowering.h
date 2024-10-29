//===-- INODEFrameLowering.h - Frame lowering for INODE -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_INODE_INODEFRAMELOWERING_H
#define LLVM_LIB_TARGET_INODE_INODEFRAMELOWERING_H

#include "llvm/ADT/IndexedMap.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class INODETargetMachine;
class INODESubtarget;

class INODEFrameLowering : public TargetFrameLowering {
public:
  INODEFrameLowering();

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  bool hasFP(const MachineFunction &MF) const override;
};
} // end namespace llvm

#endif
