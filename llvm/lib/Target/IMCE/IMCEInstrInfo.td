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

// instructions
def ADD : InstRType<0b00001, (outs SGPROpnd:$rd), (ins SGPROpnd:$rs1, SGPROpnd:$rs2), "sadd", "$rs1 $rs2", [(set SGPROpnd:$rd, (add SGPROpnd:$rs1, SGPROpnd:$rs2))]>;

def VADD : InstRType<0b00000, (outs VGPROpnd:$rd), (ins VGPROpnd:$rs1, VGPROpnd:$rs2), "vadd", "$rs1 $rs2", [(set VGPROpnd:$rd, (add VGPROpnd:$rs1, VGPROpnd:$rs2))]>;

def JMP : InstJType<0b00010, (outs), (ins jumpOperand:$imm), "jmp", "$imm", [(br (bb:$imm))]>;

let isReturn = 1, isTerminator = 1, isBarrier = 1, AsmString = "RET" in {
  def RET : Pseudo<(outs), (ins), [(retglue)]>;
}