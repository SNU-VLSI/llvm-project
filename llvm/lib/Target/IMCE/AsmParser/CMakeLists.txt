add_llvm_component_library(LLVMIMCEAsmParser
  IMCEAsmParser.cpp

  LINK_COMPONENTS
  MC
  MCParser
  Support
  IMCEDesc
  IMCEInfo

  ADD_TO_COMPONENT
  IMCE
)
