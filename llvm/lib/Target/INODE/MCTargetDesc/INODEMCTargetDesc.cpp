//===-- INODEMCTargetDesc.cpp - INODE target descriptions -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "INODEMCTargetDesc.h"
#include "INODEInstPrinter.h"
#include "INODEMCAsmInfo.h"
#include "TargetInfo/INODETargetInfo.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "INODEGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "INODEGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "INODEGenRegisterInfo.inc"

// VINN: May want to implement InstAnalysis as in Mips

static MCInstPrinter *createINODEMCInstPrinter(const Triple &T, unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI, const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI) {
  return new INODEInstPrinter(MAI, MII, MRI);
}

static MCAsmInfo *createINODEMCAsmInfo(const MCRegisterInfo &MRI, const Triple &TT,
                                      const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new INODEMCAsmInfo(TT);
  return MAI;
}

static MCInstrInfo *createINODEMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitINODEMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createINODEMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitINODEMCRegisterInfo(X, INODE::V1);
  return X;
}

static MCSubtargetInfo *createINODEMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  return createINODEMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeINODETargetMC() {
  TargetRegistry::RegisterMCAsmInfo(getTheINODETarget(), createINODEMCAsmInfo);
  TargetRegistry::RegisterMCCodeEmitter(getTheINODETarget(), createINODEMCCodeEmitter);
  TargetRegistry::RegisterMCInstrInfo(getTheINODETarget(), createINODEMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(getTheINODETarget(), createINODEMCRegisterInfo);
  TargetRegistry::RegisterMCSubtargetInfo(getTheINODETarget(), createINODEMCSubtargetInfo);
  TargetRegistry::RegisterMCInstPrinter(getTheINODETarget(), createINODEMCInstPrinter);
  TargetRegistry::RegisterMCAsmBackend(getTheINODETarget(), createINODEAsmBackend);
}
