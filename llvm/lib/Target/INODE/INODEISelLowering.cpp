//===-- INODEISelLowering.cpp - INODE DAG lowering
// implementation -----===//
//
// Part of the LLVM Project, under the Apache License
// v2.0 with LLVM Exceptions. See
// https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH
// LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the INODETargetLowering class.
//
//===----------------------------------------------------------------------===//

#include "INODEISelLowering.h"
#include "INODESubtarget.h"
#include "INODEInstrInfo.h"
#include "MCTargetDesc/INODEMCTargetDesc.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/IntrinsicsINODE.h"
#include "llvm/ADT/SmallSet.h"
using namespace llvm;

#define DEBUG_TYPE "INODE-lower"

INODETargetLowering::INODETargetLowering(const TargetMachine &TM, const INODESubtarget &STI)
    : TargetLowering(TM), Subtarget(STI) {

  addRegisterClass(MVT::i32, &INODE::SGPRRegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties(Subtarget.getRegisterInfo());

  // Set up special registers.
  // setStackPointerRegisterToSaveRestore(INODE::V31);

  // How we extend i1 boolean values.
  setBooleanContents(UndefinedBooleanContent);

  setMinFunctionAlignment(Align(4));
  setPrefFunctionAlignment(Align(4));

  setOperationAction(ISD::ADD, MVT::i32, Legal);
  setOperationAction(ISD::ADD, MVT::i16, Legal);

  setOperationAction(ISD::SELECT, MVT::i32, Custom);
  setOperationAction(ISD::SELECT_CC, MVT::i32, Custom);

  setOperationAction({ISD::INTRINSIC_WO_CHAIN, ISD::INTRINSIC_W_CHAIN, ISD::INTRINSIC_VOID},
                     MVT::Other, Custom);
}

//===----------------------------------------------------------------------===//
// Calling conventions
//===----------------------------------------------------------------------===//

#include "INODEGenCallingConv.inc"

SDValue INODETargetLowering::LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                                                  bool IsVarArg,
                                                  const SmallVectorImpl<ISD::InputArg> &Ins,
                                                  const SDLoc &DL, SelectionDAG &DAG,
                                                  SmallVectorImpl<SDValue> &InVals) const {

  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &MRI = MF.getRegInfo();

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_INODE);

  for (unsigned I = 0, E = ArgLocs.size(); I != E; ++I) {
    SDValue ArgValue;
    CCValAssign &VA = ArgLocs[I];
    EVT LocVT = VA.getLocVT();
    if (VA.isRegLoc()) {
      // Arguments passed in registers
      const TargetRegisterClass *RC;
      switch (LocVT.getSimpleVT().SimpleTy) {
      default:
        llvm_unreachable("Unexpected argument type");
      case MVT::i32:
        RC = &INODE::SGPRRegClass;
        break;
      }

      Register VReg = MRI.createVirtualRegister(RC);
      MRI.addLiveIn(VA.getLocReg(), VReg);
      ArgValue = DAG.getCopyFromReg(Chain, DL, VReg, LocVT);

      // If this is an 8/16-bit value, it is really
      // passed promoted to 32 bits. Insert an
      // assert[sz]ext to capture this, then truncate to
      // the right size.
      if (VA.getLocInfo() == CCValAssign::SExt)
        ArgValue =
            DAG.getNode(ISD::AssertSext, DL, LocVT, ArgValue, DAG.getValueType(VA.getValVT()));
      else if (VA.getLocInfo() == CCValAssign::ZExt)
        ArgValue =
            DAG.getNode(ISD::AssertZext, DL, LocVT, ArgValue, DAG.getValueType(VA.getValVT()));

      if (VA.getLocInfo() != CCValAssign::Full)
        ArgValue = DAG.getNode(ISD::TRUNCATE, DL, VA.getValVT(), ArgValue);

      InVals.push_back(ArgValue);
    } else {
      assert(VA.isMemLoc() && "Argument not register or memory");
      llvm_unreachable("INODE - LowerFormalArguments - "
                       "Memory argument not implemented");
    }
  }

  if (IsVarArg) {
    llvm_unreachable("INODE - LowerFormalArguments - "
                     "VarArgs not Implemented");
  }

  return Chain;
}

