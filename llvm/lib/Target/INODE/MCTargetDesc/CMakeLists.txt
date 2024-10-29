add_llvm_component_library(LLVMINODEDesc
  INODEInstPrinter.cpp
  INODEMCCodeEmitter.cpp
  INODEMCAsmInfo.cpp
  INODEELFObjectWriter.cpp
  INODEMCExpr.cpp
  INODEAsmBackend.cpp
  INODEMCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  Support
  INODEInfo

  ADD_TO_COMPONENT
  INODE
)
