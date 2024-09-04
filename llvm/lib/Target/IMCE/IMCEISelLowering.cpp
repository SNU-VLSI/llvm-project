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
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include <cstdint>

using namespace llvm;

#define DEBUG_TYPE "IMCE-lower"

IMCETargetLowering::IMCETargetLowering(const TargetMachine &TM, const IMCESubtarget &STI)
    : TargetLowering(TM), Subtarget(STI) {

  addRegisterClass(MVT::v16i16, &IMCE::VGPRRegClass);
  addRegisterClass(MVT::i32, &IMCE::SGPRRegClass);

  // Compute derived properties from the register
  // classes
  computeRegisterProperties(Subtarget.getRegisterInfo());

  // Set up special registers.
  setStackPointerRegisterToSaveRestore(IMCE::S31);

  // How we extend i1 boolean values.
  setBooleanContents(ZeroOrOneBooleanContent);

  setMinFunctionAlignment(Align(4));
  setPrefFunctionAlignment(Align(4));

  setOperationAction(ISD::ADD, MVT::v16i16, Legal);
  setOperationAction(ISD::AND, MVT::v16i16, Legal);
  setOperationAction(ISD::OR, MVT::v16i16, Legal);
  setOperationAction(ISD::XOR, MVT::v16i16, Legal);

  setOperationAction(ISD::ADD, MVT::i32, Legal);
  setOperationAction(ISD::AND, MVT::i32, Legal);
  setOperationAction(ISD::OR, MVT::i32, Legal);
  setOperationAction(ISD::XOR, MVT::i32, Legal);
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
        // Integers smaller than i64 should be promoted
        // to i32.
        llvm_unreachable("Unexpected argument type");
      case MVT::v16i16:
        RC = &IMCE::VGPRRegClass;
        break;

      case MVT::i32:
        RC = &IMCE::SGPRRegClass;
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
    OPCODE(IMCEISD::CLR);
    OPCODE(IMCEISD::SET);
    OPCODE(IMCEISD::EXT);
    OPCODE(IMCEISD::EXTU);
    OPCODE(IMCEISD::MAK);
    OPCODE(IMCEISD::ROT);
    OPCODE(IMCEISD::FF1);
    OPCODE(IMCEISD::FF0);
#undef OPCODE
  default:
    return nullptr;
  }
}
