//===-- IMCEInstrFormats.td - IMCE Instruction Formats -----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

class InstIMCE<dag outs, dag ins, string asm, string operands,
               list<dag> pattern = []>
  : Instruction {
  bits<32> Inst;

  bits<32> SoftFail = 0; // Needed by disassembler.

  let Namespace = "IMCE";
  let Size = 4;

  dag OutOperandList = outs;
  dag InOperandList = ins;
  // Construct the assembler string from template parameters asm and operands.
  // If the instruction has a delay slot, then append ".n" to the mnemonic.
  let AsmString = !if(!eq(operands, ""), asm,
                      !strconcat(asm, " ", operands));
  let Pattern = pattern;

  let DecoderNamespace = "IMCE";
}

class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
  : InstIMCE<outs, ins, "", "", pattern> {
  // let isCodeGenOnly       = 1;
  let isCodeGenOnly       = 0;
  let isPseudo            = 1;
  let hasNoSchedulingInfo = 1;
  let AsmString           = asmstr;
}

class InstRType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstIMCE<outs, ins, asm, operands, pattern> {
  bits<6> rd;
  bits<6> rs1;
  bits<6> rs2;
  bits<4> src_mask;

  let Inst{5-0} = op;
  let Inst{11-6} = rd;
  let Inst{17-12} = rs1;
  let Inst{23-18} = rs2;
  let Inst{27-24} = src_mask;
}

class InstAType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstIMCE<outs, ins, asm, operands, pattern> {
  bits<6> rd;
  bits<6> rs1;
  bits<3> shift_amt;
  bits<1> dwresult_valid;
  bits<4> src_mask;
  bits<2> bshr_sel;

  let Inst{5-0} = op;
  let Inst{11-6} = rd;
  let Inst{17-12} = rs1;
  let Inst{20-18} = shift_amt;
  let Inst{21} = dwresult_valid;
  let Inst{27-24} = src_mask;
  let Inst{29-28} = bshr_sel;
}

class InstPType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstIMCE<outs, ins, asm, operands, pattern> {
  bits<6> rd;
  bits<1> ksel;
  bits<4> src_mask;
  bits<2> bshr_sel;

  let Inst{5-0} = op;
  let Inst{11-6} = rd;
  let Inst{12} = ksel;
  let Inst{27-24} = src_mask;
  let Inst{29-28} = bshr_sel;
}

class InstQType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstIMCE<outs, ins, asm, operands, pattern> {
  bits<6> rs1;
  bits<6> rs2;
  bits<4> src_mask;
  bits<4> qreg_offset;

  let Inst{5-0} = op;
  let Inst{17-12} = rs1;
  let Inst{23-18} = rs2;
  let Inst{27-24} = src_mask;
  let Inst{31-28} = qreg_offset;
}

class InstIType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstIMCE<outs, ins, asm, operands, pattern> {
  bits<6> rd;
  bits<6> rs1;
  bits<14> imm;

  let Inst{5-0} = op;
  let Inst{11-6} = rd;
  let Inst{17-12} = rs1;
  let Inst{31-18} = imm;
}

class InstBType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstIMCE<outs, ins, asm, operands, pattern> {
  bits<6> imm2;
  bits<6> rs1;
  bits<14> imm;

  let Inst{5-0} = op;
  let Inst{11-6} = imm2;
  let Inst{17-12} = rs1;
  let Inst{31-18} = imm;
}

class InstXType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstIMCE<outs, ins, asm, operands, pattern> {

  let Inst{5-0} = op;
}

class InstJType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstIMCE<outs, ins, asm, operands, pattern> {
  bits<26> imm;
  let Inst{5-0} = op;
  let Inst{31-6} = imm;
}

class InstCType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstIMCE<outs, ins, asm, operands, pattern> {

  bits<6> imm2;
  bits<6> rs1;
  bits<6> fifo_id;
  bits<8> flag_value;

  let Inst{5-0} = op;
  let Inst{11-6} = imm2;
  let Inst{17-12} = rs1;
  let Inst{23-18} = fifo_id;
  let Inst{31-24} = flag_value;
}

class InstSType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstIMCE<outs, ins, asm, operands, pattern> {
  bits<6> rd;
  bits<6> rs1;
  
  let Inst{5-0} = op;
  let Inst{11-6} = rd;
  let Inst{17-12} = rs1;
}