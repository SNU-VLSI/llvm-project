//===-- IMCEISelLowering.cpp - IMCE DAG lowering
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
// This file implements the IMCETargetLowering class.
//
//===----------------------------------------------------------------------===//

#include "IMCEISelLowering.h"
#include "IMCESubtarget.h"
#include "MCTargetDesc/IMCEMCTargetDesc.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/IntrinsicsIMCE.h"
#include <cstdint>

using namespace llvm;

#define DEBUG_TYPE "IMCE-lower"

IMCETargetLowering::IMCETargetLowering(const TargetMachine &TM, const IMCESubtarget &STI)
    : TargetLowering(TM), Subtarget(STI) {

  addRegisterClass(MVT::i16, &IMCE::VGPRRegClass);
  // addRegisterClass(MVT::i32, &IMCE::VGPRRegClass);
  addRegisterClass(MVT::i32, &IMCE::HWLRRegClass);
  addRegisterClass(MVT::v16i16, &IMCE::VGPRRegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties(Subtarget.getRegisterInfo());

  // Set up special registers.
  // setStackPointerRegisterToSaveRestore(IMCE::V31);

  // How we extend i1 boolean values.
  setBooleanContents(UndefinedBooleanContent);

  setMinFunctionAlignment(Align(4));
  setPrefFunctionAlignment(Align(4));

  setOperationAction(ISD::ADD, {MVT::v16i16, MVT::i16}, Legal);

  // setOperationAction({ISD::INTRINSIC_WO_CHAIN, ISD::INTRINSIC_W_CHAIN, ISD::INTRINSIC_VOID},
  //                    {MVT::v16i16, MVT::i32}, Custom);
  setOperationAction({ISD::INTRINSIC_WO_CHAIN, ISD::INTRINSIC_W_CHAIN, ISD::INTRINSIC_VOID},
                     {MVT::Other, MVT::v16i16, MVT::i16, MVT::i32}, Custom);

  //TODO : ISD::Constant for i32
}

//===----------------------------------------------------------------------===//
// Calling conventions
//===----------------------------------------------------------------------===//

#include "IMCEGenCallingConv.inc"

SDValue IMCETargetLowering::LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                                                 bool IsVarArg,
                                                 const SmallVectorImpl<ISD::InputArg> &Ins,
                                                 const SDLoc &DL, SelectionDAG &DAG,
                                                 SmallVectorImpl<SDValue> &InVals) const {

  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &MRI = MF.getRegInfo();

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_IMCE);

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
      case MVT::v16i16:
        RC = &IMCE::VGPRRegClass;
        break;
      case MVT::i16:
        RC = &IMCE::VGPRRegClass;
        break;
      case MVT::i32:
        RC = &IMCE::HWLRRegClass;
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
      llvm_unreachable("IMCE - LowerFormalArguments - "
                       "Memory argument not implemented");
    }
  }

  if (IsVarArg) {
    llvm_unreachable("IMCE - LowerFormalArguments - "
                     "VarArgs not Implemented");
  }

  return Chain;
}

SDValue IMCETargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                                        const SmallVectorImpl<ISD::OutputArg> &Outs,
                                        const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                                        SelectionDAG &DAG) const {

  // Assign locations to each returned value.
  SmallVector<CCValAssign, 16> RetLocs;
  CCState RetCCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RetLocs, *DAG.getContext());
  RetCCInfo.AnalyzeReturn(Outs, RetCC_IMCE);

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

  return DAG.getNode(IMCEISD::RET_GLUE, DL, MVT::Other, RetOps);
}

SDValue IMCETargetLowering::LowerCall(CallLoweringInfo &CLI,
                                      SmallVectorImpl<SDValue> &InVals) const {
  SelectionDAG &DAG = CLI.DAG;
  DAG.dump();
  llvm_unreachable("IMCE - LowerCall - Not Implemented");
  // TODO: Implement LowerCall. emit intrinsic
}

const char *IMCETargetLowering::getTargetNodeName(unsigned Opcode) const {
  // TODO: ISD? maybe register ADD, SUB, etc
  switch (Opcode) {
#define OPCODE(Opc)                                                                                \
  case Opc:                                                                                        \
    return #Opc
    OPCODE(IMCEISD::RET_GLUE);
    OPCODE(IMCEISD::CALL);
    OPCODE(IMCEISD::CLOOP_BEGIN_VALUE);
    OPCODE(IMCEISD::CLOOP_BEGIN_TERMINATOR);
    OPCODE(IMCEISD::CLOOP_END_VALUE);
    OPCODE(IMCEISD::CLOOP_END_BRANCH);
    OPCODE(IMCEISD::CLOOP_GUARD_BRANCH);
#undef OPCODE
  default:
    return nullptr;
  }
}

