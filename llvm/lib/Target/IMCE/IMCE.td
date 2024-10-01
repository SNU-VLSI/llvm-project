//===-- IMCE.td - Describe the M88000 Target Machine -------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "IMCERegisterInfo.td"
include "IMCECallingConv.td"
include "IMCEInstrFormats.td"
include "IMCEInstrInfo.td"

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def IMCEInstrInfo : InstrInfo;
def IMCEAsmParser : AsmParser;
def IMCEAsmParserVariant : AsmParserVariant {
  let RegisterPrefix = "%";
}

def IMCEAsmWriter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  int PassSubtarget = 1;
  int Variant = 0;
}

def : ProcessorModel<"generic",  NoSchedModel, []>;

def IMCE : Target {
  let InstructionSet = IMCEInstrInfo;
  let AssemblyParsers  = [IMCEAsmParser];
  let AssemblyParserVariants = [IMCEAsmParserVariant];
  let AssemblyWriters = [IMCEAsmWriter];
}
