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
def SDT_CallSeqStart : SDCallSeqStart<[SDTCisVT<0, i16>, SDTCisVT<1, i16>]>;
def SDT_CallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i16>, SDTCisVT<1, i16>]>;
def SDT_Call         : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;

// Selection DAG nodes.
def call             : SDNode<"IMCEISD::CALL", SDT_Call,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                              SDNPVariadic]>;
def retglue          : SDNode<"IMCEISD::RET_GLUE", SDTNone,
                              [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def IMCECloopBeginValue :
  SDNode<"IMCEISD::CLOOP_BEGIN_VALUE",
         SDTypeProfile<1, 2, [SDTCisVT<0, i32>, SDTCisVT<1, i32>, SDTCisVT<2, i32>]>,
         [SDNPHasChain]>;
def IMCECloopBeginTerminator :
  SDNode<"IMCEISD::CLOOP_BEGIN_TERMINATOR",
         SDTypeProfile<0, 2, [SDTCisVT<0, i32>, SDTCisVT<1, i32>]>,
         [SDNPHasChain]>;
def IMCECloopEndValue :
  SDNode<"IMCEISD::CLOOP_END_VALUE",
         SDTypeProfile<1, 2, [SDTCisVT<0, i32>, SDTCisVT<1, i32>]>,
         [SDNPHasChain]>;
def IMCECloopEndBranch :
  SDNode<"IMCEISD::CLOOP_END_BRANCH",
         SDTypeProfile<0, 3, [SDTCisVT<1, i32>, SDTCisVT<2, i32>]>,
         [SDNPHasChain]>;

// ===================================================
// operands
// ===================================================

// register operand
class IMCERegisterOperand<RegisterClass RC> : RegisterOperand<RC> {
  let DecoderMethod = "decode"#RC#"RegisterClass";
}

def VGPROpnd : IMCERegisterOperand<VGPR> {
  let GIZeroRegister = V0;
}

def QREGOpnd : IMCERegisterOperand<QRegs>;
def HWLOOPREGOpnd : IMCERegisterOperand<HWLR>;
def CREGOpnd : IMCERegisterOperand<CRegs>;

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
  let OperandNamespace = "IMCEOp";
}