SDValue IMCETargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  default:
    report_fatal_error("unimplemented operand");
  case ISD::INTRINSIC_WO_CHAIN:
    return LowerINTRINSIC_WO_CHAIN(Op, DAG);
  case ISD::INTRINSIC_W_CHAIN:
    return LowerINTRINSIC_W_CHAIN(Op, DAG);
  case ISD::INTRINSIC_VOID:
    return LowerINTRINSIC_VOID(Op, DAG);
  }
}

SDValue IMCETargetLowering::LowerINTRINSIC_WO_CHAIN(SDValue Op, SelectionDAG &DAG) const {
  unsigned IntNo = cast<ConstantSDNode>(Op.getOperand(0))->getZExtValue();
  switch (IntNo) {
  default:
    break;
  case Intrinsic::IMCE_GET_QREG: {
    SDLoc dl(Op);
    SDValue Idx = Op.getOperand(1);

    Register QReg;
    switch (cast<ConstantSDNode>(Idx)->getZExtValue()) {
        case 0: QReg = IMCE::QREG0; break;
        case 1: QReg = IMCE::QREG1; break;
        case 2: QReg = IMCE::QREG2; break;
        case 3: QReg = IMCE::QREG3; break;
        default:
            llvm_unreachable("Invalid QREG index.");
    }

    // Use the getRegister node to get QReg register
    return DAG.getRegister(QReg, MVT::v16i16);
  }
  case Intrinsic::IMCE_GET_CREG: {
    SDLoc dl(Op);
    SDValue Idx = Op.getOperand(1);

    Register CReg;
    switch (cast<ConstantSDNode>(Idx)->getZExtValue()) {
        case 0: CReg = IMCE::CREG0; break;
        case 1: CReg = IMCE::CREG1; break;
        case 2: CReg = IMCE::CREG2; break;
        case 3: CReg = IMCE::CREG3; break;
        default:
            llvm_unreachable("Invalid QREG index.");
    }

    // Use the getRegister node to get QReg register
    return DAG.getRegister(CReg, MVT::v16i16);
  }
  };
  return SDValue();
}

SDValue IMCETargetLowering::LowerINTRINSIC_VOID(SDValue Op, SelectionDAG &DAG) const {
  return SDValue();
}

