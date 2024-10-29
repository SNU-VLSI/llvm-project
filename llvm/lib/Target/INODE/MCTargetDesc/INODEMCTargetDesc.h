//===-- INODEMCTargetDesc.h - INODE target descriptions -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_INODE_MCTARGETDESC_INODEMCTARGETDESC_H
#define LLVM_LIB_TARGET_INODE_MCTARGETDESC_INODEMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"
#include "INODEBaseInfo.h"

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

MCCodeEmitter *createINODEMCCodeEmitter(const MCInstrInfo &MCII, MCContext &Ctx);
MCAsmBackend *createINODEAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                    const MCRegisterInfo &MRI,
                                    const MCTargetOptions &Options);
std::unique_ptr<MCObjectTargetWriter> createINODEELFObjectWriter(uint8_t OSABI, bool Is64Bit);
} // end namespace llvm

// Defines symbolic names for INODE registers.
// This defines a mapping from register name to register number.
#define GET_REGINFO_ENUM
#include "INODEGenRegisterInfo.inc"

// Defines symbolic names for the INODE instructions.
#define GET_INSTRINFO_ENUM
#include "INODEGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "INODEGenSubtargetInfo.inc"

#endif
