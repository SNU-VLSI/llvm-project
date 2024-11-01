//===-- INODEISelLowering.h - INODE DAG lowering interface --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that INODE uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_INODE_INODEISELLOWERING_H
#define LLVM_LIB_TARGET_INODE_INODEISELLOWERING_H

#include "INODE.h"
#include "INODEInstrInfo.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

class INODESubtarget;
class INODESubtarget;

namespace INODEISD {
enum NodeType : unsigned {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,

  // Return with a glue operand. Operand 0 is the chain operand.
  RET_GLUE,

  // Calls a function.  Operand 0 is the chain operand and operand 1
  // is the target address.  The arguments start at operand 2.
  // There is an optional glue operand at the end.
  CALL,

  // Hardware loops
  CLOOP_BEGIN_VALUE,
  CLOOP_BEGIN_TERMINATOR,
  CLOOP_END_VALUE,
  CLOOP_END_BRANCH,
  CLOOP_GUARD_BRANCH,
  BR_CC,
};
} // end namespace INODEISD

class INODETargetLowering : public TargetLowering {
  const INODESubtarget &Subtarget;

public:
  explicit INODETargetLowering(const TargetMachine &TM, const INODESubtarget &STI);

  // Override TargetLowering methods.
  bool hasAndNot(SDValue X) const override { return true; }
  const char *getTargetNodeName(unsigned Opcode) const override;

  // Override required hooks.
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
                               SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                      SelectionDAG &DAG) const override;

  SDValue LowerCall(CallLoweringInfo &CLI, SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

  SDValue LowerINTRINSIC_W_CHAIN(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerINTRINSIC_WO_CHAIN(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerINTRINSIC_VOID(SDValue Op, SelectionDAG &DAG) const;
  void ReplaceNodeResults(SDNode *N, SmallVectorImpl<SDValue> &Results, SelectionDAG &DAG) const override;
};

} // end namespace llvm

#endif
