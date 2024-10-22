//===--- IMCE.h - Declare RISC-V target feature support --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares RISC-V TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_IMCE_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_IMCE_H

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/Support/Compiler.h"
#include "llvm/TargetParser/Triple.h"
#include <optional>

namespace clang {
namespace targets {

class LLVM_LIBRARY_VISIBILITY IMCETargetInfo : public TargetInfo {
protected:
  std::string ABI, CPU;

private:
  bool FastScalarUnalignedAccess;
  bool HasExperimental = false;

public:
  IMCETargetInfo(const llvm::Triple &Triple, const TargetOptions &)
      : TargetInfo(Triple) {
    BFloat16Width = 16;
    BFloat16Align = 16;
    BFloat16Format = &llvm::APFloat::BFloat();
    LongDoubleWidth = 128;
    LongDoubleAlign = 128;
    LongDoubleFormat = &llvm::APFloat::IEEEquad();
    SuitableAlign = 128;
    WCharType = SignedInt;
    WIntType = UnsignedInt;
    MCountName = "_mcount";
    HasFloat16 = true;
    HasStrictFP = true;
    resetDataLayout("E-m:e-p:32:32-i16:16:16-v256:256:256");
  }

  ArrayRef<Builtin::Info> getTargetBuiltins() const override;

  std::string_view getClobbers() const override { return ""; }

  ArrayRef<const char *> getGCCRegNames() const override;

  // int getEHDataRegisterNumber(unsigned RegNo) const override {
  //   if (RegNo == 0)
  //     return 10;
  //   else if (RegNo == 1)
  //     return 11;
  //   else
  //     return -1;
  // }

  ArrayRef<TargetInfo::GCCRegAlias> getGCCRegAliases() const override;

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override;

  BuiltinVaListKind getBuiltinVaListKind() const override {
    return TargetInfo::VoidPtrBuiltinVaList;
  }

  bool validateAsmConstraint(const char *&Name,
                             TargetInfo::ConstraintInfo &Info) const override;

  // std::string convertConstraint(const char *&Constraint) const override;

  // bool
  // initFeatureMap(llvm::StringMap<bool> &Features, DiagnosticsEngine &Diags,
  //                StringRef CPU,
  //                const std::vector<std::string> &FeaturesVec) const override;

  // std::optional<std::pair<unsigned, unsigned>>
  // getVScaleRange(const LangOptions &LangOpts) const override;

  // bool hasFeature(StringRef Feature) const override;

  // bool handleTargetFeatures(std::vector<std::string> &Features,
  //                           DiagnosticsEngine &Diags) override;

  // bool hasBitIntType() const override { return true; }

  // bool hasBFloat16Type() const override { return true; }

  // CallingConvCheckResult checkCallingConvention(CallingConv CC) const override;

  // bool useFP16ConversionIntrinsics() const override {
  //   return false;
  // }

  // bool isValidCPUName(StringRef Name) const override;
  // void fillValidCPUList(SmallVectorImpl<StringRef> &Values) const override;
  // bool isValidTuneCPUName(StringRef Name) const override;
  // void fillValidTuneCPUList(SmallVectorImpl<StringRef> &Values) const override;
  // bool supportsTargetAttributeTune() const override { return true; }
  // ParsedTargetAttr parseTargetAttr(StringRef Str) const override;

  // std::pair<unsigned, unsigned> hardwareInterferenceSizes() const override {
  //   return std::make_pair(32, 32);
  // }
};
} // namespace targets
} // namespace clang

#endif // LLVM_CLANG_LIB_BASIC_TARGETS_IMCE_H
