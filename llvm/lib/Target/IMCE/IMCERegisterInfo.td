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

def sub_16_0  : SubRegIndex<16, 0>;
def sub_16_1  : SubRegIndex<16, 1>;
def sub_16_2  : SubRegIndex<16, 2>;
def sub_16_3  : SubRegIndex<16, 3>;
def sub_16_4  : SubRegIndex<16, 4>;
def sub_16_5  : SubRegIndex<16, 5>;
def sub_16_6  : SubRegIndex<16, 6>;
def sub_16_7  : SubRegIndex<16, 7>;
def sub_16_8  : SubRegIndex<16, 8>;
def sub_16_9  : SubRegIndex<16, 9>;
def sub_16_10 : SubRegIndex<16, 10>;
def sub_16_11 : SubRegIndex<16, 11>;
def sub_16_12 : SubRegIndex<16, 12>;
def sub_16_13 : SubRegIndex<16, 13>;
def sub_16_14 : SubRegIndex<16, 14>;
def sub_16_15 : SubRegIndex<16, 15>;

// foreach I = 0-31 in {
//   let isConstant = !eq(I, 0) in
//     def S#I : Register<"s"#I> {
//       let Namespace = "IMCE";
//       let HWEncoding{15-6} = 0;
//       let HWEncoding{5-0} = I;
//     }
// }

foreach I = 0-31 in {
  let isConstant = !eq(I, 0) in
    // def V#I : RegisterWithSubRegs<"v"#I, [!cast<Register>("S"#I)]> {
    def V#I : Register<"v"#I> {
      // let SubRegIndices = [sub_32];
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
  foreach J = 0-15 in {
    def QSUBREG#I#_#J : Register<"qsubreg"#I#"_"#J> {
      let Namespace = "IMCE";
      let HWEncoding{15-6} = 0;
      let HWEncoding{5-0} = !add(I,32);
    }
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

def DWREG : Register<"dwreg"> {
  let Namespace = "IMCE";
}

foreach I=0-1 in {
  def SCANREG#I : Register<"scanreg"#I> {
    let Namespace = "IMCE";
  }
}

// ==============================================================
// Register classes
// ==============================================================

def VGPR : RegisterClass<"IMCE", [v16i16, i16, i32], 256, (add (sequence "V%u", 0, 31))> {
  let Size = 256;
  let CopyCost = 1;
}

def CRegs : RegisterClass<"IMCE", [v16i16], 256, (add (sequence "CREG%u", 0, 3))> {
  let Size = 256;
  let CopyCost = 1;
}

def QRegs : RegisterClass<"IMCE", [v16i16], 256, (add (sequence "QREG%u", 0, 3))> {
  let Size = 256;
  let CopyCost = 1;
}

def BSHR : RegisterClass<"IMCE", [v16i16], 256, (add (sequence "BSHR_REG%u", 0, 3))> {
  let Size = 256;
  let CopyCost = 1;
}

def HWLR : RegisterClass<"IMCE", [i32], 256, (add (sequence "HWLOOP_REG%u", 0, 5))> {
  let Size = 32;
  let CopyCost = 1;
}

def MMRegs : RegisterClass<"IMCE", [v16i16], 256, (add MINREG, MAXREG)> {
  let Size = 256;
  let CopyCost = 1;
}

def CfgRegs : RegisterClass<"IMCE", [v16i16], 256, (add CFGREG)> {
  let Size = 256;
  let CopyCost = 1;
}

def PcRegs : RegisterClass<"IMCE", [v16i16], 256, (add PCREG)> {
  let Size = 256;
  let CopyCost = 1;
}

def DwRegs : RegisterClass<"IMCE", [v16i16], 256, (add DWREG)> {
  let Size = 256;
  let CopyCost = 1;
}

def ScanRegs : RegisterClass<"IMCE", [v16i16], 256, (add (sequence "SCANREG%u", 0, 1))> {
  let Size = 256;
  let CopyCost = 1;
}
