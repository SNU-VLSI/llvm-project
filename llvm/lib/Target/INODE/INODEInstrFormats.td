//===-- INODEInstrFormats.td - INODE Instruction Formats -----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

class InstINODE<dag outs, dag ins, string asm, string operands, list<dag> pattern = []> : Instruction {
  bits<32> Inst;
  bits<32> SoftFail = 0; // Needed by disassembler.
  let Namespace = "INODE";
  let Size = 4;
  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = !if(!eq(operands, ""), asm, !strconcat(asm, " ", operands));
  let Pattern = pattern;
  let DecoderNamespace = "INODE";
}

class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern> : InstINODE<outs, ins, "", "", pattern> {
  let isCodeGenOnly       = 1;
  // let isCodeGenOnly       = 0;
  let isPseudo            = 1;
  let hasNoSchedulingInfo = 1;
  let AsmString           = asmstr;
}

class InstIType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []> : InstINODE<outs, ins, asm, operands, pattern> {
  bits<3> rd;
  bits<3> rs1;
  bits<20> imm;

  let Inst{5-0} = op;
  let Inst{8-6} = rd;
  let Inst{11-9} = rs1;
  let Inst{31-12} = imm;
}

class InstBType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstINODE<outs, ins, asm, operands, pattern> {
  bits<3> rs1;
  bits<3> rs2;
  bits<20> imm;

  let Inst{5-0} = op;
  let Inst{8-6} = rs1;
  let Inst{11-9} = rs2;
  let Inst{31-12} = imm;
}

class InstBIType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstINODE<outs, ins, asm, operands, pattern> {
  bits<3> rs1;
  bits<14> imm;
  bits<9> offset;

  let Inst{5-0} = op;
  let Inst{8-6} = rs1;
  let Inst{22-9} = imm;
  let Inst{31-23} = offset;
}

class InstXType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstINODE<outs, ins, asm, operands, pattern> {

  bits <26> intr_id;

  let Inst{5-0} = op;
  let Inst{31-6} = intr_id;
}

class InstJType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstINODE<outs, ins, asm, operands, pattern> {
  bits<26> imm;
  let Inst{5-0} = op;
  let Inst{31-6} = imm;
}

class InstCType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstINODE<outs, ins, asm, operands, pattern> {

  bits<3> rs1;
  bits<13> imm;
  bits<6> policy;
  bits<3> fifo_id;

  let Inst{5-0} = op;
  let Inst{8-6} = rs1;
  let Inst{21-9} = imm;
  let Inst{27-22} = policy;
  let Inst{30-28} = fifo_id;
  let Inst{31} = 0;
}

class InstSType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstINODE<outs, ins, asm, operands, pattern> {
  bits<7> rs1;
  bits<3> flag_value_rs;

  let Inst{5-0} = op;
  let Inst{12-6} = rs1;
  let Inst{15-13} = flag_value_rs;
  let Inst{31-16} = 0;
}

class InstUType<int op, dag outs, dag ins, string asm, string operands, list<dag> pattern = []>
   : InstINODE<outs, ins, asm, operands, pattern> {

  bits<6> entry_addr;
  bits<4> node_col_id;
  bits<3> rs1;
  bits<13> imm;

  let Inst{5-0} = op;
  let Inst{11-6} = entry_addr;
  let Inst{15-12} = node_col_id;
  let Inst{18-16} = rs1;
  let Inst{31-19} = imm;
}