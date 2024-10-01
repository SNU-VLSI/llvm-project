add_llvm_component_library(LLVMIMCEDesc
  IMCEInstPrinter.cpp
  IMCEMCCodeEmitter.cpp
  IMCEMCAsmInfo.cpp
  IMCEELFObjectWriter.cpp
  IMCEMCExpr.cpp
  IMCEAsmBackend.cpp
  IMCEMCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  Support
  IMCEInfo

  ADD_TO_COMPONENT
  IMCE
)
