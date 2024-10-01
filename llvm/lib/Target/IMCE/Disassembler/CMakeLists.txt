add_llvm_component_library(LLVMIMCEDisassembler
  IMCEDisassembler.cpp

  LINK_COMPONENTS
  MC
  MCDisassembler
  Support
  IMCEDesc
  IMCEInfo

  ADD_TO_COMPONENT
  IMCE
)