def uimm1 : Operand<i16>, TImmLeaf<i16, [{return isUInt<1>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<1>;
  let DecoderMethod = "decodeUImmOperand<1>";
  let OperandType = "OPERAND_UIMM1";
  let OperandNamespace = "IMCEOp";
}

def uimm2 : Operand<i16>, TImmLeaf<i16, [{return isUInt<2>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<2>;
  let DecoderMethod = "decodeUImmOperand<2>";
  let OperandType = "OPERAND_UIMM2";
  let OperandNamespace = "IMCEOp";
}

def uimm3 : Operand<i16>, TImmLeaf<i16, [{return isUInt<3>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<3>;
  let DecoderMethod = "decodeUImmOperand<3>";
  let OperandType = "OPERAND_UIMM3";
  let OperandNamespace = "IMCEOp";
}

def uimm4 : Operand<i16>, TImmLeaf<i16, [{return isUInt<4>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<4>;
  let DecoderMethod = "decodeUImmOperand<4>";
  let OperandType = "OPERAND_UIMM4";
  let OperandNamespace = "IMCEOp";
}

def uimm4_allone : Operand<i16>, TImmLeaf<i16, [{return (imm == 15);}]> {
  let ParserMatchClass = UImmAsmOperand<4>;
  let DecoderMethod = "decodeUImmOperand<4>";
  let OperandType = "OPERAND_UIMM4_ALLONE";
  let OperandNamespace = "IMCEOp";
}

def uimm6 : Operand<i16>, TImmLeaf<i16, [{return isUInt<6>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<6>;
  let DecoderMethod = "decodeUImmOperand<6>";
  let OperandType = "OPERAND_UIMM6";
  let OperandNamespace = "IMCEOp";
}

def uimm8 : Operand<i16>, TImmLeaf<i16, [{return isUInt<8>(Imm);}]> {
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

def uimm16 : Operand<i32>, ImmLeaf<i32, [{return isUInt<16>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<16>;
  let DecoderMethod = "decodeUImmOperand<16>";
  let OperandType = "OPERAND_UIMM16";
  let OperandNamespace = "IMCEOp";
}

def uimm26 : Operand<i16>, TImmLeaf<i16, [{return isUInt<26>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<26>;
  let DecoderMethod = "decodeUImmOperand<26>";
  let OperandType = "OPERAND_UIMM26";
  let OperandNamespace = "IMCEOp";
}

def simm6 : Operand<i16>, TImmLeaf<i16, [{return isInt<6>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<6>;
  let DecoderMethod = "decodeSImmOperand<6>";
  let OperandType = "OPERAND_SIMM6";
  let OperandNamespace = "IMCEOp";
}

def simm14 : Operand<i16>, TImmLeaf<i16, [{return isInt<14>(Imm);}]> {
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

// ========================================
// instructions
// ========================================
// pseudo instructions
let isReturn = 1, isTerminator = 1, isBarrier = 1, AsmString = "RET" in {
  def IMCE_RET_INST : Pseudo<(outs), (ins), "#REGGLUE#", [(retglue)]>;
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
                            (ins HWLOOPREGOpnd:$op0, jumpOperand:$op1, uimm16:$meta),
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
  def IMCE_VADD_INST   : RInst<"vadd"  ,0b000000>;
  def IMCE_VSUB_INST   : RInst<"vsub"  ,0b000001>;
  def IMCE_VAND_INST   : RInst<"vand"  ,0b000010>;
  def IMCE_VOR_INST    : RInst<"vor"   ,0b000011>;
  def IMCE_VXOR_INST   : RInst<"vxor"  ,0b000100>;
  def IMCE_VSRL_INST   : RInst<"vsrl"  ,0b000101>;
  def IMCE_VSLL_INST   : RInst<"vsll"  ,0b000110>;
  def IMCE_VSRA_INST   : RInst<"vsra"  ,0b000111>;
  def IMCE_VMAX_INST   : RInst<"vmax"  ,0b001000>;
  def IMCE_VMIN_INST   : RInst<"vmin"  ,0b001001>;
  def IMCE_VMULTL_INST : RInst<"vmultl",0b001010>;
  def IMCE_VMULTH_INST : RInst<"vmulth",0b001011>;
}

// I type
class IInst<string opname, int opcode> : InstIType<opcode, (outs VGPROpnd:$rd), (ins VGPROpnd:$rs1, simm14:$imm), opname, "$rd $rs1 $imm", []>;
let mayLoad=0, mayStore=0 in {
  def IMCE_VADDI_INST   : IInst<"vaddi"  , 0b100000>;
  def IMCE_VSUBI_INST   : IInst<"vsubi"  , 0b100001>;
  def IMCE_VANDI_INST   : IInst<"vandi"  , 0b100010>;
  def IMCE_VORI_INST    : IInst<"vori"   , 0b100011>;
  def IMCE_VXORI_INST   : IInst<"vxori"  , 0b100100>;
  def IMCE_VSRLI_INST   : IInst<"vsrli"  , 0b100101>;
  def IMCE_VSLLI_INST   : IInst<"vslli"  , 0b100110>;
  def IMCE_VSRAI_INST   : IInst<"vsrai"  , 0b100111>;
  def IMCE_VMAXI_INST   : IInst<"vmaxi"  , 0b101000>;
  def IMCE_VMINI_INST   : IInst<"vmini"  , 0b101001>;
  def IMCE_VMULTLI_INST : IInst<"vmultli", 0b101010>;
  def IMCE_VMULTHI_INST : IInst<"vmulthi", 0b101011>;
}

// A type
class AInst<string opname, int opcode> : InstAType<opcode, (outs VGPROpnd:$rd), (ins VGPROpnd:$rs1, uimm3:$shift_amt, uimm1:$dwresult_valid, uimm4:$src_mask, uimm2:$bshr_sel), opname, "$rd $rs1 $shift_amt $dwresult_valid $src_mask $bshr_sel", []>;
def IMCE_DWCONV_INST : AInst<"dwconv", 0b001100> {
  let mayLoad=0;
  let mayStore=0;
  let Uses = [DWREG, BSHR_REG0, BSHR_REG1, BSHR_REG2, BSHR_REG3];
  let Defs = [DWREG];
  let hasSideEffects = 1;
}

// B type
class BInst<string opname, int opcode> : InstBType<opcode, (outs HWLOOPREGOpnd:$rs2), (ins simm6:$imm2, HWLOOPREGOpnd:$rs1, uimm14:$imm), opname, "$imm2 $rs1 $imm", []>;
def IMCE_BNE_INST : BInst<"bne", 0b111000> {
  let mayLoad=0;
  let mayStore=0;
  let Constraints = "$rs2 = $rs1";
}

// C type
class CInst<string opname, int opcode> : InstCType<opcode, (outs), (ins uimm6:$imm2, VGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value), opname, "$imm2 $rs1 $fifo_id, $flag_value", []>;
let hasSideEffects=1 in {
  def IMCE_SEND_INST : InstIMCE<(outs), (ins uimm6:$imm2, VGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value), "send", "$imm2 $rs1 $fifo_id, $flag_value", []> {
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

  def IMCE_RECV_INST : InstIMCE<(outs VGPROpnd:$rd), (ins uimm6:$fifo_id), "recv", "$rd $fifo_id", []> {
    bits<6> rd;
    bits<6> fifo_id;

    let Inst{5-0} = 0b110010;
    let Inst{11-6} = rd;
    let Inst{17-12} = 0;
    let Inst{23-18} = fifo_id;
    let Inst{31-24} = 0;
  }

  def IMCE_SET_FLAG_INST : InstIMCE<(outs), (ins VGPROpnd:$rs1), "set_flag", "$rs1", []> {
    bits<6> rs1;

    let Inst{5-0} = 0b110100;
    let Inst{11-6} = 0;
    let Inst{17-12} = 0;
    let Inst{23-18} = 0;
    let Inst{29-24} = rs1;
    let Inst{31-30} = 0;
  }

  def IMCE_STANDBY_INST  : InstIMCE<(outs), (ins uimm6:$reg_id, VGPROpnd:$rs1), "standby", "$reg_id $rs1", []> {
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

def IMCE_LOAD_LB : Pseudo<(outs), (ins uimm6:$fifo_id), "# LOAD_LB $fifo_id", []> {
  let hasSideEffects = 1;
  let Defs = [BSHR_REG0, BSHR_REG1, BSHR_REG2, BSHR_REG3];
}

// P type
class PInst<string opname, int opcode> : InstPType<opcode, (outs VGPROpnd:$rd), (ins uimm1:$ksel, uimm4:$src_mask, uimm2:$bshr_sel), opname, "$rd $ksel $src_mask $bshr_sel", []>;
let Uses = [BSHR_REG0, BSHR_REG1, BSHR_REG2, BSHR_REG3] in {
  def IMCE_MAXPOOL_INST : PInst<"maxpool", 0b001101>;
  def IMCE_AVGPOOL_INST : PInst<"avgpool", 0b001110>;
}

// Q type
class QInst<string opname, int opcode> : InstQType<opcode, (outs), (ins VGPROpnd:$rs1, VGPROpnd:$rs2, uimm4:$src_mask, uimm4:$qreg_offset), opname, "$rs1 $rs2 $src_mask $qreg_offset", []>;
let Defs=[QREG0, QREG1, QREG2, QREG3] in {
  def IMCE_ADDQ_INST     : QInst<"addq", 0b010000>;
  def IMCE_SUBQ_INST     : QInst<"subq", 0b010001>;
  def IMCE_MULTLQ_INST   : QInst<"multlq", 0b011010>;
  def IMCE_MULTHQ_INST   : QInst<"multhq", 0b011011>;
  def IMCE_NU_QUANT_INST : QInst<"nu_quant", 0b011100>;
  def IMCE_MM_QUANT_INST : QInst<"mm_quant", 0b011101>;
}

// Pseduo for Q type
// def IMCE_GET_QREG_Pseudo : Pseudo<(outs QREGOpnd:$rd), (ins uimm2:$idx), "# GET_QREG $rd $idx", []> {
//   let Uses = [QREG0, QREG1, QREG2, QREG3];
// }

// X type
class XInst<string opname, int opcode> : InstXType<opcode, (outs), (ins), opname, "", []>;
// def IMCE_STEP_INST : InstIMCE<(outs CREGOpnd:$rd0, CREGOpnd:$rd1, CREGOpnd:$rd2, CREGOpnd:$rd3), (ins), "step", "", []> {
//   let Inst{5-0} = 0b111101;
//   let Uses = [BSHR_REG0, BSHR_REG1, BSHR_REG2, BSHR_REG3];
// }
def IMCE_STEP_INST : InstIMCE<(outs), (ins), "step", "", []> {
  let Inst{5-0} = 0b111101;
  let Uses = [BSHR_REG0, BSHR_REG1, BSHR_REG2, BSHR_REG3];
  let Defs = [CREG0, CREG1, CREG2, CREG3];
}

def IMCE_NOP_INST  : XInst<"nop" , 0b111110>;
def IMCE_STOP_INST : XInst<"stop", 0b111111>;

// def IMCE_GET_CREG_Pseudo : Pseudo<(outs CREGOpnd:$rd), (ins uimm2:$idx), "# GET_CREG $rd $idx", []> {
//   let Uses = [CREG0, CREG1, CREG2, CREG3];
// }

// J type
// def IMCE_JMP_INST : InstJType<0b111100, (outs), (ins jumpOperand:$imm), "jmp", "$imm", [(br (bb:$imm))]> {
def IMCE_JMP_INST : InstJType<0b111100, (outs), (ins jumpOperand:$imm), "jmp", "$imm", []> {
  let isTerminator=1;
  let isBarrier=1;
}

// S type
def IMCE_SCAN_RW_INST : InstSType<0b101100, (outs VGPROpnd:$rd), (ins VGPROpnd:$rs1), "scan_rw", "$rd $rs1", []> {
  let Uses = [SCANREG0, SCANREG1];
  let Defs = [SCANREG0, SCANREG1];
}

// ================================================
// Patterns
// ================================================
// R pattern
class RPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op VGPROpnd:$rs1, VGPROpnd:$rs2, uimm4:$src_mask), (inst VGPROpnd:$rs1, VGPROpnd:$rs2, uimm4:$src_mask)>;
def : RPat<int_IMCE_ADD, IMCE_VADD_INST>;
def : RPat<int_IMCE_SUB, IMCE_VSUB_INST>;
def : RPat<int_IMCE_AND, IMCE_VAND_INST>;
def : RPat<int_IMCE_OR, IMCE_VOR_INST>;
def : RPat<int_IMCE_XOR, IMCE_VXOR_INST>;
def : RPat<int_IMCE_SRL, IMCE_VSRL_INST>;
def : RPat<int_IMCE_SLL, IMCE_VSLL_INST>;
def : RPat<int_IMCE_SRA, IMCE_VSRA_INST>;
def : RPat<int_IMCE_MAX, IMCE_VMAX_INST>;
def : RPat<int_IMCE_MIN, IMCE_VMIN_INST>;
def : RPat<int_IMCE_MULTL, IMCE_VMULTL_INST>;
def : RPat<int_IMCE_MULTH, IMCE_VMULTH_INST>;
// def : Pat<(add VGPROpnd:$rs1, VGPROpnd:$rs2), (Pseudo_IMCE_VADD_INST_all_lane VGPROpnd:$rs1, VGPROpnd:$rs2)>;
// def : Pat<(add SGPROpnd:$rs1, SGPROpnd:$rs2), (Pseudo_IMCE_VADD_INST_one_lane SGPROpnd:$rs1, SGPROpnd:$rs2)>;
def : Pat<(add (v16i16 VGPROpnd:$rs1), (v16i16 VGPROpnd:$rs2)), (IMCE_VADD_INST (v16i16 VGPROpnd:$rs1), (v16i16 VGPROpnd:$rs2), 15)>;
def : Pat<(add (i16 VGPROpnd:$rs1), (i16 VGPROpnd:$rs2)), (IMCE_VADD_INST (i16 VGPROpnd:$rs1), (i16 VGPROpnd:$rs2), 0)>;
// def : Pat<(add (i16 SGPROpnd:$rs1), (i16 SGPROpnd:$rs2)), (IMCE_VADD_INST (i16 VGPROpnd:$rs1), (i16 VGPROpnd:$rs2), 0)>;

// I type pattern
class IPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op VGPROpnd:$rs1, simm14:$imm), (inst VGPROpnd:$rs1, simm14:$imm)>;
def : IPat<int_IMCE_ADDI  , IMCE_VADDI_INST>;
def : IPat<int_IMCE_SUBI  , IMCE_VSUBI_INST>;
def : IPat<int_IMCE_ANDI  , IMCE_VANDI_INST>;
def : IPat<int_IMCE_ORI   , IMCE_VORI_INST>;
def : IPat<int_IMCE_XORI  , IMCE_VXORI_INST>;
def : IPat<int_IMCE_SRLI  , IMCE_VSRLI_INST>;
def : IPat<int_IMCE_SLLI  , IMCE_VSLLI_INST>;
def : IPat<int_IMCE_SRAI  , IMCE_VSRAI_INST>;
def : IPat<int_IMCE_MAXI  , IMCE_VMAXI_INST>;
def : IPat<int_IMCE_MINI  , IMCE_VMINI_INST>;
def : IPat<int_IMCE_MULTLI, IMCE_VMULTLI_INST>;
def : IPat<int_IMCE_MULTHI, IMCE_VMULTHI_INST>;
def : Pat<(v16i16 (build_vector (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0), (i16 0))),
          (v16i16 (IMCE_VADDI_INST (i16 V0), (i16 0)))>;

// A type pattern
class APat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op VGPROpnd:$rs1, uimm3:$shift_amt, uimm1:$dwresult_valid, uimm4:$src_mask, uimm2:$bshr_sel), (inst VGPROpnd:$rs1, uimm3:$shift_amt, uimm1:$dwresult_valid, uimm4:$src_mask, uimm2:$bshr_sel)>;
def : APat<int_IMCE_DWCONV, IMCE_DWCONV_INST>;

// B type pattern
// class BPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op simm6:$imm2, HWLOOPREGOpnd:$rs1, uimm14:$imm), (inst simm6:$imm2, HWLOOPREGOpnd:$rs1, uimm14:$imm)>;
// def : BPat<int_IMCE_BNE, IMCE_BNE_INST>;

// C type pattern
// class CPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op uimm6:$imm2, VGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value), (inst uimm6:$imm2, VGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value)>;
def : Pat<(int_IMCE_SEND uimm6:$imm2, VGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value), (IMCE_SEND_INST uimm6:$imm2, VGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value)>;
def : Pat<(int_IMCE_RECV uimm6:$fifo_id), (IMCE_RECV_INST uimm6:$fifo_id)>;
def : Pat<(int_IMCE_SETFLAG VGPROpnd:$rs1), (IMCE_SET_FLAG_INST VGPROpnd:$rs1)>;
def : Pat<(int_IMCE_STANDBY uimm6:$reg_id, VGPROpnd:$rs1), (IMCE_STANDBY_INST uimm6:$reg_id, VGPROpnd:$rs1)>;
def : Pat<(int_IMCE_LOAD_LB uimm6:$fifo_id), (IMCE_LOAD_LB uimm6:$fifo_id)>;

// P type Pattern
class PPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op uimm1:$ksel, uimm4:$src_mask, uimm2:$bshr_sel), (inst uimm1:$ksel, uimm4:$src_mask, uimm2:$bshr_sel)>;
def : PPat<int_IMCE_MAXPOOL, IMCE_MAXPOOL_INST>;
def : PPat<int_IMCE_AVGPOOL, IMCE_AVGPOOL_INST>;

// Q type Pattern
class QPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op VGPROpnd:$rs1, VGPROpnd:$rs2, uimm4:$src_mask, uimm4:$qreg_offset), (inst VGPROpnd:$rs1, VGPROpnd:$rs2, uimm4:$src_mask, uimm4:$qreg_offset)>;
def : QPat<int_IMCE_ADDQ    , IMCE_ADDQ_INST>;
def : QPat<int_IMCE_SUBQ    , IMCE_SUBQ_INST>;
def : QPat<int_IMCE_MULTLQ  , IMCE_MULTLQ_INST>;
def : QPat<int_IMCE_MULTHQ  , IMCE_MULTHQ_INST>;
def : QPat<int_IMCE_NU_QUANT, IMCE_NU_QUANT_INST>;
def : QPat<int_IMCE_MM_QUANT, IMCE_MM_QUANT_INST>;

// X type Pattern
class XPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op), (inst)>;
def : XPat<int_IMCE_STEP, IMCE_STEP_INST>;
def : XPat<int_IMCE_NOP , IMCE_NOP_INST>;
def : XPat<int_IMCE_STOP, IMCE_STOP_INST>;

// J type pattern
// class JPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op jumpOperand:$imm), (inst jumpOperand:$imm)>;
// def : JPat<int_IMCE_JUMP, IMCE_JMP_INST>;
def : Pat<(br (bb:$imm)), (IMCE_JMP_INST jumpOperand:$imm)>;

// S type Pattern
class SPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op VGPROpnd:$rs1), (inst VGPROpnd:$rs1)>;
def : SPat<int_IMCE_SCAN_RW, IMCE_SCAN_RW_INST>;

def i32MR : PatLeaf<(i32 HWLOOPREGOpnd:$R)>;
// def : Pat<(i32 (IMCECloopBeginValue i32MR:$op0, uimm16:$meta)),
//           (i32 (CLOOP_BEGIN_VALUE i32MR:$op0, uimm16:$meta))>;
def : Pat<(i32 (IMCECloopBeginValue uimm14:$op0, uimm16:$meta)),
          (i32 (CLOOP_BEGIN_VALUE uimm14:$op0, uimm16:$meta))>;
def : Pat<(i32 (IMCECloopEndValue i32MR:$op0, uimm16:$meta)),
          (i32 (CLOOP_END_VALUE i32MR:$op0, uimm16:$meta))>;
def : Pat<(IMCECloopBeginTerminator i32MR:$op0, uimm16:$meta),
          (CLOOP_BEGIN_TERMINATOR i32MR:$op0, uimm16:$meta)>;
def : Pat<(IMCECloopEndBranch bb:$op0, i32MR:$op1, uimm16:$meta),
          (CLOOP_END_BRANCH i32MR:$op1, bb:$op0, uimm16:$meta)>;
// def : Pat<(IMCECloopGuardBranch bb:$op0, i32MR:$op1, uimm16:$meta),
//           (CLOOP_GUARD_BRANCH i32MR:$op1, bb:$op0, uimm16:$meta)>;