SDValue INODETargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                                         const SmallVectorImpl<ISD::OutputArg> &Outs,
                                         const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                                         SelectionDAG &DAG) const {

  // Assign locations to each returned value.
  SmallVector<CCValAssign, 16> RetLocs;
  CCState RetCCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RetLocs, *DAG.getContext());
  RetCCInfo.AnalyzeReturn(Outs, RetCC_INODE);

  SDValue Glue;
  SmallVector<SDValue, 4> RetOps(1, Chain);
  for (unsigned I = 0, E = RetLocs.size(); I != E; ++I) {
    CCValAssign &VA = RetLocs[I];

    // Make the return register live on exit.
    assert(VA.isRegLoc() && "Can only return in registers!");

    // Chain and glue the copies together.
    Register Reg = VA.getLocReg();
    Chain = DAG.getCopyToReg(Chain, DL, Reg, OutVals[I], Glue);
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(Reg, VA.getLocVT()));
  }

  // Update chain and glue.
  RetOps[0] = Chain;
  if (Glue.getNode())
    RetOps.push_back(Glue);

  return DAG.getNode(INODEISD::RET_GLUE, DL, MVT::Other, RetOps);
}

SDValue INODETargetLowering::LowerCall(CallLoweringInfo &CLI,
                                       SmallVectorImpl<SDValue> &InVals) const {
  SelectionDAG &DAG = CLI.DAG;
  DAG.dump();
  llvm_unreachable("INODE - LowerCall - Not Implemented");
  // TODO: Implement LowerCall. emit intrinsic
}

const char *INODETargetLowering::getTargetNodeName(unsigned Opcode) const {
  // TODO: ISD? maybe register ADD, SUB, etc
  switch (Opcode) {
#define OPCODE(Opc)                                                                                \
  case Opc:                                                                                        \
    return #Opc
    OPCODE(INODEISD::RET_GLUE);
    OPCODE(INODEISD::CALL);
    OPCODE(INODEISD::CLOOP_BEGIN_VALUE);
    OPCODE(INODEISD::CLOOP_BEGIN_TERMINATOR);
    OPCODE(INODEISD::CLOOP_END_VALUE);
    OPCODE(INODEISD::CLOOP_END_BRANCH);
    OPCODE(INODEISD::CLOOP_GUARD_BRANCH);
    OPCODE(INODEISD::BR_CC);
#undef OPCODE
  default:
    return nullptr;
  }
}

SDValue INODETargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  default:
    report_fatal_error("unimplemented operand");
  case ISD::INTRINSIC_WO_CHAIN:
    return LowerINTRINSIC_WO_CHAIN(Op, DAG);
  case ISD::INTRINSIC_W_CHAIN:
    return LowerINTRINSIC_W_CHAIN(Op, DAG);
  case ISD::INTRINSIC_VOID:
    return LowerINTRINSIC_VOID(Op, DAG);
  case ISD::SELECT_CC: {
    // This occurs because we custom legalize SETGT and SETUGT for setcc. That
    // causes LegalizeDAG to think we need to custom legalize select_cc. Expand
    // into separate SETCC+SELECT just like LegalizeDAG.
    SDValue Tmp1 = Op.getOperand(0);
    SDValue Tmp2 = Op.getOperand(1);
    SDValue True = Op.getOperand(2);
    SDValue False = Op.getOperand(3);
    EVT VT = Op.getValueType();
    SDValue CC = Op.getOperand(4);
    EVT CmpVT = Tmp1.getValueType();
    EVT CCVT = getSetCCResultType(DAG.getDataLayout(), *DAG.getContext(), CmpVT);
    SDLoc DL(Op);
    SDValue Cond = DAG.getNode(ISD::SETCC, DL, CCVT, Tmp1, Tmp2, CC, Op->getFlags());
    return DAG.getSelect(DL, VT, Cond, True, False);
  }
  case ISD::SELECT: {
    return lowerSELECT(Op, DAG);
  }
  }
}

SDValue INODETargetLowering::LowerINTRINSIC_WO_CHAIN(SDValue Op, SelectionDAG &DAG) const {
  return SDValue();
}

SDValue INODETargetLowering::LowerINTRINSIC_VOID(SDValue Op, SelectionDAG &DAG) const {
  return SDValue();
}

