//===-- IMCEMCTargetDesc.cpp - IMCE target descriptions -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "IMCEMCTargetDesc.h"
#include "IMCEInstPrinter.h"
#include "IMCEMCAsmInfo.h"
#include "TargetInfo/IMCETargetInfo.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "IMCEGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "IMCEGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "IMCEGenRegisterInfo.inc"

static MCInstPrinter *createIMCEMCInstPrinter(const Triple &T, unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI, const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI) {
  return new IMCEInstPrinter(MAI, MII, MRI);
}

static MCAsmInfo *createIMCEMCAsmInfo(const MCRegisterInfo &MRI, const Triple &TT,
                                      const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new IMCEMCAsmInfo(TT);
  return MAI;
}

static MCInstrInfo *createIMCEMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitIMCEMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createIMCEMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitIMCEMCRegisterInfo(X, IMCE::V1);
  return X;
}

static MCSubtargetInfo *createIMCEMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  return createIMCEMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeIMCETargetMC() {
  TargetRegistry::RegisterMCAsmInfo(getTheIMCETarget(), createIMCEMCAsmInfo);
  TargetRegistry::RegisterMCCodeEmitter(getTheIMCETarget(), createIMCEMCCodeEmitter);
  TargetRegistry::RegisterMCInstrInfo(getTheIMCETarget(), createIMCEMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(getTheIMCETarget(), createIMCEMCRegisterInfo);
  TargetRegistry::RegisterMCSubtargetInfo(getTheIMCETarget(), createIMCEMCSubtargetInfo);
  TargetRegistry::RegisterMCInstPrinter(getTheIMCETarget(), createIMCEMCInstPrinter);
  TargetRegistry::RegisterMCAsmBackend(getTheIMCETarget(), createIMCEAsmBackend);
}
