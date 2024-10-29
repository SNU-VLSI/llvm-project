add_llvm_component_library(LLVMINODEAsmParser
  INODEAsmParser.cpp

  LINK_COMPONENTS
  MC
  MCParser
  Support
  INODEDesc
  INODEInfo

  ADD_TO_COMPONENT
  INODE
)
