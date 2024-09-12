//===-- IMCEInstrInfo.td - IMCE Instructions ---------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the IMCE instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

// ---------------------------------------------------------------------------//
// Selection DAG Nodes.
// ---------------------------------------------------------------------------//

// Selection DAG types.

// These are target-independent nodes, but have target-specific formats.
def SDT_CallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def SDT_CallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def SDT_Call         : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;

// Selection DAG nodes.
def call             : SDNode<"IMCEISD::CALL", SDT_Call,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                              SDNPVariadic]>;
def retglue          : SDNode<"IMCEISD::RET_GLUE", SDTNone,
                              [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

// operands
def JumpTargetAsmOperand : AsmOperandClass {
  let Name = "JumpTarget";
  // let ParserMethod = "parseJumpTargetOperands";
  let PredicateMethod = "isJumpTarget";
  let RenderMethod = "addJumpTargetOperands";
}

def jumpOperand : Operand<OtherVT> {
  // let EncoderMethod = "encodeJumpOperand";
  let ParserMatchClass = JumpTargetAsmOperand;
  // let PrintMethod = "printJumpOperand";
}

class ImmAsmOperand<string prefix, int width, string suffix> : AsmOperandClass {
  let Name = prefix # "Imm" # width # suffix;
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

class UImmAsmOperand<int width, string suffix = "">
    : ImmAsmOperand<"U", width, suffix> {
}

def uimm6 : Operand<i32>, TImmLeaf<i32, [{return ((Imm > 0) & (Imm < 64));}]> {
  let ParserMatchClass = UImmAsmOperand<6>;
  let DecoderMethod = "decodeUImmOperand6";
}

// instructions
def IMCE_ADD_INST : InstRType<0b00001, (outs SGPROpnd:$rd), (ins SGPROpnd:$rs1, SGPROpnd:$rs2), "sadd", "$rd $rs1 $rs2", [(set SGPROpnd:$rd, (add SGPROpnd:$rs1, SGPROpnd:$rs2))]>;

def IMCE_VADD_INST : InstRType<0b00000, (outs VGPROpnd:$rd), (ins VGPROpnd:$rs1, VGPROpnd:$rs2), "vadd", "$rd $rs1 $rs2", [(set VGPROpnd:$rd, (add VGPROpnd:$rs1, VGPROpnd:$rs2))]>;

def IMCE_JMP_INST : InstJType<0b00010, (outs), (ins jumpOperand:$imm), "jmp", "$imm", [(br (bb:$imm))]>;

let isReturn = 1, isTerminator = 1, isBarrier = 1, AsmString = "RET" in {
  def IMCE_RET_INST : Pseudo<(outs), (ins), [(retglue)]>;
}

// intrinsics
def SDT_SEND : SDTypeProfile<0, 3, [SDTCisVT<0, v16i16>,
                                    SDTCisVT<1, v16i16>,
                                    SDTCisVT<2, i32>]>;
def IMCE_SEND_NODE : SDNode<"IMCEISD::IMCE_SEND", SDT_SEND, [SDNPHasChain]>;
let fifo_id = 0 in {
  def IMCE_SEND_INST : InstCType<0b11111, (outs), (ins VGPROpnd:$rs1, VGPROpnd:$flag_value, uimm6:$imm2), "send", "$rs1 $flag_value $imm2", [(IMCE_SEND_NODE VGPROpnd:$rs1, VGPROpnd:$flag_value, uimm6:$imm2)]>;
}