add_llvm_component_library(LLVMINODEInfo
  INODETargetInfo.cpp

  LINK_COMPONENTS
  Support

  ADD_TO_COMPONENT
  INODE
)
