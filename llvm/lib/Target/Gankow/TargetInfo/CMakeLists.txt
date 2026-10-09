add_llvm_component_library(LLVMGankowInfo
  GankowTargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  Gankow
  )
