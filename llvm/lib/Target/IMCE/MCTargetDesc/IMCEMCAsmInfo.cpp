//===-- IMCEMCAsmInfo.cpp - IMCE asm properties ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "IMCEMCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCSectionELF.h"

using namespace llvm;

IMCEMCAsmInfo::IMCEMCAsmInfo(const Triple &TT) {
  // TODO: Check!
  CodePointerSize = 4;
  CalleeSaveStackSlotSize = 4;
  IsLittleEndian = true;
  UseDotAlignForAlignment = true;
  MinInstAlignment = 4;

  CommentString = "|"; // # as comment delimiter is only allowed at first column
  ZeroDirective = "\t.space\t";
  Data64bitsDirective = "\t.quad\t";
  UsesELFSectionDirectiveForBSS = true;
  SupportsDebugInformation = false;
  ExceptionsType = ExceptionHandling::SjLj;
}
