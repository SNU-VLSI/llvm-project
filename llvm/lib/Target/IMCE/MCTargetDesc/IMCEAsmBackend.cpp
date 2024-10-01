// //===-- IMCEAsmBackend.cpp - IMCE Assembler Backend ---------------------===//
// //
// // Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// // See https://llvm.org/LICENSE.txt for license information.
// // SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// //
// //===----------------------------------------------------------------------===//

// #include "IMCEAsmBackend.h"
// #include "llvm/ADT/APInt.h"
// #include "llvm/MC/MCAsmInfo.h"
// #include "llvm/MC/MCAsmLayout.h"
// #include "llvm/MC/MCAssembler.h"
// #include "llvm/MC/MCContext.h"
// #include "llvm/MC/MCDirectives.h"
// #include "llvm/MC/MCELFObjectWriter.h"
// #include "llvm/MC/MCExpr.h"
// #include "llvm/MC/MCObjectWriter.h"
// #include "llvm/MC/MCSymbol.h"
// #include "llvm/MC/MCValue.h"
// #include "llvm/Support/Endian.h"
// #include "llvm/Support/EndianStream.h"
// #include "llvm/Support/ErrorHandling.h"
// #include "llvm/Support/LEB128.h"
// #include "llvm/Support/raw_ostream.h"

// using namespace llvm;

// std::unique_ptr<MCObjectTargetWriter> IMCEAsmBackend::createObjectTargetWriter() const {
//   return createIMCEELFObjectWriter(OSABI, Is64Bit);
// }

// void IMCEAsmBackend::applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
//                                  const MCValue &Target,
//                                  MutableArrayRef<char> Data, uint64_t Value,
//                                  bool IsResolved,
//                                  const MCSubtargetInfo *STI) const {
// }

// bool IMCEAsmBackend::writeNopData(raw_ostream &OS, uint64_t Count,
//                                    const MCSubtargetInfo *STI) const {
//   return true;
// }

// MCAsmBackend *llvm::createIMCEAsmBackend(const Target &T,
//                                           const MCSubtargetInfo &STI,
//                                           const MCRegisterInfo &MRI,
//                                           const MCTargetOptions &Options) {
//   const Triple &TT = STI.getTargetTriple();
//   uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(TT.getOS());
//   return new IMCEAsmBackend(STI, OSABI, TT.isArch64Bit(), Options);
// }