SDValue INODETargetLowering::LowerINTRINSIC_W_CHAIN(SDValue Op, SelectionDAG &DAG) const {

  auto isValidCloopMetadata = [&](SDValue x) {
    auto metadata = dyn_cast<ConstantSDNode>(x);
    return metadata && (metadata->getZExtValue() <= UINT16_MAX);
  };

  SDLoc dl(Op);

  unsigned int IntNo = Op.getConstantOperandVal(1);
  switch (IntNo) {
  default:
    break;
  case Intrinsic::INODE_cloop_begin: {
    // reg = cloop_begin(count)
    // OP : chain(0), ID(1), count(2)
    assert(Op->getNumOperands() == 3);
    auto onFailure = [&]() {
      // Fall back is to delete the intrinsic in situ
      LLVM_DEBUG(dbgs() << "Replacing cloop begin intrinsic with fallback: "; Op.dump(););
      assert(Op.getOpcode() == ISD::INTRINSIC_W_CHAIN);
      DAG.ReplaceAllUsesOfValueWith(Op, Op.getOperand(2));                       // count
      DAG.ReplaceAllUsesOfValueWith(SDValue(Op.getNode(), 1), Op.getOperand(0)); // chain
      return SDValue();
    };

    // i16 (*INODE.cloop.begin)(i16, i16)
    // This is lowered into two ISD nodes. One is a terminator, the other
    // represents the potential need to keep an induction variable around.
    // Should be able to write a more robust version of this using DAG.getRoot
    // The second argument is metadata, passed along to the back end
    // The induction variable node is a like for like replacement
    LLVM_DEBUG(dbgs() << "Lowering INODE.cloop.begin: "; Op.dump(); DAG.dump(););

    // First replace the intrinsic call with a node that represents the
    // induction variable, then insert a terminator as near to the
    // root of the DAG as possible
    SDValue root = DAG.getRoot();
    if (root->getOpcode() == ISD::BR) {
      SDValue brOp = root->getOperand(0);
      if (brOp.getOpcode() == ISD::BRCOND) {
        // hardware loop header should only be inserted in an unconditional BB
        return onFailure();
      }
    } else if (root->getOpcode() == ISD::TokenFactor) {
      // TokenFactor is OK
    } else {
      return onFailure();
    }

    // if (!isValidCloopMetadata(Op.getOperand(3))) {
    //   return onFailure();
    // }

    // Replace the intrinsic with an ISD node. This will update the CopyToReg
    // that copies the induction variable out of the basic block
    SDValue originalChain = Op.getOperand(0);
    assert(originalChain.getValueType() == MVT::Other);

    SDVTList VTs = DAG.getVTList(MVT::i32, MVT::Other);
    // SDValue beginValue = DAG.getNode(INODEISD::CLOOP_BEGIN_VALUE, dl, VTs, originalChain,
    //                                  Op.getOperand(2), Op.getOperand(3));
    int count_val = cast<ConstantSDNode>(Op.getOperand(2))->getZExtValue();
    MVT count_val_type = Op.getOperand(2).getValueType().getSimpleVT();
    // SDValue beginValue = DAG.getNode(INODEISD::CLOOP_BEGIN_VALUE, dl, VTs, originalChain,
    //                                  DAG.getConstant(count_val, dl, count_val_type, true),
    //                                  Op.getOperand(3));
    SDValue beginValue = DAG.getNode(INODEISD::CLOOP_BEGIN_VALUE, dl, VTs, originalChain,
                                     DAG.getConstant(count_val, dl, count_val_type, true));

    if (root.getOpcode() == ISD::TokenFactor) {
      // SDValue begin_terminator = DAG.getNode(INODEISD::CLOOP_BEGIN_TERMINATOR, dl, MVT::Other,
      // root,
      //                                        beginValue, Op.getOperand(3));
      SDValue begin_terminator =
          DAG.getNode(INODEISD::CLOOP_BEGIN_TERMINATOR, dl, MVT::Other, root, beginValue);
      DAG.setRoot(begin_terminator);
    } else {
      assert(root.getOpcode() == ISD::BR);
      // SDValue begin_terminator = DAG.getNode(INODEISD::CLOOP_BEGIN_TERMINATOR, dl, MVT::Other,
      //                                        root.getOperand(0), beginValue, Op.getOperand(3));
      SDValue begin_terminator = DAG.getNode(INODEISD::CLOOP_BEGIN_TERMINATOR, dl, MVT::Other,
                                             root.getOperand(0), beginValue);
      DAG.ReplaceAllUsesWith(
          root, DAG.getNode(ISD::BR, dl, MVT::Other, begin_terminator, root.getOperand(1)));
    }
    return beginValue;
  }
  case Intrinsic::INODE_cloop_end: {
    // val,meet = cloop_end(indvar)
    // OP : chain(0), ID(1), indvar(2)
    // Expecting an IR sequence:
    // %cloop.end = call i32 @llvm.INODE.cloop.end(i32 %cloop.phi, i32 %meta)
    // %cloop.end.iv = extractvalue {i32, i32} %cloop.end, 0
    // %cloop.end.cc = extractvalue {i32, i32} %cloop.end, 1
    // %cloop.end.cc.trunc = trunc i32 %cloop.end to i1
    // br i1 %cloop.end.cc.trunc, label %t, label %f
    assert(Op->getNumOperands() == 3);
    auto onFailure = [&]() {
      assert(Op.getOpcode() == ISD::INTRINSIC_W_CHAIN);
      LLVM_DEBUG(dbgs() << "Replacing cloop end intrinsic with fallback: "; Op.dump(););

      SDValue decr =
          DAG.getNode(ISD::SUB, dl, MVT::i32, Op.getOperand(2), DAG.getConstant(1, dl, MVT::i32));

      // Replace the uses of the induction variable with the decrement
      DAG.ReplaceAllUsesOfValueWith(SDValue(Op.getNode(), 0), decr);

      // Replace the uses of condition with a new setcc
      SDValue nz = DAG.getSetCC(dl, MVT::i32, decr, DAG.getConstant(0, dl, MVT::i32), ISD::SETNE);
      DAG.ReplaceAllUsesOfValueWith(SDValue(Op.getNode(), 1), nz);

      // Remove the intrinsic from its chain
      DAG.ReplaceAllUsesOfValueWith(SDValue(Op.getNode(), 2), Op.getOperand(0));
      return SDValue();
    };

    auto isCountedLoopEnd = [&](SDValue x) {
      if (x.getOpcode() == ISD::INTRINSIC_W_CHAIN) {
        unsigned IntNo = cast<ConstantSDNode>(x.getOperand(1))->getZExtValue();
        return IntNo == Intrinsic::INODE_cloop_end;
      }
      return false;
    };

    LLVM_DEBUG(dbgs() << "Lowering INODE.cloop.end: "; Op.dump(); DAG.dump(););

    // Require this instruction to be in a conditional block. Find the brcond.
    SDValue brcond = DAG.getRoot();
    if (brcond.getOpcode() == ISD::BR) {
      brcond = brcond.getOperand(0);
    }
    if (brcond.getOpcode() != ISD::BRCOND) {
      return onFailure();
    }

    // if (!isValidCloopMetadata(Op.getOperand(3))) {
    //   return onFailure();
    // }

    // The brcond condition is likely to be an and with 1 from legalisation.
    // If so we want to reach through it.
    SDValue brcondValue = brcond.getOperand(1);
    if (brcondValue.getOpcode() == ISD::AND) {
      if (brcondValue.getOperand(1) == DAG.getConstant(1, dl, MVT::i32)) {
        brcondValue = brcondValue.getOperand(0);
      }
    }

    if (!isCountedLoopEnd(brcondValue)) {
      return onFailure();
    }

    // Established that we have a conditional branch on the return value
    // of the counted loop end intrinsic. This is the desired pattern.

    // The intrinsic returns i32 (indvar), i32 (cc), ch
    // Replace all uses of this by a node that represents the decrement of
    // the loop counter.
    SDVTList VTs = DAG.getVTList(MVT::i32, MVT::Other);
    SDNode *endValue =
        DAG.getNode(INODEISD::CLOOP_END_VALUE, dl, VTs, Op.getOperand(0), Op.getOperand(2))
            .getNode();
    // SDNode *endValue = DAG.getNode(INODEISD::CLOOP_END_VALUE, dl, VTs, Op.getOperand(0),
    //                                Op.getOperand(2), Op.getOperand(3))
    //                        .getNode();

    // Replace indvar and cc with the integer returned by CLOOP_END_VALUE
    for (unsigned i = 0; i < 2; i++) {
      DAG.ReplaceAllUsesOfValueWith(SDValue(Op.getNode(), i), SDValue(endValue, 0));
    }

    // Replace chain
    DAG.ReplaceAllUsesOfValueWith(SDValue(Op.getNode(), 2), SDValue(endValue, 1));

    // Replace the conditional branch with a specialised version that also
    // takes the integer returned by CLOOP_END_VALUE
    SDValue endBranch =
        // DAG.getNode(INODEISD::CLOOP_END_BRANCH, SDLoc(brcond), MVT::Other, brcond.getOperand(0),
        //             brcond.getOperand(2), SDValue(endValue, 0), Op.getOperand(3));
        DAG.getNode(INODEISD::CLOOP_END_BRANCH, SDLoc(brcond), MVT::Other, brcond.getOperand(0),
                    brcond.getOperand(2), SDValue(endValue, 0));

    // Replace the brcond with a specialised conditional branch
    DAG.ReplaceAllUsesWith(brcond, endBranch);
    return SDValue();
  }
  }

  return SDValue();
}

