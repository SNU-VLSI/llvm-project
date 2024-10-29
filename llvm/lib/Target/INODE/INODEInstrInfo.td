//===-- INODEInstrInfo.td - INODE Instructions ---------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the INODE instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

// ---------------------------------------------------------------------------//
// Selection DAG Nodes.
// ---------------------------------------------------------------------------//

// Selection DAG types.

// These are target-independent nodes, but have target-specific formats.
def SDT_CallSeqStart : SDCallSeqStart<[SDTCisVT<0, i16>, SDTCisVT<1, i16>]>;
def SDT_CallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i16>, SDTCisVT<1, i16>]>;
def SDT_Call         : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;

// Selection DAG nodes.
def call             : SDNode<"INODEISD::CALL", SDT_Call,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                              SDNPVariadic]>;
def retglue          : SDNode<"INODEISD::RET_GLUE", SDTNone,
                              [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def INODECloopBeginValue :
  SDNode<"INODEISD::CLOOP_BEGIN_VALUE",
         SDTypeProfile<1, 2, [SDTCisVT<0, i32>, SDTCisVT<1, i32>, SDTCisVT<2, i32>]>,
         [SDNPHasChain]>;
def INODECloopBeginTerminator :
  SDNode<"INODEISD::CLOOP_BEGIN_TERMINATOR",
         SDTypeProfile<0, 2, [SDTCisVT<0, i32>, SDTCisVT<1, i32>]>,
         [SDNPHasChain]>;
def INODECloopEndValue :
  SDNode<"INODEISD::CLOOP_END_VALUE",
         SDTypeProfile<1, 2, [SDTCisVT<0, i32>, SDTCisVT<1, i32>]>,
         [SDNPHasChain]>;
def INODECloopEndBranch :
  SDNode<"INODEISD::CLOOP_END_BRANCH",
         SDTypeProfile<0, 3, [SDTCisVT<1, i32>, SDTCisVT<2, i32>]>,
         [SDNPHasChain]>;

// ===================================================
// operands
// ===================================================

// register operand
class INODERegisterOperand<RegisterClass RC> : RegisterOperand<RC> {
  let DecoderMethod = "decodeRegisterClass";
}

def VGPROpnd : INODERegisterOperand<VGPR> {
  let GIZeroRegister = V0;
}

def QREGOpnd : INODERegisterOperand<QRegs>;
def HWLOOPREGOpnd : INODERegisterOperand<HWLR>;
def CREGOpnd : INODERegisterOperand<CRegs>;

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

def ImmZeroAsmOperand : AsmOperandClass {
  let Name = "ImmZero";
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

def immzero : Operand<i16>,
              TImmLeaf<i16, [{return (Imm == 0);}]> {
  let ParserMatchClass = ImmZeroAsmOperand;
  let OperandType = "OPERAND_ZERO";
  let OperandNamespace = "INODEOp";
}

def uimm1 : Operand<i16>, TImmLeaf<i16, [{return isUInt<1>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<1>;
  let DecoderMethod = "decodeUImmOperand<1>";
  let OperandType = "OPERAND_UIMM1";
  let OperandNamespace = "INODEOp";
}

def uimm2 : Operand<i16>, TImmLeaf<i16, [{return isUInt<2>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<2>;
  let DecoderMethod = "decodeUImmOperand<2>";
  let OperandType = "OPERAND_UIMM2";
  let OperandNamespace = "INODEOp";
}

def uimm3 : Operand<i16>, TImmLeaf<i16, [{return isUInt<3>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<3>;
  let DecoderMethod = "decodeUImmOperand<3>";
  let OperandType = "OPERAND_UIMM3";
  let OperandNamespace = "INODEOp";
}

def uimm4 : Operand<i16>, TImmLeaf<i16, [{return isUInt<4>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<4>;
  let DecoderMethod = "decodeUImmOperand<4>";
  let OperandType = "OPERAND_UIMM4";
  let OperandNamespace = "INODEOp";
}

def uimm4_allone : Operand<i16>, TImmLeaf<i16, [{return (imm == 15);}]> {
  let ParserMatchClass = UImmAsmOperand<4>;
  let DecoderMethod = "decodeUImmOperand<4>";
  let OperandType = "OPERAND_UIMM4_ALLONE";
  let OperandNamespace = "INODEOp";
}

def uimm6 : Operand<i16>, TImmLeaf<i16, [{return isUInt<6>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<6>;
  let DecoderMethod = "decodeUImmOperand<6>";
  let OperandType = "OPERAND_UIMM6";
  let OperandNamespace = "INODEOp";
}

def uimm8 : Operand<i16>, TImmLeaf<i16, [{return isUInt<8>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<8>;
  let DecoderMethod = "decodeUImmOperand<8>";
  let OperandType = "OPERAND_UIMM8";
  let OperandNamespace = "INODEOp";
}

def uimm14 : Operand<i32>, TImmLeaf<i32, [{return isUInt<14>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<14>;
  let DecoderMethod = "decodeUImmOperand<14>";
  let OperandType = "OPERAND_UIMM14";
  let OperandNamespace = "INODEOp";
}

def uimm16 : Operand<i32>, ImmLeaf<i32, [{return isUInt<16>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<16>;
  let DecoderMethod = "decodeUImmOperand<16>";
  let OperandType = "OPERAND_UIMM16";
  let OperandNamespace = "INODEOp";
}

def uimm26 : Operand<i16>, TImmLeaf<i16, [{return isUInt<26>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<26>;
  let DecoderMethod = "decodeUImmOperand<26>";
  let OperandType = "OPERAND_UIMM26";
  let OperandNamespace = "INODEOp";
}

def simm14 : Operand<i16>, TImmLeaf<i16, [{return isInt<14>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<14>;
  let DecoderMethod = "decodeSImmOperand<14>";
  let OperandType = "OPERAND_SIMM14";
  let OperandNamespace = "INODEOp";
}

def JumpTargetAsmOperand : AsmOperandClass {
  let Name = "JumpTarget";
  let PredicateMethod = "isJumpTarget"; // CHECK
  let RenderMethod = "addJumpTargetOperands"; // CHECK
}

def jumptarget : Operand<OtherVT> {
  let EncoderMethod = "getJumpTargetOpValue";
  let DecoderMethod = "decodeJumpTarget";
  let ParserMatchClass = JumpTargetAsmOperand;
  let PrintMethod = "printJumpOperand";
}

def brtarget : Operand<OtherVT> {
  let EncoderMethod = "getBranchTargetOpValue";
  let OperandType = "OPERAND_PCREL";
  let DecoderMethod = "decodeBranchTarget";
  let ParserMatchClass = JumpTargetAsmOperand;
  let PrintMethod = "printBranchOperand";
}

// ========================================
// SDNodes
// ========================================

// ========================================
// instructions
// ========================================
// pseudo instructions
let isReturn = 1, isTerminator = 1, isBarrier = 1, AsmString = "RET" in {
  def INODE_RET_INST : Pseudo<(outs), (ins), "#REGGLUE#", [(retglue)]>;
}

class RegConstraint<string C> {
  string Constraints = C;
}

let mayLoad = 0, mayStore = 0 in {
// The loop value nodes can't be terminators because a copytoreg is needed
// between their location and the terminators
// Metadata limit of imm16zi could be increased by implementing imm32
  let hasSideEffects = 0, isAsCheapAsAMove = 1 in {
    def CLOOP_BEGIN_VALUE : Pseudo<(outs HWLOOPREGOpnd:$op0),
                               (ins uimm14:$op1, uimm16:$meta),
                               "# CLOOP_BEGIN_VALUE $op0, $op1, $meta",
                               []>;

    def CLOOP_END_VALUE : Pseudo<(outs HWLOOPREGOpnd:$op0),
                             (ins HWLOOPREGOpnd:$op1, uimm16:$meta),
                             "# CLOOP_END_VALUE $op0, $op1, $meta",
                             []>,RegConstraint<"$op0 = $op1">;
  }

  def CLOOP_BEGIN_TERMINATOR : Pseudo<(outs),
                                      (ins HWLOOPREGOpnd:$op0, uimm16:$meta),
                                      "# CLOOP_BEGIN_TERMINATOR $op0, $meta",
                                      []> {
    let isTerminator = 1;
    let hasSideEffects = 0;
  }

  def CLOOP_END_BRANCH : Pseudo<(outs),
                            (ins HWLOOPREGOpnd:$op0, brtarget:$op1, uimm16:$meta),
                            "# CLOOP_END_BRANCH $op0, $op1, $meta",
                             []> {
    let isBranch = 1;
    let isTerminator = 1;
    let hasSideEffects = 1;
  }

}

// R type
class RInst<string opname, int opcode> : InstRType<opcode, (outs VGPROpnd:$rd), (ins VGPROpnd:$rs1, VGPROpnd:$rs2, uimm4:$src_mask), opname, "$rd $rs1 $rs2 $src_mask", []>;
let mayLoad=0, mayStore=0 in {
  def INODE_VADD_INST   : RInst<"vadd"  ,0b000000>;
  def INODE_VSUB_INST   : RInst<"vsub"  ,0b000001>;
  def INODE_VAND_INST   : RInst<"vand"  ,0b000010>;
  def INODE_VOR_INST    : RInst<"vor"   ,0b000011>;
  def INODE_VXOR_INST   : RInst<"vxor"  ,0b000100>;
  def INODE_VSRL_INST   : RInst<"vsrl"  ,0b000101>;
  def INODE_VSLL_INST   : RInst<"vsll"  ,0b000110>;
  def INODE_VSRA_INST   : RInst<"vsra"  ,0b000111>;
  def INODE_VMAX_INST   : RInst<"vmax"  ,0b001000>;
  def INODE_VMIN_INST   : RInst<"vmin"  ,0b001001>;
  def INODE_VMULTL_INST : RInst<"vmultl",0b001010>;
  def INODE_VMULTH_INST : RInst<"vmulth",0b001011>;
}

// I type
class IInst<string opname, int opcode> : InstIType<opcode, (outs VGPROpnd:$rd), (ins VGPROpnd:$rs1, simm14:$imm), opname, "$rd $rs1 $imm", []>;
let mayLoad=0, mayStore=0 in {
  def INODE_VADDI_INST   : IInst<"vaddi"  , 0b100000>;
  def INODE_VSUBI_INST   : IInst<"vsubi"  , 0b100001>;
  def INODE_VANDI_INST   : IInst<"vandi"  , 0b100010>;
  def INODE_VORI_INST    : IInst<"vori"   , 0b100011>;
  def INODE_VXORI_INST   : IInst<"vxori"  , 0b100100>;
  def INODE_VSRLI_INST   : IInst<"vsrli"  , 0b100101>;
  def INODE_VSLLI_INST   : IInst<"vslli"  , 0b100110>;
  def INODE_VSRAI_INST   : IInst<"vsrai"  , 0b100111>;
  def INODE_VMAXI_INST   : IInst<"vmaxi"  , 0b101000>;
  def INODE_VMINI_INST   : IInst<"vmini"  , 0b101001>;
  def INODE_VMULTLI_INST : IInst<"vmultli", 0b101010>;
  def INODE_VMULTHI_INST : IInst<"vmulthi", 0b101011>;
}

// A type
class AInst<string opname, int opcode> : InstAType<opcode, (outs VGPROpnd:$rd), (ins VGPROpnd:$rs1, uimm3:$shift_amt, uimm1:$dwresult_valid, uimm4:$src_mask, uimm2:$bshr_sel), opname, "$rd $rs1 $shift_amt $dwresult_valid $src_mask $bshr_sel", []>;
def INODE_DWCONV_INST : AInst<"dwconv", 0b001100> {
  let mayLoad=0;
  let mayStore=0;
  let Uses = [DWREG, BSHR0, BSHR1, BSHR2, BSHR3];
  let Defs = [DWREG];
  let hasSideEffects = 1;
}

// B type
class BInst<string opname, int opcode> : InstBType<opcode, (outs HWLOOPREGOpnd:$rs2), (ins brtarget:$imm2, HWLOOPREGOpnd:$rs1, uimm14:$imm), opname, "$imm2 $rs1 $imm", []>;
def INODE_BNE_INST : BInst<"bne", 0b111000> {
  let mayLoad=0;
  let mayStore=0;
  let isBranch = 1;
  let isTerminator = 1;
  let Constraints = "$rs2 = $rs1";
}

def INODE_LONG_BNE : Pseudo<(outs HWLOOPREGOpnd:$rs2),
                           (ins brtarget:$imm2, HWLOOPREGOpnd:$rs1, uimm14:$imm),
                          "# LONG_BNE $imm2, $rs1, $imm",
                           []> {
  let isBarrier = 1;
  let isBranch = 1;
  let hasSideEffects = 0;
  let mayStore = 0;
  let mayLoad = 0;
  let isAsmParserOnly = 1;
  let hasNoSchedulingInfo = 1;
}

// C type
class CInst<string opname, int opcode> : InstCType<opcode, (outs), (ins uimm6:$imm2, VGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value), opname, "$imm2 $rs1 $fifo_id, $flag_value", []>;
let hasSideEffects=1 in {
  def INODE_SEND_INST : InstINODE<(outs), (ins uimm6:$imm2, VGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value), "send", "$imm2 $rs1 $fifo_id, $flag_value", []> {
    bits<6> imm2;
    bits<6> rs1;
    bits<6> fifo_id;
    bits<8> flag_value;

    let Inst{5-0} = 0b110010;
    let Inst{11-6} = imm2;
    let Inst{17-12} = rs1;
    let Inst{23-18} = fifo_id;
    let Inst{31-24} = flag_value;
  }

  def INODE_RECV_INST : InstINODE<(outs VGPROpnd:$rd), (ins uimm6:$fifo_id), "recv", "$rd $fifo_id", []> {
    bits<6> rd;
    bits<6> fifo_id;

    let Inst{5-0} = 0b110010;
    let Inst{11-6} = rd;
    let Inst{17-12} = 0;
    let Inst{23-18} = fifo_id;
    let Inst{31-24} = 0;
  }

  def INODE_SET_FLAG_INST : InstINODE<(outs), (ins VGPROpnd:$rs1), "set_flag", "$rs1", []> {
    bits<6> rs1;

    let Inst{5-0} = 0b110100;
    let Inst{11-6} = 0;
    let Inst{17-12} = 0;
    let Inst{23-18} = 0;
    let Inst{29-24} = rs1;
    let Inst{31-30} = 0;
  }

  def INODE_STANDBY_INST  : InstINODE<(outs), (ins uimm6:$reg_id, VGPROpnd:$rs1), "standby", "$reg_id $rs1", []> {
    bits<6> rs1;
    bits<6> reg_id;

    let Inst{5-0} = 0b110100;
    let Inst{11-6} = reg_id;
    let Inst{17-12} = 0;
    let Inst{23-18} = 0;
    let Inst{29-24} = rs1;
    let Inst{31-30} = 0;
  }
}

def INODE_LOAD_LB : Pseudo<(outs), (ins uimm6:$fifo_id), "# LOAD_LB $fifo_id", []> {
  let hasSideEffects = 1;
  let Defs = [BSHR0, BSHR1, BSHR2, BSHR3];
}

// P type
class PInst<string opname, int opcode> : InstPType<opcode, (outs VGPROpnd:$rd), (ins uimm1:$ksel, uimm4:$src_mask, uimm2:$bshr_sel), opname, "$rd $ksel $src_mask $bshr_sel", []>;
let Uses = [BSHR0, BSHR1, BSHR2, BSHR3] in {
  def INODE_MAXPOOL_INST : PInst<"maxpool", 0b001101>;
  def INODE_AVGPOOL_INST : PInst<"avgpool", 0b001110>;
}

// Q type
class QInst<string opname, int opcode> : InstQType<opcode, (outs), (ins VGPROpnd:$rs1, VGPROpnd:$rs2, uimm4:$src_mask, uimm4:$qreg_offset), opname, "$rs1 $rs2 $src_mask $qreg_offset", []>;
let Defs=[QREG0, QREG1, QREG2, QREG3] in {
  def INODE_ADDQ_INST     : QInst<"addq", 0b010000>;
  def INODE_SUBQ_INST     : QInst<"subq", 0b010001>;
  def INODE_MULTLQ_INST   : QInst<"multlq", 0b011010>;
  def INODE_MULTHQ_INST   : QInst<"multhq", 0b011011>;
  def INODE_NU_QUANT_INST : QInst<"nu_quant", 0b011100>;
  def INODE_MM_QUANT_INST : QInst<"mm_quant", 0b011101>;
}

// Pseduo for Q type
// def INODE_GET_QREG_Pseudo : Pseudo<(outs QREGOpnd:$rd), (ins uimm2:$idx), "# GET_QREG $rd $idx", []> {
//   let Uses = [QREG0, QREG1, QREG2, QREG3];
// }

// X type
class XInst<string opname, int opcode> : InstXType<opcode, (outs), (ins), opname, "", []>;
// def INODE_STEP_INST : InstINODE<(outs CREGOpnd:$rd0, CREGOpnd:$rd1, CREGOpnd:$rd2, CREGOpnd:$rd3), (ins), "step", "", []> {
//   let Inst{5-0} = 0b111101;
//   let Uses = [BSHR_REG0, BSHR_REG1, BSHR_REG2, BSHR_REG3];
// }
def INODE_STEP_INST : InstINODE<(outs), (ins), "step", "", []> {
  let Inst{5-0} = 0b111101;
  let Uses = [BSHR0, BSHR1, BSHR2, BSHR3];
  let Defs = [CREG0, CREG1, CREG2, CREG3];
}

def INODE_NOP_INST  : XInst<"nop" , 0b111110>;
def INODE_STOP_INST : XInst<"stop", 0b111111>;

// def INODE_GET_CREG_Pseudo : Pseudo<(outs CREGOpnd:$rd), (ins uimm2:$idx), "# GET_CREG $rd $idx", []> {
//   let Uses = [CREG0, CREG1, CREG2, CREG3];
// }

// J type
// def INODE_JMP_INST : InstJType<0b111100, (outs), (ins jumptarget:$imm), "jmp", "$imm", [(br (bb:$imm))]> {
def INODE_JMP_INST : InstJType<0b111100, (outs), (ins jumptarget:$imm), "jmp", "$imm", []> {
  let isTerminator=1;
  let isBarrier=1;
}

// S type
def INODE_SCAN_RW_INST : InstSType<0b101100, (outs VGPROpnd:$rd), (ins VGPROpnd:$rs1), "scan_rw", "$rd $rs1", []> {
  let Uses = [SCANREG0, SCANREG1];
  let Defs = [SCANREG0, SCANREG1];
}

// ================================================
// Patterns
// ================================================
// R pattern
class RPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op VGPROpnd:$rs1, VGPROpnd:$rs2, uimm4:$src_mask), (inst VGPROpnd:$rs1, VGPROpnd:$rs2, uimm4:$src_mask)>;
def : RPat<int_INODE_ADD, INODE_VADD_INST>;
def : RPat<int_INODE_SUB, INODE_VSUB_INST>;
def : RPat<int_INODE_AND, INODE_VAND_INST>;
def : RPat<int_INODE_OR, INODE_VOR_INST>;
def : RPat<int_INODE_XOR, INODE_VXOR_INST>;
def : RPat<int_INODE_SRL, INODE_VSRL_INST>;
def : RPat<int_INODE_SLL, INODE_VSLL_INST>;
def : RPat<int_INODE_SRA, INODE_VSRA_INST>;
def : RPat<int_INODE_MAX, INODE_VMAX_INST>;
def : RPat<int_INODE_MIN, INODE_VMIN_INST>;
def : RPat<int_INODE_MULTL, INODE_VMULTL_INST>;
def : RPat<int_INODE_MULTH, INODE_VMULTH_INST>;
// def : Pat<(add VGPROpnd:$rs1, VGPROpnd:$rs2), (Pseudo_INODE_VADD_INST_all_lane VGPROpnd:$rs1, VGPROpnd:$rs2)>;
// def : Pat<(add SGPROpnd:$rs1, SGPROpnd:$rs2), (Pseudo_INODE_VADD_INST_one_lane SGPROpnd:$rs1, SGPROpnd:$rs2)>;
def : Pat<(add (v16i16 VGPROpnd:$rs1), (v16i16 VGPROpnd:$rs2)), (INODE_VADD_INST (v16i16 VGPROpnd:$rs1), (v16i16 VGPROpnd:$rs2), 15)>;
def : Pat<(add (i16 VGPROpnd:$rs1), (i16 VGPROpnd:$rs2)), (INODE_VADD_INST (i16 VGPROpnd:$rs1), (i16 VGPROpnd:$rs2), 0)>;
// def : Pat<(add (i16 SGPROpnd:$rs1), (i16 SGPROpnd:$rs2)), (INODE_VADD_INST (i16 VGPROpnd:$rs1), (i16 VGPROpnd:$rs2), 0)>;

// I type pattern
class IPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op VGPROpnd:$rs1, simm14:$imm), (inst VGPROpnd:$rs1, simm14:$imm)>;
def : IPat<int_INODE_ADDI  , INODE_VADDI_INST>;
def : IPat<int_INODE_SUBI  , INODE_VSUBI_INST>;
def : IPat<int_INODE_ANDI  , INODE_VANDI_INST>;
def : IPat<int_INODE_ORI   , INODE_VORI_INST>;
def : IPat<int_INODE_XORI  , INODE_VXORI_INST>;
def : IPat<int_INODE_SRLI  , INODE_VSRLI_INST>;
def : IPat<int_INODE_SLLI  , INODE_VSLLI_INST>;
def : IPat<int_INODE_SRAI  , INODE_VSRAI_INST>;
def : IPat<int_INODE_MAXI  , INODE_VMAXI_INST>;
def : IPat<int_INODE_MINI  , INODE_VMINI_INST>;
def : IPat<int_INODE_MULTLI, INODE_VMULTLI_INST>;
def : IPat<int_INODE_MULTHI, INODE_VMULTHI_INST>;
def : Pat<(v16i16 (build_vector (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0))),
          (v16i16 (INODE_VADDI_INST (i16 V0), (i16 0)))>;

// A type pattern
class APat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op VGPROpnd:$rs1, uimm3:$shift_amt, uimm1:$dwresult_valid, uimm4:$src_mask, uimm2:$bshr_sel), (inst VGPROpnd:$rs1, uimm3:$shift_amt, uimm1:$dwresult_valid, uimm4:$src_mask, uimm2:$bshr_sel)>;
def : APat<int_INODE_DWCONV, INODE_DWCONV_INST>;

// B type pattern
// class BPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op simm6:$imm2, HWLOOPREGOpnd:$rs1, uimm14:$imm), (inst simm6:$imm2, HWLOOPREGOpnd:$rs1, uimm14:$imm)>;
// def : BPat<int_INODE_BNE, INODE_BNE_INST>;

// C type pattern
// class CPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op uimm6:$imm2, VGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value), (inst uimm6:$imm2, VGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value)>;
def : Pat<(int_INODE_SEND uimm6:$imm2, VGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value), (INODE_SEND_INST uimm6:$imm2, VGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value)>;
def : Pat<(int_INODE_RECV uimm6:$fifo_id), (INODE_RECV_INST uimm6:$fifo_id)>;
def : Pat<(int_INODE_SETFLAG VGPROpnd:$rs1), (INODE_SET_FLAG_INST VGPROpnd:$rs1)>;
def : Pat<(int_INODE_STANDBY uimm6:$reg_id, VGPROpnd:$rs1), (INODE_STANDBY_INST uimm6:$reg_id, VGPROpnd:$rs1)>;
def : Pat<(int_INODE_LOAD_LB uimm6:$fifo_id), (INODE_LOAD_LB uimm6:$fifo_id)>;

// P type Pattern
class PPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op uimm1:$ksel, uimm4:$src_mask, uimm2:$bshr_sel), (inst uimm1:$ksel, uimm4:$src_mask, uimm2:$bshr_sel)>;
def : PPat<int_INODE_MAXPOOL, INODE_MAXPOOL_INST>;
def : PPat<int_INODE_AVGPOOL, INODE_AVGPOOL_INST>;

// Q type Pattern
class QPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op VGPROpnd:$rs1, VGPROpnd:$rs2, uimm4:$src_mask, uimm4:$qreg_offset), (inst VGPROpnd:$rs1, VGPROpnd:$rs2, uimm4:$src_mask, uimm4:$qreg_offset)>;
def : QPat<int_INODE_ADDQ    , INODE_ADDQ_INST>;
def : QPat<int_INODE_SUBQ    , INODE_SUBQ_INST>;
def : QPat<int_INODE_MULTLQ  , INODE_MULTLQ_INST>;
def : QPat<int_INODE_MULTHQ  , INODE_MULTHQ_INST>;
def : QPat<int_INODE_NU_QUANT, INODE_NU_QUANT_INST>;
def : QPat<int_INODE_MM_QUANT, INODE_MM_QUANT_INST>;

// X type Pattern
class XPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op), (inst)>;
def : XPat<int_INODE_STEP, INODE_STEP_INST>;
def : XPat<int_INODE_NOP , INODE_NOP_INST>;
def : XPat<int_INODE_STOP, INODE_STOP_INST>;

// J type pattern
// class JPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op jumptarget:$imm), (inst jumptarget:$imm)>;
// def : JPat<int_INODE_JUMP, INODE_JMP_INST>;
def : Pat<(br (bb:$imm)), (INODE_JMP_INST jumptarget:$imm)>;

// S type Pattern
class SPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op VGPROpnd:$rs1), (inst VGPROpnd:$rs1)>;
def : SPat<int_INODE_SCAN_RW, INODE_SCAN_RW_INST>;

def i32MR : PatLeaf<(i32 HWLOOPREGOpnd:$R)>;
// def : Pat<(i32 (INODECloopBeginValue i32MR:$op0, uimm16:$meta)),
//           (i32 (CLOOP_BEGIN_VALUE i32MR:$op0, uimm16:$meta))>;
def : Pat<(i32 (INODECloopBeginValue uimm14:$op0, uimm16:$meta)),
          (i32 (CLOOP_BEGIN_VALUE uimm14:$op0, uimm16:$meta))>;
def : Pat<(i32 (INODECloopEndValue i32MR:$op0, uimm16:$meta)),
          (i32 (CLOOP_END_VALUE i32MR:$op0, uimm16:$meta))>;
def : Pat<(INODECloopBeginTerminator i32MR:$op0, uimm16:$meta),
          (CLOOP_BEGIN_TERMINATOR i32MR:$op0, uimm16:$meta)>;
def : Pat<(INODECloopEndBranch bb:$op0, i32MR:$op1, uimm16:$meta),
          (CLOOP_END_BRANCH i32MR:$op1, bb:$op0, uimm16:$meta)>;
// def : Pat<(INODECloopGuardBranch bb:$op0, i32MR:$op1, uimm16:$meta),
//           (CLOOP_GUARD_BRANCH i32MR:$op1, bb:$op0, uimm16:$meta)>;