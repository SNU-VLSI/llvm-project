//===-- IMCEAsmParser.cpp - Parse IMCE assembly to
// MCInst instructions ----===//
//
// Part of the LLVM Project, under the Apache License
// v2.0 with LLVM Exceptions. See
// https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH
// LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/IMCEInstPrinter.h"
#include "MCTargetDesc/IMCEMCTargetDesc.h"
#include "TargetInfo/IMCETargetInfo.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/MC/MCAsmMacro.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCParser/MCAsmLexer.h"
#include "llvm/MC/MCParser/MCAsmParser.h"
#include "llvm/MC/MCParser/MCParsedAsmOperand.h"
#include "llvm/MC/MCParser/MCTargetAsmParser.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/SMLoc.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/TargetParser/SubtargetFeature.h"
#include <cassert>
#include <cstdint>
#include <memory>
#include <string>

using namespace llvm;

namespace {

// Instances of this class represented a parsed machine
// instruction
class IMCEOperand : public MCParsedAsmOperand {
  enum OperandKind {
    OpKind_Token,
    OpKind_Reg,
    OpKind_Imm,
  };

  OperandKind Kind;
  SMLoc StartLoc, EndLoc;

  union {
    StringRef Token;
    unsigned RegNo;
    const MCExpr *Imm;
  };

  void addExpr(MCInst &Inst, const MCExpr *Expr) const {
    // Add as immediates when possible.  Null MCExpr =
    // 0.
    if (!Expr)
      Inst.addOperand(MCOperand::createImm(0));
    else if (auto *CE = dyn_cast<MCConstantExpr>(Expr))
      Inst.addOperand(MCOperand::createImm(CE->getValue()));
    else
      Inst.addOperand(MCOperand::createExpr(Expr));
  }

public:
  IMCEOperand(OperandKind Kind, SMLoc StartLoc, SMLoc EndLoc)
      : Kind(Kind), StartLoc(StartLoc), EndLoc(EndLoc) {}

  // getStartLoc - Gets location of the first token of
  // this operand
  SMLoc getStartLoc() const override { return StartLoc; }

  // getEndLoc - Gets location of the last token of this
  // operand
  SMLoc getEndLoc() const override { return EndLoc; }

  bool isReg() const override { return Kind == OpKind_Reg; }

  unsigned getReg() const override {
    assert(isReg() && "Invalid type access!");
    return RegNo;
  }

  bool isImm() const override { return Kind == OpKind_Imm; }

  bool isJumpTarget() const { return isImm(); }

  const MCExpr *getImm() const {
    assert(isImm() && "Invalid type access!");
    return Imm;
  }

  bool isToken() const override { return Kind == OpKind_Token; }

  StringRef getToken() const {
    assert(isToken() && "Not a token");
    return Token;
  }

  bool isMem() const override { return false; }

  static std::unique_ptr<IMCEOperand> createToken(StringRef Str, SMLoc Loc) {
    auto Op = std::make_unique<IMCEOperand>(OpKind_Token, Loc, Loc);
    Op->Token = Str;
    return Op;
  }

  static std::unique_ptr<IMCEOperand> createReg(unsigned Num, SMLoc StartLoc, SMLoc EndLoc) {
    auto Op = std::make_unique<IMCEOperand>(OpKind_Reg, StartLoc, EndLoc);
    Op->RegNo = Num;
    return Op;
  }

  static std::unique_ptr<IMCEOperand> createImm(const MCExpr *Expr, SMLoc StartLoc, SMLoc EndLoc) {
    auto Op = std::make_unique<IMCEOperand>(OpKind_Imm, StartLoc, EndLoc);
    Op->Imm = Expr;
    return Op;
  }

  // Used by the TableGen code to add particular types
  // of operand to an instruction.
  void addRegOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands");
    Inst.addOperand(MCOperand::createReg(getReg()));
  }