SDValue IMCETargetLowering::LowerINTRINSIC_W_CHAIN(SDValue Op, SelectionDAG &DAG) const {

  auto isValidCloopMetadata = [&](SDValue x) {
    auto metadata = dyn_cast<ConstantSDNode>(x);
    return metadata && (metadata->getZExtValue() <= UINT16_MAX);
  };

  SDLoc dl(Op);

  unsigned int IntNo = Op.getConstantOperandVal(1);
  switch (IntNo) {
  default:
    break;
  case Intrinsic::IMCE_cloop_begin: {
    assert(Op->getNumOperands() == 4);
    auto onFailure = [&]() {
      // Fall back is to delete the intrinsic in situ
      LLVM_DEBUG(dbgs() << "Replacing cloop begin intrinsic with fallback: "; Op.dump(););
      assert(Op.getOpcode() == ISD::INTRINSIC_W_CHAIN);
      DAG.ReplaceAllUsesOfValueWith(Op, Op.getOperand(2));
      DAG.ReplaceAllUsesOfValueWith(SDValue(Op.getNode(), 1), Op.getOperand(0));
      return SDValue();
    };

    // i16 (*imce.cloop.begin)(i16, i16)
    // This is lowered into two ISD nodes. One is a terminator, the other
    // represents the potential need to keep an induction variable around.
    // Should be able to write a more robust version of this using DAG.getRoot
    // The second argument is metadata, passed along to the back end
    // The induction variable node is a like for like replacement
    LLVM_DEBUG(dbgs() << "Lowering imce.cloop.begin: "; Op.dump(); DAG.dump(););

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

    if (!isValidCloopMetadata(Op.getOperand(3))) {
      return onFailure();
    }

    // Replace the intrinsic with an ISD node. This will update the CopyToReg
    // that copies the induction variable out of the basic block
    SDValue originalChain = Op.getOperand(0);
    assert(originalChain.getValueType() == MVT::Other);

    SDVTList VTs = DAG.getVTList(MVT::i32, MVT::Other);
    // SDValue beginValue = DAG.getNode(IMCEISD::CLOOP_BEGIN_VALUE, dl, VTs, originalChain,
    //                                  Op.getOperand(2), Op.getOperand(3));
    int count_val = cast<ConstantSDNode>(Op.getOperand(2))->getZExtValue();
    MVT count_val_type = Op.getOperand(2).getValueType().getSimpleVT();
    SDValue beginValue = DAG.getNode(IMCEISD::CLOOP_BEGIN_VALUE, dl, VTs, originalChain,
                                     DAG.getConstant(count_val, dl, count_val_type, true), Op.getOperand(3));

    if (root.getOpcode() == ISD::TokenFactor) {
      SDValue begin_terminator = DAG.getNode(IMCEISD::CLOOP_BEGIN_TERMINATOR, dl, MVT::Other, root,
                                             beginValue, Op.getOperand(3));
      DAG.setRoot(begin_terminator);
    } else {
      assert(root.getOpcode() == ISD::BR);
      SDValue begin_terminator = DAG.getNode(IMCEISD::CLOOP_BEGIN_TERMINATOR, dl, MVT::Other,
                                             root.getOperand(0), beginValue, Op.getOperand(3));
      DAG.ReplaceAllUsesWith(
          root, DAG.getNode(ISD::BR, dl, MVT::Other, begin_terminator, root.getOperand(1)));
    }
    return beginValue;
  }
  case Intrinsic::IMCE_cloop_end: {
    // Expecting an IR sequence:
    // %cloop.end = call i32 @llvm.IMCE.cloop.end(i32 %cloop.phi, i32 %meta)
    // %cloop.end.iv = extractvalue {i32, i32} %cloop.end, 0
    // %cloop.end.cc = extractvalue {i32, i32} %cloop.end, 1
    // %cloop.end.cc.trunc = trunc i32 %cloop.end to i1
    // br i1 %cloop.end.cc.trunc, label %t, label %f
    assert(Op->getNumOperands() == 4);
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
        return IntNo == Intrinsic::IMCE_cloop_end;
      }
      return false;
    };

    LLVM_DEBUG(dbgs() << "Lowering IMCE.cloop.end: "; Op.dump(); DAG.dump(););

    // Require this instruction to be in a conditional block. Find the brcond.
    SDValue brcond = DAG.getRoot();
    if (brcond.getOpcode() == ISD::BR) {
      brcond = brcond.getOperand(0);
    }
    if (brcond.getOpcode() != ISD::BRCOND) {
      return onFailure();
    }

    if (!isValidCloopMetadata(Op.getOperand(3))) {
      return onFailure();
    }

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
    SDNode *endValue = DAG.getNode(IMCEISD::CLOOP_END_VALUE, dl, VTs, Op.getOperand(0),
                                   Op.getOperand(2), Op.getOperand(3))
                           .getNode();

    // Replace indvar and cc with the integer returned by CLOOP_END_VALUE
    for (unsigned i = 0; i < 2; i++) {
      DAG.ReplaceAllUsesOfValueWith(SDValue(Op.getNode(), i), SDValue(endValue, 0));
    }

    // Replace chain
    DAG.ReplaceAllUsesOfValueWith(SDValue(Op.getNode(), 2), SDValue(endValue, 1));

    // Replace the conditional branch with a specialised version that also
    // takes the integer returned by CLOOP_END_VALUE
    SDValue endBranch =
        DAG.getNode(IMCEISD::CLOOP_END_BRANCH, SDLoc(brcond), MVT::Other, brcond.getOperand(0),
                    brcond.getOperand(2), SDValue(endValue, 0), Op.getOperand(3));

    // Replace the brcond with a specialised conditional branch
    DAG.ReplaceAllUsesWith(brcond, endBranch);
    return SDValue();
  }
  }

  return SDValue();
}

void IMCETargetLowering::ReplaceNodeResults(SDNode *N, SmallVectorImpl<SDValue> &Results,
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
    case Intrinsic::IMCE_cloop_begin: {
      // Results.push_back(DAG.getNode(ISD::TRUNCATE, DL, MVT::i32, SDValue(N, 0)));
      // Results.push_back(SDValue(N, 1));
      break;
    }
    case Intrinsic::IMCE_cloop_end: {
      // Results.push_back(DAG.getNode(ISD::TRUNCATE, DL, MVT::i32, SDValue(N, 0)));
      // Results.push_back(SDValue(N, 1));
      break;
    }
    }
  }
  }
}