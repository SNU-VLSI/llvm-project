//===-- IMCERegisterInfo.td - IMCE Register definitions ----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

class IMCEReg<bits<5> Enc, string n> : Register<n> {
  let HWEncoding{15-5} = 0;
  let HWEncoding{4-0} = Enc;
  let Namespace = "IMCE";
}

// ==============================================================
// Register classes
// ==============================================================
class IMCERegisterClass<list<ValueType> types, int size, int alignment,
                        dag regList, int copycost = 1>
  : RegisterClass<"IMCE", types, alignment, regList> {
      let Size = size;
      let CopyCost = copycost;
}

class VRegClass<list<ValueType> regTypes, dag regList, int size>
  : RegisterClass<"IMCE",
                  regTypes,
                  16,
                  regList> {
  int Size = size;
}

class SRegClass<list<ValueType> regTypes, dag regList, int size>
  : RegisterClass<"IMCE",
                  regTypes,
                  4,
                  regList> {
  int Size = size;
}


// ============================================
// register operand
// ============================================
class IMCERegisterOperand<RegisterClass RC> : RegisterOperand<RC> {
  let DecoderMethod = "decode"#RC#"RegisterClass";
}

// ============================================
// General Purpose Scalar Registers
// ============================================
foreach I = 0-31 in {
  let isConstant = !eq(I, 0) in
    def S#I : IMCEReg<I, "s"#I>;
}

def SGPR : SRegClass<[i32], (add (sequence "S%u", 0, 3)), 128>;

def SGPROpnd : IMCERegisterOperand<SGPR> {
  let GIZeroRegister = S0;
}

// ============================================
// General-purpose vector registers
// ============================================
foreach I = 0-31 in {
  let isConstant = !eq(I, 0) in
    def V#I : IMCEReg<I, "r"#I>;
}

def VGPR : VRegClass<[v16i16], (add (sequence "V%u", 0, 31)), 256>;

def VGPROpnd : IMCERegisterOperand<VGPR> {
  let GIZeroRegister = V0;
}