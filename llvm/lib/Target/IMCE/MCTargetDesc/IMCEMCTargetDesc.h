//===-- IMCEMCTargetDesc.h - IMCE target descriptions -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_IMCE_MCTARGETDESC_IMCEMCTARGETDESC_H
#define LLVM_LIB_TARGET_IMCE_MCTARGETDESC_IMCEMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

#include <memory>

namespace llvm {

class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;
class StringRef;
class Target;
class Triple;
class raw_pwrite_stream;
class raw_ostream;

MCCodeEmitter *createIMCEMCCodeEmitter(const MCInstrInfo &MCII, MCContext &Ctx);
} // end namespace llvm

// Defines symbolic names for IMCE registers.
// This defines a mapping from register name to register number.
#define GET_REGINFO_ENUM
#include "IMCEGenRegisterInfo.inc"

// Defines symbolic names for the IMCE instructions.
#define GET_INSTRINFO_ENUM
#include "IMCEGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "IMCEGenSubtargetInfo.inc"

#endif
