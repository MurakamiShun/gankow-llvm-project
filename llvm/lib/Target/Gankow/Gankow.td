//===-- Gankow.td - Describe the Gankow Target Machine ------*- tablegen -*-===//

include "llvm/Target/Target.td"

include "GankowFeatures.td"

include "GankowRegisterInfo.td"
include "GankowRegisterClass.td"
include "GankowInstrFormats.td"
include "GankowInstrInfo.td"
include "GankowCallingConv.td"

def GankowInstrInfo : InstrInfo {
  let guessInstructionProperties = false;
}

def GankowAsmParser : AsmParser {}

def GankowAsmWriter : AsmWriter {}

defm : RemapAllTargetPseudoPointerOperands<GPR>;

def Gankow : Target {
  let InstructionSet = GankowInstrInfo;
  let AssemblyParsers = [GankowAsmParser];
  let AssemblyWriters = [GankowAsmWriter];
}