  void addImmOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands");
    addExpr(Inst, getImm());
  }

  void addJumpTargetOperands(MCInst &Inst, unsigned N) const { addImmOperands(Inst, N); }

  void print(raw_ostream &OS) const override {
    switch (Kind) {
    case OpKind_Imm:
      OS << "Imm: " << getImm() << "\n";
      break;
    case OpKind_Token:
      OS << "Token: " << getToken() << "\n";
      break;
    case OpKind_Reg:
      OS << "Reg: " << IMCEInstPrinter::getRegisterName(getReg()) << "\n";
      break;
    }
  }

  bool isUImm1() const {
    if (!isImm())
      return false;
    if (auto *CE = dyn_cast<MCConstantExpr>(getImm()))
      return isUInt<1>(CE->getValue());
    return false;
  }

  bool isUImm2() const {
    if (!isImm())
      return false;
    if (auto *CE = dyn_cast<MCConstantExpr>(getImm()))
      return isUInt<2>(CE->getValue());
    return false;
  }

  bool isUImm3() const {
    if (!isImm())
      return false;
    if (auto *CE = dyn_cast<MCConstantExpr>(getImm()))
      return isUInt<3>(CE->getValue());
    return false;
  }

  bool isUImm4() const {
    if (!isImm())
      return false;
    if (auto *CE = dyn_cast<MCConstantExpr>(getImm()))
      return isUInt<4>(CE->getValue());
    return false;
  }

  bool isUImm6() const {
    if (!isImm())
      return false;
    if (auto *CE = dyn_cast<MCConstantExpr>(getImm()))
      return isUInt<6>(CE->getValue());
    return false;
  }

  bool isUImm8() const {
    if (!isImm())
      return false;
    if (auto *CE = dyn_cast<MCConstantExpr>(getImm()))
      return isUInt<8>(CE->getValue());
    return false;
  }

  bool isUImm14() const {
    if (!isImm())
      return false;
    if (auto *CE = dyn_cast<MCConstantExpr>(getImm()))
      return isUInt<14>(CE->getValue());
    return false;
  }

  bool isUImm26() const {
    if (!isImm())
      return false;
    if (auto *CE = dyn_cast<MCConstantExpr>(getImm()))
      return isUInt<26>(CE->getValue());
    return false;
  }

  bool isSImm6() const {
    if (!isImm())
      return false;
    if (auto *CE = dyn_cast<MCConstantExpr>(getImm()))
      return isInt<6>(CE->getValue());
    return false;
  }

  bool isSImm14() const {
    if (!isImm())
      return false;
    if (auto *CE = dyn_cast<MCConstantExpr>(getImm()))
      return isInt<14>(CE->getValue());
    return false;
  }

  bool isImmZero() const {
    if (!isImm())
      return false;
    if (auto *CE = dyn_cast<MCConstantExpr>(getImm()))
      return (CE->getValue() == 0);
    return false;
  }
};

class IMCEAsmParser : public MCTargetAsmParser {
// Auto-generated instruction matching functions
#define GET_ASSEMBLER_HEADER
#include "IMCEGenAsmMatcher.inc"

  MCAsmParser &Parser;
  const MCSubtargetInfo &SubtargetInfo;

  bool ParseInstruction(ParseInstructionInfo &Info, StringRef Name, SMLoc NameLoc,
                        OperandVector &Operands) override;
  bool parseRegister(MCRegister &RegNo, SMLoc &StartLoc, SMLoc &EndLoc) override;
  OperandMatchResultTy tryParseRegister(MCRegister &RegNo, SMLoc &StartLoc, SMLoc &EndLoc) override;

  bool parseRegister(MCRegister &RegNo, SMLoc &StartLoc, SMLoc &EndLoc, bool RestoreOnFailure);
  bool parseOperand(OperandVector &Operands, StringRef Mnemonic);

  bool MatchAndEmitInstruction(SMLoc IdLoc, unsigned &Opcode, OperandVector &Operands,
                               MCStreamer &Out, uint64_t &ErrorInfo,
                               bool MatchingInlineAsm) override;

public:
  IMCEAsmParser(const MCSubtargetInfo &STI, MCAsmParser &Parser, const MCInstrInfo &MII,
                const MCTargetOptions &Options)
      : MCTargetAsmParser(Options, STI, MII), Parser(Parser), SubtargetInfo(STI) {
    setAvailableFeatures(ComputeAvailableFeatures(SubtargetInfo.getFeatureBits()));
  }

  enum IMCEMatchResultTy {
    Match_Dummy = FIRST_TARGET_MATCH_RESULT_TY,
    Match_RequiresEvenGPRs,
#define GET_OPERAND_DIAGNOSTIC_TYPES
#include "IMCEGenAsmMatcher.inc"
#undef GET_OPERAND_DIAGNOSTIC_TYPES
  };
};

} // end anonymous namespace

#define GET_REGISTER_MATCHER
#define GET_MATCHER_IMPLEMENTATION
#include "IMCEGenAsmMatcher.inc"

bool IMCEAsmParser::ParseInstruction(ParseInstructionInfo &Info, StringRef Name, SMLoc NameLoc,
                                     OperandVector &Operands) {
  // First operand in MCInst is instruction mnemonic.
  Operands.push_back(IMCEOperand::createToken(Name, NameLoc));

  // Read the remaining operands.
  if (getLexer().isNot(AsmToken::EndOfStatement)) {

    // Read the first operand.
    if (parseOperand(Operands, Name)) {
      return Error(getLexer().getLoc(), "expected operand");
    }

    // Read the following operands.
    while (getLexer().is(AsmToken::Comma)) {
      Parser.Lex();
      if (parseOperand(Operands, Name)) {
        return Error(getLexer().getLoc(), "expected operand");
      }
    }
    if (getLexer().isNot(AsmToken::EndOfStatement))
      return Error(getLexer().getLoc(), "unexpected token in argument list");
  }

  // Consume the EndOfStatement.
  Parser.Lex();
  return false;
}

