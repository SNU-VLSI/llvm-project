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
class IMCEReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "IMCE";
}

foreach I = 0-31 in {
  let isConstant = !eq(I, 0) in
    def V#I : IMCEReg<I, "v"#I>;
}

foreach I = 0-3 in {
  def CREG#I : IMCEReg<!add(I, 32), "creg"#I>;
  def QREG#I : IMCEReg<!add(I, 36), "qreg"#I>;
  def BSHR#I : IMCEReg<!add(I, 40), "bshr"#I>;
}

def MINREG : IMCEReg<44 ,"minreg">;
def MAXREG : IMCEReg<45, "maxreg">;
def CFGREG : IMCEReg<46, "cfgreg">;
def PCREG  : IMCEReg<47, "pcreg">;

foreach I=0-5 in {
  def HWLR#I : IMCEReg<!add(I, 48), "hwlr"#I>;
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

def VGPR : RegisterClass<"IMCE", [v16i16, i16, i32], 256, (add
                         (sequence "V%u", 0, 31), (sequence "QREG%u", 0, 3),
                         (sequence "CREG%u", 0, 3))> {
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

def BSHR : RegisterClass<"IMCE", [v16i16], 256, (add (sequence "BSHR%u", 0, 3))> {
  let Size = 256;
  let CopyCost = 1;
}

def HWLR : RegisterClass<"IMCE", [i32], 32, (add (sequence "HWLR%u", 0, 5))> {
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