void INODETargetLowering::ReplaceNodeResults(SDNode *N, SmallVectorImpl<SDValue> &Results,
                                             SelectionDAG &DAG) const {
  SDLoc DL(N);
  switch (N->getOpcode()) {
  default:
    llvm_unreachable("Don't know how to custom type legalize this operation!");
  case ISD::Constant:
    Results.push_back(DAG.getConstant(N->getConstantOperandVal(0), DL, MVT::i32));
    break;
  case ISD::INTRINSIC_W_CHAIN: {
    unsigned int IntNo = N->getConstantOperandVal(1);
    switch (IntNo) {
    default:
      llvm_unreachable("Don't know how to custom type legalize this intrinsic!");
    case Intrinsic::INODE_cloop_begin: {
      // Results.push_back(DAG.getNode(ISD::TRUNCATE, DL, MVT::i32, SDValue(N, 0)));
      // Results.push_back(SDValue(N, 1));
      break;
    }
    case Intrinsic::INODE_cloop_end: {
      // Results.push_back(DAG.getNode(ISD::TRUNCATE, DL, MVT::i32, SDValue(N, 0)));
      // Results.push_back(SDValue(N, 1));
      break;
    }
    }
  }
  }
}

// Changes the condition code and swaps operands if necessary, so the SetCC
// operation matches one of the comparisons supported directly by branches
// in the RISC-V ISA. May adjust compares to favor compare with 0 over compare
// with 1/-1.
static void translateSetCCForBranch(const SDLoc &DL, SDValue &LHS, SDValue &RHS, ISD::CondCode &CC,
                                    SelectionDAG &DAG) {
  // If this is a single bit test that can't be handled by ANDI, shift the
  // bit to be tested to the MSB and perform a signed compare with 0.
  if (isIntEqualitySetCC(CC) && isNullConstant(RHS) && LHS.getOpcode() == ISD::AND &&
      LHS.hasOneUse() && isa<ConstantSDNode>(LHS.getOperand(1))) {
    uint64_t Mask = LHS.getConstantOperandVal(1);
    if ((isPowerOf2_64(Mask) || isMask_64(Mask)) && !isInt<12>(Mask)) {
      unsigned ShAmt = 0;
      if (isPowerOf2_64(Mask)) {
        CC = CC == ISD::SETEQ ? ISD::SETGE : ISD::SETLT;
        ShAmt = LHS.getValueSizeInBits() - 1 - Log2_64(Mask);
      } else {
        ShAmt = LHS.getValueSizeInBits() - llvm::bit_width(Mask);
      }

      LHS = LHS.getOperand(0);
      if (ShAmt != 0)
        LHS = DAG.getNode(ISD::SHL, DL, LHS.getValueType(), LHS,
                          DAG.getConstant(ShAmt, DL, LHS.getValueType()));
      return;
    }
  }

  if (auto *RHSC = dyn_cast<ConstantSDNode>(RHS)) {
    int64_t C = RHSC->getSExtValue();
    switch (CC) {
    default:
      break;
    case ISD::SETGT:
      // Convert X > -1 to X >= 0.
      if (C == -1) {
        RHS = DAG.getConstant(0, DL, RHS.getValueType());
        CC = ISD::SETGE;
        return;
      }
      break;
    case ISD::SETLT:
      // Convert X < 1 to 0 >= X.
      if (C == 1) {
        RHS = LHS;
        LHS = DAG.getConstant(0, DL, RHS.getValueType());
        CC = ISD::SETGE;
        return;
      }
      break;
    }
  }

  switch (CC) {
  default:
    break;
  case ISD::SETGT:
  case ISD::SETLE:
  case ISD::SETUGT:
  case ISD::SETULE:
    CC = ISD::getSetCCSwappedOperands(CC);
    std::swap(LHS, RHS);
    break;
  }
}

