add_llvm_component_library(LLVMINODEDisassembler
  INODEDisassembler.cpp

  LINK_COMPONENTS
  MC
  MCDisassembler
  Support
  INODEDesc
  INODEInfo

  ADD_TO_COMPONENT
  INODE
)
