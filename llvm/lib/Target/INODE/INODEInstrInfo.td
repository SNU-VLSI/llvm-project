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
def SDT_CallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def SDT_CallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def SDT_Call         : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;
def SDT_INODEBrCC    : SDTypeProfile<0, 4, [SDTCisSameAs<0, 1>,
                                            SDTCisVT<2, OtherVT>,
                                            SDTCisVT<3, OtherVT>]>;
def SDT_INODESelectCC : SDTypeProfile<1, 5, [SDTCisSameAs<1, 2>,
                                             SDTCisVT<3, OtherVT>,
                                             SDTCisSameAs<0, 4>,
                                             SDTCisSameAs<4, 5>]>;

// Selection DAG nodes.
def call             : SDNode<"INODEISD::CALL", SDT_Call,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                              SDNPVariadic]>;
def retglue          : SDNode<"INODEISD::RET_GLUE", SDTNone,
                              [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def INODECloopBeginValue :
  SDNode<"INODEISD::CLOOP_BEGIN_VALUE",
         SDTypeProfile<1, 2, [SDTCisVT<0, i32>, SDTCisVT<1, i32>, SDTCisVT<2, i32>]>, // loop_indvar_reg, loop_bound_constant, meta_id 
         [SDNPHasChain]>;
def INODECloopBeginTerminator :
  SDNode<"INODEISD::CLOOP_BEGIN_TERMINATOR",
         SDTypeProfile<0, 1, [SDTCisVT<0, i32>]>, // loop_indvar_reg
         [SDNPHasChain]>;
def INODECloopEndValue :
  SDNode<"INODEISD::CLOOP_END_VALUE",
         SDTypeProfile<1, 1, [SDTCisVT<0, i32>, SDTCisVT<1, i32>]>, //loop_indvar_reg, loop_indvar_reg
         [SDNPHasChain]>;
def INODECloopEndBranch :
  SDNode<"INODEISD::CLOOP_END_BRANCH",
         SDTypeProfile<0, 2, [SDTCisVT<1, i32>]>, // br_target, loop_indvar
         [SDNPHasChain]>;

// ===================================================
// operands
// ===================================================

// register operand
class INODERegisterOperand<RegisterClass RC> : RegisterOperand<RC> {
  let DecoderMethod = "decodeRegisterClass";
}
def SGPROpnd : INODERegisterOperand<SGPR> {
  let GIZeroRegister = SReg0;
}
def AddrCntRegOpnd : INODERegisterOperand<AddrCnt>;

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

def ixlenimm : Operand<i32>;

def ImmZeroAsmOperand : AsmOperandClass {
  let Name = "ImmZero";
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

def immzero : Operand<i32>,
              TImmLeaf<i32, [{return (Imm == 0);}]> {
  let ParserMatchClass = ImmZeroAsmOperand;
  let OperandType = "OPERAND_ZERO";
  let OperandNamespace = "INODEOp";
}

def uimm1 : Operand<i32>, TImmLeaf<i32, [{return isUInt<1>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<1>;
  let DecoderMethod = "decodeUImmOperand<1>";
  let OperandType = "OPERAND_UIMM1";
  let OperandNamespace = "INODEOp";
}

def uimm2 : Operand<i32>, TImmLeaf<i32, [{return isUInt<2>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<2>;
  let DecoderMethod = "decodeUImmOperand<2>";
  let OperandType = "OPERAND_UIMM2";
  let OperandNamespace = "INODEOp";
}

def uimm3 : Operand<i32>, TImmLeaf<i32, [{return isUInt<3>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<3>;
  let DecoderMethod = "decodeUImmOperand<3>";
  let OperandType = "OPERAND_UIMM3";
  let OperandNamespace = "INODEOp";
}

def uimm4 : Operand<i32>, TImmLeaf<i32, [{return isUInt<4>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<4>;
  let DecoderMethod = "decodeUImmOperand<4>";
  let OperandType = "OPERAND_UIMM4";
  let OperandNamespace = "INODEOp";
}

def uimm4_allone : Operand<i32>, TImmLeaf<i32, [{return (imm == 15);}]> {
  let ParserMatchClass = UImmAsmOperand<4>;
  let DecoderMethod = "decodeUImmOperand<4>";
  let OperandType = "OPERAND_UIMM4_ALLONE";
  let OperandNamespace = "INODEOp";
}

def uimm6 : Operand<i32>, TImmLeaf<i32, [{return isUInt<6>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<6>;
  let DecoderMethod = "decodeUImmOperand<6>";
  let OperandType = "OPERAND_UIMM6";
  let OperandNamespace = "INODEOp";
}

def uimm7 : Operand<i32>, TImmLeaf<i32, [{return isUInt<7>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<7>;
  let DecoderMethod = "decodeUImmOperand<7>";
  let OperandType = "OPERAND_UIMM7";
  let OperandNamespace = "INODEOp";
}

def uimm8 : Operand<i32>, TImmLeaf<i32, [{return isUInt<8>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<8>;
  let DecoderMethod = "decodeUImmOperand<8>";
  let OperandType = "OPERAND_UIMM8";
  let OperandNamespace = "INODEOp";
}

def uimm12 : Operand<i32>, TImmLeaf<i32, [{return isUInt<12>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<12>;
  let DecoderMethod = "decodeUImmOperand<12>";
  let OperandType = "OPERAND_UIMM12";
  let OperandNamespace = "INODEOp";
}

def uimm13 : Operand<i32>, TImmLeaf<i32, [{return isUInt<13>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<13>;
  let DecoderMethod = "decodeUImmOperand<13>";
  let OperandType = "OPERAND_UIMM13";
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

def uimm26 : Operand<i32>, TImmLeaf<i32, [{return isUInt<26>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<26>;
  let DecoderMethod = "decodeUImmOperand<26>";
  let OperandType = "OPERAND_UIMM26";
  let OperandNamespace = "INODEOp";
}

def uimm32 : Operand<i32>, TImmLeaf<i32, [{return isUInt<32>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<32>;
  let DecoderMethod = "decodeUImmOperand<32>";
  let OperandType = "OPERAND_UIMM32";
  let OperandNamespace = "INODEOp";
}

def simm13 : Operand<i32>, TImmLeaf<i32, [{return isInt<13>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<13>;
  let DecoderMethod = "decodeSImmOperand<13>";
  let OperandType = "OPERAND_SIMM13";
  let OperandNamespace = "INODEOp";
}

def simm14 : Operand<i32>, TImmLeaf<i32, [{return isInt<14>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<14>;
  let DecoderMethod = "decodeSImmOperand<14>";
  let OperandType = "OPERAND_SIMM14";
  let OperandNamespace = "INODEOp";
}

def ntsimm14 : Operand<i32>, ImmLeaf<i32, [{return isInt<14>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<14>;
  let DecoderMethod = "decodeSImmOperand<14>";
  let OperandType = "OPERAND_SIMM14";
  let OperandNamespace = "INODEOp";
}

def ntsimm20 : Operand<i32>, ImmLeaf<i32, [{return isInt<20>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<20>;
  let DecoderMethod = "decodeSImmOperand<20>";
  let OperandType = "OPERAND_SIMM20";
  let OperandNamespace = "INODEOp";
}

def ntsimm_over20 : Operand<i32>, ImmLeaf<i32, [{return isInt<32>(Imm) & !isInt<20>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<20, "over">;
  let DecoderMethod = "decodeSImmOperand<32>";
  let OperandType = "OPERAND_SIMM32";
  let OperandNamespace = "INODEOp";
}

def simm20 : Operand<i32>, TImmLeaf<i32, [{return isInt<20>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<20>;
  let DecoderMethod = "decodeSImmOperand<20>";
  let OperandType = "OPERAND_SIMM20";
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

def brtarget9 : Operand<OtherVT> {
  let EncoderMethod = "getBranchTargetOpValue9";
  let OperandType = "OPERAND_PCREL";
  let DecoderMethod = "decodeBranchTarget";
  let ParserMatchClass = JumpTargetAsmOperand;
  let PrintMethod = "printBranchOperand";
}

// ========================================
// SDNodes
// ========================================
def inode_brcc      : SDNode<"INODEISD::BR_CC", SDT_INODEBrCC,
                             [SDNPHasChain]>;
def inode_selectcc  : SDNode<"INODEISD::SELECT_CC", SDT_INODESelectCC>;

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
    def CLOOP_BEGIN_VALUE : Pseudo<(outs SGPROpnd:$loop_indvar), (ins uimm32:$loop_cnt, uimm32:$meta_id),
                               "# CLOOP_BEGIN_VALUE $loop_indvar, $loop_cnt $meta_id",
                               []>;

    def CLOOP_END_VALUE : Pseudo<(outs SGPROpnd:$indvar_o), (ins SGPROpnd:$indvar_i),
                             "# CLOOP_END_VALUE $indvar_o, $indvar_i",
                             []>,RegConstraint<"$indvar_o = $indvar_i">;
  }

  def CLOOP_BEGIN_TERMINATOR : Pseudo<(outs),
                                      (ins SGPROpnd:$indvar),
                                      "# CLOOP_BEGIN_TERMINATOR $indvar",
                                      []> {
    let isTerminator = 1;
    let hasSideEffects = 0;
  }

  def CLOOP_END_BRANCH : Pseudo<(outs),
                            (ins SGPROpnd:$indvar, brtarget:$br_target),
                            "# CLOOP_END_BRANCH $indvar, $br_target",
                             []> {
    let isBranch = 1;
    let isTerminator = 1;
    let hasSideEffects = 1;
  }
}

// I type
class IInst<string opname, int opcode> : InstIType<opcode, (outs SGPROpnd:$rd), (ins SGPROpnd:$rs1, simm20:$imm), opname, "$rd $rs1 $imm", []>;
let mayLoad=0, mayStore=0 in {
  def INODE_ADDI_INST   : IInst<"addi"  , 0b10001>;
  def INODE_MULI_INST : IInst<"muli", 0b10010>;
}
def INODE_LUI_INST : InstIType<0b10011, (outs SGPROpnd:$rd), (ins SGPROpnd:$rs1, uimm12:$imm), "lui", "$rd $rs1 $imm", []> {
  let mayLoad = 0;
  let mayStore = 0;
}
def INODE_LOAD_INST : InstIType<0b101, (outs SGPROpnd:$rd), (ins SGPROpnd:$rs1, simm20:$imm), "load", "$rd, $rs1, $imm", []> {
  let mayLoad = 1;
  let hasSideEffects = 1;
}
def INODE_STORE_INST : InstIType<0b110, (outs), (ins SGPROpnd:$rd, simm20:$imm, SGPROpnd:$rs1), "store", "$rd, $imm, $rs1", []> {
  let mayStore = 1;
  let hasSideEffects = 1;
}

// B type
class BInst<string opname, int opcode> : InstBType<opcode, (outs), (ins SGPROpnd:$rs1, SGPROpnd:$rs2, brtarget:$imm), opname, "$rs1 $rs2 $imm", []>;
let mayLoad=0, mayStore=0, isBranch=1, isTerminator=1 in {
  def INODE_BEQ : BInst<"beq", 0b1001>;
  def INODE_BNE : BInst<"bne", 0b1010>;
  def INODE_BLT : BInst<"blt", 0b1100>;
  def INODE_BGE : BInst<"bge", 0b1101>;
}

let isBarrier = 1, isBranch = 1, hasSideEffects = 0, mayStore = 0, mayLoad = 0, isAsmParserOnly = 1, hasNoSchedulingInfo = 1 in {
  def INODE_LONG_BEQ : Pseudo<(outs), (ins SGPROpnd:$rs1, SGPROpnd:$rs2, brtarget:$imm), "# LONG_BEQ $rs1, $rs2, $imm", []>;
  def INODE_LONG_BNE : Pseudo<(outs), (ins SGPROpnd:$rs1, SGPROpnd:$rs2, brtarget:$imm), "# LONG_BNE $rs1, $rs2, $imm", []>;
  def INODE_LONG_BLT : Pseudo<(outs), (ins SGPROpnd:$rs1, SGPROpnd:$rs2, brtarget:$imm), "# LONG_BLT $rs1, $rs2, $imm", []>;
  def INODE_LONG_BGE : Pseudo<(outs), (ins SGPROpnd:$rs1, SGPROpnd:$rs2, brtarget:$imm), "# LONG_BGE $rs1, $rs2, $imm", []>;
}

class BIInst<string opname, int opcode> : InstBIType<opcode, (outs SGPROpnd:$indvar_o), (ins SGPROpnd:$rs1, uimm14:$imm, brtarget9:$offset), opname, "$rs1 $imm $offset", []>;
def INODE_BNE_UPDATE_INST : BIInst<"bne_update", 0b1011> {
  let mayLoad=0;
  let mayStore=0;
  let isBranch = 1;
  let isTerminator = 1;
  let Constraints = "$indvar_o = $rs1";
}
def INODE_LONG_BNE_UPDATE : Pseudo<(outs SGPROpnd:$indvar_o), (ins SGPROpnd:$indvar_i, uimm14:$loop_cnt, brtarget9:$imm),
                          "# LONG_BNE_UPDATE $indvar_i, $loop_cnt, $imm", []> {
  let isBarrier = 1;
  let isBranch = 1;
  let hasSideEffects = 0;
  let mayStore = 0;
  let mayLoad = 0;
  let isAsmParserOnly = 1;
  let hasNoSchedulingInfo = 1;
}

// C type
class CCommInst<string opname, int opcode> : InstCType<opcode, (outs), (ins SGPROpnd:$rs1, simm13:$imm, uimm6:$policy, uimm3:$fifo_id), opname, "$rs1 $imm $policy, $fifo_id", []>;
let hasSideEffects=1, mayLoad=1 in {
  def INODE_SEND_INST : CCommInst<"send", 0b01>;
}

let hasSideEffects=1, mayStore=1 in {
  def INODE_RECV_INST : CCommInst<"recv", 0b10>;
}

let hasSideEffects=1 in {
  def INODE_LAYERINIT_INST : InstCType<0b11111, (outs), (ins), "layerinit", "", []> {
    let rs1 = 0;
    let imm = 0;
    let policy = 0;
    let fifo_id = 0; 
  }
}

let hasSideEffects=1 in {
  def INODE_IMCE_COMPUTE_INST : InstCType<0b100000, (outs), (ins uimm13:$imm), "imce_compute", "$imm", []> {
    let rs1 = 0;
    let policy = 0;
    let fifo_id = 0; 
  }
}

class CConfigInst<string opname, int opcode> : InstCType<opcode, (outs), (ins SGPROpnd:$rs1, simm13:$imm, uimm6:$policy), opname, "$rs1 $imm $policy", []> {
  let Uses = [AddrCntReg];
  let Defs = [AddrCntReg];
  let fifo_id = 0;
}

let hasSideEffects=1, mayLoad=1 in {
  def INODE_WR_IMEM_INST : CConfigInst<"wr_imem", 0b10101>;
  def INODE_WR_IMCU_INST : CConfigInst<"wr_imcu", 0b11001>;
  def INODE_WR_REG_INST : CConfigInst<"wr_reg", 0b10111>;

}

let hasSideEffects=1 in {
  def INODE_SET_ADDR_CNT_INST : InstCType<0b10100, (outs), (ins uimm13:$imm), "set_addr_cnt", "$imm", []> {
    let rs1 = 0;
    let policy = 0;
    let fifo_id = 0; 
    let Defs = [AddrCntReg];
  }
}

let hasSideEffects=1 in {
  def INODE_SET_FLAG_INST : InstSType<0b011, (outs), (ins SGPROpnd:$flag_value_rs), "set_flag", "$flag_value_rs", []> {
    let rs1 = 0;
  }

  def INODE_STANDBY_INST  : InstSType<0b100, (outs), (ins uimm7:$rs1, SGPROpnd:$flag_value_rs), "set_flag", "$rs1, $flag_value_rs", []>;
}

// X type
class XNoOpInst<string opname, int opcode> : InstXType<opcode, (outs), (ins), opname, "", []> {
  let intr_id = 0;
  let hasSideEffects = 1;
}
def INODE_DONE_INST : XNoOpInst<"done", 0b100001>;
def INODE_HALT_INST : XNoOpInst<"halt", 0b100010>;

def INODE_INTRT_INST : InstXType<0b111101, (outs), (ins uimm26:$intr_id), "intrt", "$intr_id", []> {
  let hasSideEffects = 1;
}

// J type
def INODE_JMP_INST : InstJType<0b11100, (outs), (ins jumptarget:$imm), "jmp", "$imm", []> {
  let isTerminator=1;
  let isBarrier=1;
}

// U type
def INODE_PU_INST : InstUType<0b11011, (outs), (ins SGPROpnd:$rs1, simm13:$imm, SGPROpnd:$entry_addr, uimm4:$node_col_id), "pu", "$rs1 $imm $entry_addr $node_col_id", []>;

// ================================================
// Patterns
// ================================================
// I type pattern
// LUI selected from custom isel
//TODO: implement SelectAddrRegImm and SelectConstant
def ConvertToTargetImm : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant(N->getSExtValue(), SDLoc(N), N->getValueType(0));
}]>;
def : Pat<(add SGPROpnd:$rs1, simm20:$imm), (INODE_ADDI_INST SGPROpnd:$rs1, simm20:$imm)>;
def : Pat<(mul SGPROpnd:$rs1, simm20:$imm), (INODE_MULI_INST SGPROpnd:$rs1, simm20:$imm)>;
def : Pat<(add SGPROpnd:$rs1, ntsimm20:$imm), (INODE_ADDI_INST SGPROpnd:$rs1, (ConvertToTargetImm ntsimm20:$imm))>;
def : Pat<(mul SGPROpnd:$rs1, ntsimm20:$imm), (INODE_MULI_INST SGPROpnd:$rs1, (ConvertToTargetImm ntsimm20:$imm))>;
// def : Pat<(add SGPROpnd:$rs1, ntsimm_over20:$imm), (INODE_ADDI_INST SGPROpnd:$rs1, (ConvertToTargetImm ntsimm_over20:$imm))>;
// def : Pat<(mul SGPROpnd:$rs1, ntsimm_over20:$imm), (INODE_MULI_INST SGPROpnd:$rs1, (ConvertToTargetImm ntsimm_over20:$imm))>;

def AddrRegImm : ComplexPattern<iPTR, 2, "SelectAddrRegImm">;
class LdPat<PatFrag LoadOp, InstINODE Inst>
    : Pat<(i32 (LoadOp (AddrRegImm (i32 SGPROpnd:$rs1), simm20:$imm20))),
          (Inst SGPROpnd:$rs1, simm20:$imm20)>;
def : LdPat<load, INODE_LOAD_INST>;

class StPat<PatFrag StoreOp, InstINODE Inst>
    : Pat<(StoreOp (i32 SGPROpnd:$rs1), (AddrRegImm (i32 SGPROpnd:$rd), simm20:$imm20)),
          (Inst SGPROpnd:$rd, simm20:$imm20, SGPROpnd:$rs1)>;
def : StPat<store, INODE_STORE_INST>;

// B type pattern
multiclass BccPat<CondCode Cond, InstINODE Inst> {
  def : Pat<(inode_brcc (i32 SGPROpnd:$rs1), SGPROpnd:$rs2, Cond, bb:$imm20),
            (Inst SGPROpnd:$rs1, SGPROpnd:$rs2, brtarget:$imm20)>;
  // Explicitly select 0 to X0. The register coalescer doesn't always do it.
  def : Pat<(inode_brcc (i32 SGPROpnd:$rs1), 0, Cond, bb:$imm20),
            (Inst SGPROpnd:$rs1, (i32 SReg0), brtarget:$imm20)>;
}

defm : BccPat<SETEQ, INODE_BEQ>;
defm : BccPat<SETNE, INODE_BNE>;
defm : BccPat<SETLT, INODE_BLT>;
defm : BccPat<SETGE, INODE_BGE>;

def IntCCtoINODECC : SDNodeXForm<inode_selectcc, [{
  ISD::CondCode CC = cast<CondCodeSDNode>(N->getOperand(2))->get();
  INODECC::CondCode BrCC = getINODECCForIntCC(CC);
  return CurDAG->getTargetConstant(BrCC, SDLoc(N), MVT::i32);
}]>;

def inode_selectcc_frag : PatFrag<(ops node:$lhs, node:$rhs, node:$cc,
                                       node:$truev, node:$falsev),
                                  (inode_selectcc node:$lhs, node:$rhs,
                                                  node:$cc, node:$truev,
                                                  node:$falsev), [{}],
                                  IntCCtoINODECC>;

multiclass SelectCC_GPR_rrirr<DAGOperand valty, ValueType vt> {
  let usesCustomInserter = 1 in
  def _Using_CC_GPR : Pseudo<(outs valty:$dst),
                             (ins SGPR:$lhs, SGPR:$rhs, ixlenimm:$cc,
                              valty:$truev, valty:$falsev),
                             "",
                             [(set valty:$dst,
                               (inode_selectcc_frag:$cc (i32 SGPR:$lhs), SGPR:$rhs, cond,
                                                        (vt valty:$truev), valty:$falsev))]>;
  // Explicitly select 0 in the condition to X0. The register coalescer doesn't
  // always do it.
  def : Pat<(inode_selectcc_frag:$cc (i32 SGPR:$lhs), 0, cond, (vt valty:$truev),
                                     valty:$falsev),
            (!cast<Instruction>(NAME#"_Using_CC_GPR") SGPR:$lhs, (i32 SReg0),
             (IntCCtoINODECC $cc), valty:$truev, valty:$falsev)>;
}

defm Select_GPR : SelectCC_GPR_rrirr<SGPR, i32>;

// BI type pattern
// BNE_UPDATE is selected from custom isel

// C type pattern
// class CPat<SDPatternOperator intrin_op, Instruction inst> : Pat<(intrin_op uimm6:$imm2, SGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value), (inst uimm6:$imm2, SGPROpnd:$rs1, uimm6:$fifo_id, uimm8:$flag_value)>;
def : Pat<(int_INODE_SEND SGPROpnd:$rs1, simm13:$imm, uimm6:$policy, uimm3:$fifo_id), (INODE_SEND_INST SGPROpnd:$rs1, simm13:$imm, uimm6:$policy, uimm3:$fifo_id)>;
def : Pat<(int_INODE_RECV SGPROpnd:$rs1, simm13:$imm, uimm6:$policy, uimm3:$fifo_id), (INODE_RECV_INST SGPROpnd:$rs1, simm13:$imm, uimm6:$policy, uimm3:$fifo_id)>;

def : Pat<(int_INODE_LAYERINIT), (INODE_LAYERINIT_INST)>;
def : Pat<(int_INODE_IMCE_COMPUTE uimm13:$imm), (INODE_IMCE_COMPUTE_INST uimm13:$imm)>;

def : Pat<(int_INODE_WR_IMEM SGPROpnd:$rs1, simm13:$imm, uimm6:$policy), (INODE_WR_IMEM_INST SGPROpnd:$rs1, simm13:$imm, uimm6:$policy)>;
def : Pat<(int_INODE_WR_IMCU SGPROpnd:$rs1, simm13:$imm, uimm6:$policy), (INODE_WR_IMCU_INST SGPROpnd:$rs1, simm13:$imm, uimm6:$policy)>;
def : Pat<(int_INODE_WR_REG SGPROpnd:$rs1, simm13:$imm, uimm6:$policy), (INODE_WR_REG_INST SGPROpnd:$rs1, simm13:$imm, uimm6:$policy)>;

def : Pat<(int_INODE_SET_ADDR_CNT uimm13:$imm), (INODE_SET_ADDR_CNT_INST uimm13:$imm)>;

// S type pattern
def : Pat<(int_INODE_SET_FLAG SGPROpnd:$flag_value_rs), (INODE_SET_FLAG_INST SGPROpnd:$flag_value_rs)>;
def : Pat<(int_INODE_STANDBY uimm7:$rs1, SGPROpnd:$flag_value_rs), (INODE_STANDBY_INST uimm7:$rs1, SGPROpnd:$flag_value_rs)>;

// X type Pattern
def : Pat<(int_INODE_DONE), (INODE_DONE_INST)>;
def : Pat<(int_INODE_HALT), (INODE_HALT_INST)>;
def : Pat<(int_INODE_INTRT uimm26:$intr_id), (INODE_INTRT_INST uimm26:$intr_id)>;

// J type pattern
def : Pat<(br (bb:$imm)), (INODE_JMP_INST jumptarget:$imm)>;

// U type Pattern
def : Pat<(int_INODE_PU SGPROpnd:$rs1, simm13:$imm, SGPROpnd:$entry_addr, uimm4:$node_col_id), (INODE_PU_INST SGPROpnd:$rs1, simm13:$imm, SGPROpnd:$entry_addr, uimm4:$node_col_id)>;

def i32MR : PatLeaf<(i32 SGPROpnd:$R)>;
def : Pat<(i32 (INODECloopBeginValue uimm32:$op0, uimm32:$meta)), (i32 (CLOOP_BEGIN_VALUE uimm32:$op0, uimm32:$meta))>;
def : Pat<(INODECloopBeginTerminator i32MR:$op0), (CLOOP_BEGIN_TERMINATOR i32MR:$op0)>;
def : Pat<(i32 (INODECloopEndValue i32MR:$op0)), (i32 (CLOOP_END_VALUE i32MR:$op0))>;
def : Pat<(INODECloopEndBranch bb:$op0, i32MR:$op1), (CLOOP_END_BRANCH i32MR:$op1, bb:$op0)>;