bool IMCEAsmParser::parseOperand(OperandVector &Operands, StringRef Mnemonic) {
  // Check if it is a register.
  if (Parser.getTok().is(AsmToken::Percent)) {
    MCRegister RegNo;
    SMLoc StartLoc, EndLoc;
    if (parseRegister(RegNo, StartLoc, EndLoc,
                      /*RestoreOnFailure=*/false))
      return true;
    Operands.push_back(IMCEOperand::createReg(RegNo, StartLoc, EndLoc));
    return false;
  }

  // Could be immediate or address.
  if (Parser.getTok().is(AsmToken::Integer)) {
    SMLoc StartLoc = Parser.getTok().getLoc();
    const MCExpr *Expr;
    if (Parser.parseExpression(Expr))
      return true;
    SMLoc EndLoc = Parser.getTok().getLoc();
    Operands.push_back(IMCEOperand::createImm(Expr, StartLoc, EndLoc));
    return false;
  }
  // Failure
  return true;
}

// Parses register of form %(r|x|cr|fcr)<No>.
bool IMCEAsmParser::parseRegister(MCRegister &RegNo, SMLoc &StartLoc, SMLoc &EndLoc,
                                  bool RestoreOnFailure) {
  StartLoc = Parser.getTok().getLoc();

  // Eat the '%' prefix.
  if (Parser.getTok().isNot(AsmToken::Percent))
    return true;
  const AsmToken &PercentTok = Parser.getTok();
  Parser.Lex();

  // Match the register.
  if (Parser.getTok().isNot(AsmToken::Identifier) ||
      (RegNo = MatchRegisterName(Parser.getTok().getIdentifier())) == 0) {
    if (RestoreOnFailure)
      Parser.getLexer().UnLex(PercentTok);
    return Error(StartLoc, "invalid register");
  }

  Parser.Lex(); // Eat identifier token.
  EndLoc = Parser.getTok().getLoc();
  return false;
}

bool IMCEAsmParser::parseRegister(MCRegister &RegNo, SMLoc &StartLoc, SMLoc &EndLoc) {
  return parseRegister(RegNo, StartLoc, EndLoc,
                       /*RestoreOnFailure=*/false);
}

OperandMatchResultTy IMCEAsmParser::tryParseRegister(MCRegister &RegNo, SMLoc &StartLoc,
                                                     SMLoc &EndLoc) {
  bool Result = parseRegister(RegNo, StartLoc, EndLoc,
                              /*RestoreOnFailure=*/true);
  bool PendingErrors = getParser().hasPendingError();
  getParser().clearPendingErrors();
  if (PendingErrors)
    return MatchOperand_ParseFail;
  if (Result)
    return MatchOperand_NoMatch;
  return MatchOperand_Success;
}

bool IMCEAsmParser::MatchAndEmitInstruction(SMLoc IdLoc, unsigned &Opcode, OperandVector &Operands,
                                            MCStreamer &Out, uint64_t &ErrorInfo,
                                            bool MatchingInlineAsm) {
  MCInst Inst;
  SMLoc ErrorLoc;

  switch (MatchInstructionImpl(Operands, Inst, ErrorInfo, MatchingInlineAsm)) {
  case Match_Success:
    Out.emitInstruction(Inst, SubtargetInfo);
    Opcode = Inst.getOpcode();
    return false;
  case Match_MissingFeature:
    return Error(IdLoc, "Instruction use requires "
                        "option to be enabled");
  case Match_MnemonicFail:
    return Error(IdLoc, "Unrecognized instruction mnemonic");
  case Match_InvalidOperand: {
    ErrorLoc = IdLoc;
    if (ErrorInfo != ~0U) {
      if (ErrorInfo >= Operands.size())
        return Error(IdLoc, "Too few operands for instruction");

      ErrorLoc = ((IMCEOperand &)*Operands[ErrorInfo]).getStartLoc();
      if (ErrorLoc == SMLoc())
        ErrorLoc = IdLoc;
    }
    return Error(ErrorLoc, "Invalid operand for instruction");
  }
  default:
    break;
  }

  llvm_unreachable("Unknown match type detected!");
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeIMCEAsmParser() {
  RegisterMCAsmParser<IMCEAsmParser> X(getTheIMCETarget());
}
