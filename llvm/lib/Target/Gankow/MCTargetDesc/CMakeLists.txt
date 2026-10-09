add_llvm_component_library(LLVMGankowDesc
  GankowMCTargetDesc.cpp
  GankowInstPrinter.cpp
  GankowMCAsmInfo.cpp
  GankowMCCodeEmitter.cpp
  GankowELFStreamer.cpp
  GankowELFObjectWriter.cpp
  GankowAsmBackend.cpp
  GankowTargetStreamer.cpp

  LINK_COMPONENTS
  BinaryFormat
  MC
  GankowInfo
  Support
  TargetParser

  ADD_TO_COMPONENT
  Gankow
)