SDValue INODETargetLowering::lowerSELECT(SDValue Op, SelectionDAG &DAG) const {
  SDValue CondV = Op.getOperand(0);
  SDValue TrueV = Op.getOperand(1);
  SDValue FalseV = Op.getOperand(2);
  SDLoc DL(Op);
  MVT VT = Op.getSimpleValueType();
  MVT XLenVT = MVT::i32;

  // If the CondV is the output of a SETCC node which operates on XLenVT inputs,
  // then merge the SETCC node into the lowered INODEISD::SELECT_CC to take
  // advantage of the integer compare+branch instructions. i.e.:
  // (select (setcc lhs, rhs, cc), truev, falsev)
  // -> (riscvisd::select_cc lhs, rhs, cc, truev, falsev)
  SDValue LHS = CondV.getOperand(0);
  SDValue RHS = CondV.getOperand(1);
  ISD::CondCode CCVal = cast<CondCodeSDNode>(CondV.getOperand(2))->get();

  translateSetCCForBranch(DL, LHS, RHS, CCVal, DAG);
  // 1 < x ? x : 1 -> 0 < x ? x : 1
  if (isOneConstant(LHS) && (CCVal == ISD::SETLT || CCVal == ISD::SETULT) && RHS == TrueV &&
      LHS == FalseV) {
    LHS = DAG.getConstant(0, DL, VT);
    // 0 <u x is the same as x != 0.
    if (CCVal == ISD::SETULT) {
      std::swap(LHS, RHS);
      CCVal = ISD::SETNE;
    }
  }

  // x <s -1 ? x : -1 -> x <s 0 ? x : -1
  if (isAllOnesConstant(RHS) && CCVal == ISD::SETLT && LHS == TrueV && RHS == FalseV) {
    RHS = DAG.getConstant(0, DL, VT);
  }

  SDValue TargetCC = DAG.getCondCode(CCVal);

  if (isa<ConstantSDNode>(TrueV) && !isa<ConstantSDNode>(FalseV)) {
    // (select (setcc lhs, rhs, CC), constant, falsev)
    // -> (select (setcc lhs, rhs, InverseCC), falsev, constant)
    std::swap(TrueV, FalseV);
    TargetCC = DAG.getCondCode(ISD::getSetCCInverse(CCVal, LHS.getValueType()));
  }

  SDValue Ops[] = {LHS, RHS, TargetCC, TrueV, FalseV};
  return DAG.getNode(INODEISD::SELECT_CC, DL, VT, Ops);
}

