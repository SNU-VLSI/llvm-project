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

// ===================================================
// operands
// ===================================================

// register operand
class IMCERegisterOperand<RegisterClass RC> : RegisterOperand<RC> {
  let DecoderMethod = "decode"#RC#"RegisterClass";
}

def SGPROpnd : IMCERegisterOperand<SGPR> {
  let GIZeroRegister = S0;
}

def VGPROpnd : IMCERegisterOperand<VGPR> {
  let GIZeroRegister = V0;
}

// immediate operands
class ImmAsmOperand<string prefix, int width, string suffix> : AsmOperandClass {
  let Name = prefix # "Imm" # width # suffix;
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

class SImmAsmOperand<int width, string suffix = "">
    : ImmAsmOperand<"S", width, suffix> {
}

class UImmAsmOperand<int width, string suffix = "">
    : ImmAsmOperand<"U", width, suffix> {
}

def uimm1 : Operand<i32>, TImmLeaf<i32, [{return isUInt<1>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<1>;
  let DecoderMethod = "decodeUImmOperand<1>";
  let OperandType = "OPERAND_UIMM1";
  let OperandNamespace = "IMCEOp";
}

def uimm2 : Operand<i32>, TImmLeaf<i32, [{return isUInt<2>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<2>;
  let DecoderMethod = "decodeUImmOperand<2>";
  let OperandType = "OPERAND_UIMM2";
  let OperandNamespace = "IMCEOp";
}

def uimm3 : Operand<i32>, TImmLeaf<i32, [{return isUInt<3>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<3>;
  let DecoderMethod = "decodeUImmOperand<3>";
  let OperandType = "OPERAND_UIMM3";
  let OperandNamespace = "IMCEOp";
}

def uimm4 : Operand<i32>, TImmLeaf<i32, [{return isUInt<4>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<4>;
  let DecoderMethod = "decodeUImmOperand<4>";
  let OperandType = "OPERAND_UIMM4";
  let OperandNamespace = "IMCEOp";
}

def uimm6 : Operand<i32>, TImmLeaf<i32, [{return isUInt<6>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<6>;
  let DecoderMethod = "decodeUImmOperand<6>";
  let OperandType = "OPERAND_UIMM6";
  let OperandNamespace = "IMCEOp";
}

def uimm8 : Operand<i32>, TImmLeaf<i32, [{return isUInt<8>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<8>;
  let DecoderMethod = "decodeUImmOperand<8>";
  let OperandType = "OPERAND_UIMM8";
  let OperandNamespace = "IMCEOp";
}

def uimm14 : Operand<i32>, TImmLeaf<i32, [{return isUInt<14>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<14>;
  let DecoderMethod = "decodeUImmOperand<14>";
  let OperandType = "OPERAND_UIMM14";
  let OperandNamespace = "IMCEOp";
}

def uimm26 : Operand<i32>, TImmLeaf<i32, [{return isUInt<26>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<26>;
  let DecoderMethod = "decodeUImmOperand<26>";
  let OperandType = "OPERAND_UIMM26";
  let OperandNamespace = "IMCEOp";
}

def simm14 : Operand<i32>, TImmLeaf<i32, [{return isSInt<14>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<14>;
  let DecoderMethod = "decodeSImmOperand<14>";
  let OperandType = "OPERAND_SIMM14";
  let OperandNamespace = "IMCEOp";
}

def JumpTargetAsmOperand : AsmOperandClass {
  let Name = "JumpTarget";
  let PredicateMethod = "isJumpTarget";
  let RenderMethod = "addJumpTargetOperands";
}

def jumpOperand : Operand<OtherVT> {
  let ParserMatchClass = JumpTargetAsmOperand;
}

// ========================================
// SDNodes
// ========================================
// def SDT_SEND : SDTypeProfile<0, 3, [SDTCisVT<0, v16i16>, SDTCisVT<1, v16i16>, SDTCisVT<2, i32>]>;
// def IMCE_SEND_NODE : SDNode<"IMCEISD::IMCE_SEND", SDT_SEND, [SDNPHasChain]>;

// ========================================
// instructions
// ========================================
// pseudo instructions
let isReturn = 1, isTerminator = 1, isBarrier = 1, AsmString = "RET" in {
  def IMCE_RET_INST : Pseudo<(outs), (ins), [(retglue)]>;
}

// R type
class RInst<string opname, string operand_names, int opcode, SDPatternOperator dag_op> : 
  InstRType<opcode, (outs VGPROpnd:$rd), 
                    (ins VGPROpnd:$rs1, VGPROpnd:$rs2, uimm4:$src_mask), 
                    opname, operand_names, 
                    [(set VGPROpnd:$rd, (dag_op VGPROpnd:$rs1, VGPROpnd:$rs2, uimm4:$src_mask))]>;
// def IMCE_ADD_INST : InstRType<0b00001, (outs SGPROpnd:$rd), (ins SGPROpnd:$rs1, SGPROpnd:$rs2), "sadd", "$rd $rs1 $rs2", [(set SGPROpnd:$rd, (add SGPROpnd:$rs1, SGPROpnd:$rs2))]>;
def IMCE_VADD_INST : RInst<"vadd", "$rd $rs1 $rs2", 0b000000, int_IMCE_ADD>;
def IMCE_VSUB_INST : RInst<"vsub", "$rd $rs1 $rs2", 0b000001, int_IMCE_SUB>;

// J type
def IMCE_JMP_INST : InstJType<0b00010, (outs), (ins jumpOperand:$imm), "jmp", "$imm", [(br (bb:$imm))]>;

// C type
// let fifo_id = 0 in {
//   def IMCE_SEND_INST : InstCType<0b11111, (outs), (ins VGPROpnd:$rs1, VGPROpnd:$flag_value, uimm6:$imm2), "send", "$rs1 $flag_value $imm2", [(IMCE_SEND_NODE VGPROpnd:$rs1, VGPROpnd:$flag_value, uimm6:$imm2)]>;
// }