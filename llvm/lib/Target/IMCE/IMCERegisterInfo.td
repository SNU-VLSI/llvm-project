//===-- IMCERegisterInfo.td - IMCE Register definitions ----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// ==================================================
// Registers
// ==================================================
def sub_32     : SubRegIndex<32>;

foreach I = 0-31 in {
  let isConstant = !eq(I, 0) in
    def S#I : Register<"s"#I> {
      let Namespace = "IMCE";
      let HWEncoding{15-6} = 0;
      let HWEncoding{5-0} = I;
    }
}

foreach I = 0-31 in {
  let isConstant = !eq(I, 0) in
    def V#I : RegisterWithSubRegs<"v"#I, [!cast<Register>("S"#I)]> {
      let SubRegIndices = [sub_32];
      let Namespace = "IMCE";
      let HWEncoding{15-6} = 0;
      let HWEncoding{5-0} = I;
    }
}

foreach I = 0-3 in {
  def CREG#I : Register<"creg"#I> {
    let Namespace = "IMCE";
    let HWEncoding{15-6} = 0;
    let HWEncoding{5-0} = !add(I,32);
  }
}

foreach I = 0-3 in {
  def QREG#I : Register<"qreg"#I> {
    let Namespace = "IMCE";
    let HWEncoding{15-6} = 0;
    let HWEncoding{5-0} = !add(I,36);
  }
}

foreach I=0-3 in {
  def BSHR_REG#I : Register<"bshr_reg"#I> {
    let Namespace = "IMCE";
    let HWEncoding{15-6} = 0;
    let HWEncoding{5-0} = !add(I, 40);
  }
}

def MINREG : Register<"minreg"> {
  let Namespace = "IMCE";
  let HWEncoding{15-6} = 0;
  let HWEncoding{5-0} = 44;
}

def MAXREG : Register<"maxreg"> {
  let Namespace = "IMCE";
  let HWEncoding{15-6} = 0;
  let HWEncoding{5-0} = 45;
}

def CFGREG : Register<"cfgreg"> {
  let Namespace = "IMCE";
  let HWEncoding{15-6} = 0;
  let HWEncoding{5-0} = 46;
}

def PCREG  : Register<"pcreg"> {
  let Namespace = "IMCE";
  let HWEncoding{15-6} = 0;
  let HWEncoding{5-0} = 47;
}

foreach I=0-5 in {
  def HWLOOP_REG#I : Register<"hwloop_reg"#I> {
    let Namespace = "IMCE";
    let HWEncoding{15-6} = 0;
    let HWEncoding{5-0} = !add(I, 48);
  }
}

// ==============================================================
// Register classes
// ==============================================================
def SGPR : RegisterClass<"IMCE", [i32], 32, (add (sequence "S%u", 0, 31))> {
  let Size = 32;
  let CopyCost = 1;
}

def VGPR : RegisterClass<"IMCE", [v16i16], 256, (add (sequence "V%u", 0, 31))> {
  let Size = 256;
  let CopyCost = 1;
}

def CREG_CLASS : RegisterClass<"IMCE", [v16i16], 256, (add (sequence "CREG%u", 0, 3))> {
  let Size = 256;
  let CopyCost = 1;
}

def QREG_CLASS : RegisterClass<"IMCE", [v16i16], 256, (add (sequence "QREG%u", 0, 3))> {
  let Size = 256;
  let CopyCost = 1;
}

def BSHR_REG_CLASS : RegisterClass<"IMCE", [v16i16], 256, (add (sequence "BSHR_REG%u", 0, 3))> {
  let Size = 256;
  let CopyCost = 1;
}

def HWLOOP_REG_CLASS : RegisterClass<"IMCE", [v16i16], 256, (add (sequence "HWLOOP_REG%u", 0, 5))> {
  let Size = 256;
  let CopyCost = 1;
}

def MINMAXREG_CLASS : RegisterClass<"IMCE", [v16i16], 256, (add MINREG, MAXREG)> {
  let Size = 256;
  let CopyCost = 1;
}

def CFGREG_CLASS : RegisterClass<"IMCE", [v16i16], 256, (add CFGREG)> {
  let Size = 256;
  let CopyCost = 1;
}

def PCREG_CLASS : RegisterClass<"IMCE", [v16i16], 256, (add PCREG)> {
  let Size = 256;
  let CopyCost = 1;
}