static bool isSelectPseudo(MachineInstr &MI) {
  switch (MI.getOpcode()) {
  default:
    return false;
  case INODE::Select_GPR_Using_CC_GPR:
    return true;
  }
}

static MachineBasicBlock *emitSelectPseudo(MachineInstr &MI,
                                           MachineBasicBlock *BB,
                                           const INODESubtarget &Subtarget) {

  // out = select_cc %LHS, %RHS, %CC, %TrueValue, %FalseValue
  auto Next = next_nodbg(MI.getIterator(), BB->instr_end());
  Register LHS = MI.getOperand(1).getReg();
  Register RHS;
  if (MI.getOperand(2).isReg())
    RHS = MI.getOperand(2).getReg();
  auto CC = static_cast<INODECC::CondCode>(MI.getOperand(3).getImm());

  SmallVector<MachineInstr *, 4> SelectDebugValues;
  SmallSet<Register, 4> SelectDests;
  SelectDests.insert(MI.getOperand(0).getReg());

  MachineInstr *LastSelectPseudo = &MI;
  for (auto E = BB->end(), SequenceMBBI = MachineBasicBlock::iterator(MI);
       SequenceMBBI != E; ++SequenceMBBI) {
    if (SequenceMBBI->isDebugInstr())
      continue;
    if (isSelectPseudo(*SequenceMBBI)) {
      if (SequenceMBBI->getOperand(1).getReg() != LHS ||
          !SequenceMBBI->getOperand(2).isReg() ||
          SequenceMBBI->getOperand(2).getReg() != RHS ||
          SequenceMBBI->getOperand(3).getImm() != CC ||
          SelectDests.count(SequenceMBBI->getOperand(4).getReg()) ||
          SelectDests.count(SequenceMBBI->getOperand(5).getReg()))
        break;
      LastSelectPseudo = &*SequenceMBBI;
      SequenceMBBI->collectDebugValues(SelectDebugValues);
      SelectDests.insert(SequenceMBBI->getOperand(0).getReg());
      continue;
    }
    if (SequenceMBBI->hasUnmodeledSideEffects() ||
        SequenceMBBI->mayLoadOrStore() ||
        SequenceMBBI->usesCustomInsertionHook())
      break;
    if (llvm::any_of(SequenceMBBI->operands(), [&](MachineOperand &MO) {
          return MO.isReg() && MO.isUse() && SelectDests.count(MO.getReg());
        }))
      break;
  }

  const INODEInstrInfo &TII = *Subtarget.getInstrInfo();
  const BasicBlock *LLVM_BB = BB->getBasicBlock();
  DebugLoc DL = MI.getDebugLoc();
  MachineFunction::iterator I = ++BB->getIterator();

  MachineBasicBlock *HeadMBB = BB;
  MachineFunction *F = BB->getParent();
  MachineBasicBlock *TailMBB = F->CreateMachineBasicBlock(LLVM_BB);
  MachineBasicBlock *IfFalseMBB = F->CreateMachineBasicBlock(LLVM_BB);

  F->insert(I, IfFalseMBB);
  F->insert(I, TailMBB);

  // Set the call frame size on entry to the new basic blocks.
  unsigned CallFrameSize = TII.getCallFrameSizeAt(*LastSelectPseudo);
  IfFalseMBB->setCallFrameSize(CallFrameSize);
  TailMBB->setCallFrameSize(CallFrameSize);

  // Transfer debug instructions associated with the selects to TailMBB.
  for (MachineInstr *DebugInstr : SelectDebugValues) {
    TailMBB->push_back(DebugInstr->removeFromParent());
  }

  // Move all instructions after the sequence to TailMBB.
  TailMBB->splice(TailMBB->end(), HeadMBB,
                  std::next(LastSelectPseudo->getIterator()), HeadMBB->end());
  // Update machine-CFG edges by transferring all successors of the current
  // block to the new block which will contain the Phi nodes for the selects.
  TailMBB->transferSuccessorsAndUpdatePHIs(HeadMBB);
  // Set the successors for HeadMBB.
  HeadMBB->addSuccessor(IfFalseMBB);
  HeadMBB->addSuccessor(TailMBB);

  // Insert appropriate branch.
  BuildMI(HeadMBB, DL, TII.getBrCond(CC))
      .addReg(LHS)
      .addReg(RHS)
      .addMBB(TailMBB);

  // IfFalseMBB just falls through to TailMBB.
  IfFalseMBB->addSuccessor(TailMBB);

  // Create PHIs for all of the select pseudo-instructions.
  auto SelectMBBI = MI.getIterator();
  auto SelectEnd = std::next(LastSelectPseudo->getIterator());
  auto InsertionPoint = TailMBB->begin();
  while (SelectMBBI != SelectEnd) {
    auto Next = std::next(SelectMBBI);
    if (isSelectPseudo(*SelectMBBI)) {
      // %Result = phi [ %TrueValue, HeadMBB ], [ %FalseValue, IfFalseMBB ]
      BuildMI(*TailMBB, InsertionPoint, SelectMBBI->getDebugLoc(),
              TII.get(INODE::PHI), SelectMBBI->getOperand(0).getReg())
          .addReg(SelectMBBI->getOperand(4).getReg())
          .addMBB(HeadMBB)
          .addReg(SelectMBBI->getOperand(5).getReg())
          .addMBB(IfFalseMBB);
      SelectMBBI->eraseFromParent();
    }
    SelectMBBI = Next;
  }

  F->getProperties().reset(MachineFunctionProperties::Property::NoPHIs);
  return TailMBB;
}

MachineBasicBlock *INODETargetLowering::EmitInstrWithCustomInserter(MachineInstr &MI,
                                                                    MachineBasicBlock *BB) const {
  switch (MI.getOpcode()) {
  default:
    llvm_unreachable("Unexpected instr type to insert");
  case INODE::Select_GPR_Using_CC_GPR:
    return emitSelectPseudo(MI, BB, Subtarget);
  }
}