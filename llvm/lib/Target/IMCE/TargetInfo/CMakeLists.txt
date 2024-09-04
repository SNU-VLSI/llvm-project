add_llvm_component_library(LLVMIMCEInfo
  IMCETargetInfo.cpp

  LINK_COMPONENTS
  Support

  ADD_TO_COMPONENT
  IMCE
)
