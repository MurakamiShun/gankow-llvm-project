add_llvm_component_group(Gankow)

set(LLVM_TARGET_DEFINITIONS Gankow.td)

tablegen(LLVM GankowGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM GankowGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM GankowGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM GankowGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM GankowGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM GankowGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM GankowGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM GankowGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM GankowGenMCPseudoLowering.inc -gen-pseudo-lowering)

tablegen(LLVM GankowGenCallingConv.inc -gen-callingconv)

add_public_tablegen_target(GankowCommonTableGen)

add_llvm_target(GankowCodeGen
  GankowTargetMachine.cpp
  GankowTargetObjectFile.cpp
  GankowSubtarget.cpp
  GankowRegisterInfo.cpp
  GankowInstrInfo.cpp
  GankowISelLowering.cpp
  GankowFrameLowering.cpp
  GankowAsmPrinter.cpp

  LINK_COMPONENTS
  Analysis
  CodeGen
  CodeGenTypes
  Core
  MC
  Passes
  Scalar
  SelectionDAG
  Support
  Target
  TargetParser

  ADD_TO_COMPONENT
  Gankow
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)



