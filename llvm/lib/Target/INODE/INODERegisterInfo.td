//===-- INODERegisterInfo.td - INODE Register definitions ----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// ==================================================
// Registers
// ==================================================
class INODEReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "INODE";
}

foreach I = 0-7 in {
  let isConstant = !eq(I, 0) in
    def SReg#I : INODEReg<I, "sreg"#I>;
}

def AddrCntReg : INODEReg<0 ,"addr_cnt_reg">;

// ==============================================================
// Register classes
// ==============================================================

def SGPR : RegisterClass<"INODE", [i32], 32, (add (sequence "sreg%u", 0, 7))> {
  let Size = 32;
  let CopyCost = 1;
}

def AddrCnt : RegisterClass<"INODE", [i32], 32, (add AddrCntReg)> {
  let Size = 32;
  let CopyCost = 1;
}