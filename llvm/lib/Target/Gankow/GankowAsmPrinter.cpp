//===-- GankowAsmPrinter.cpp - Gankow Assembly Printer --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GankowAsmPrinter.h"
#include "TargetInfo/GankowTargetInfo.h"
#include "MCTargetDesc/GankowELFStreamer.h"
#include "MCTargetDesc/GankowInstPrinter.h"
#include "MCTargetDesc/GankowMCAsmInfo.h"
#include "MCTargetDesc/GankowTargetStreamer.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/AsmPrinterAnalysis.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionAnalysisManager.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CHERICapabilityFormat.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Instrumentation/HWAddressSanitizer.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"


namespace {
class GankowAsmPrinter : public AsmPrinter {
public:
  static char ID;

private:
  const GankowSubtarget *STI;

public:
  explicit GankowAsmPrinter(TargetMachine &TM,
                           std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer), ID) {}

  StringRef getPassName() const override { return "Gankow Assembly Printer"; }

  GankowTargetStreamer &getTargetStreamer() const {
    return static_cast<GankowTargetStreamer &>(
        *OutStreamer->getTargetStreamer());
  }

  // bool runOnMachineFunction(MachineFunction &MF) override;

  // void emitInstruction(const MachineInstr *MI) override;

  // void emitMachineConstantPoolValue(MachineConstantPoolValue *MCPV) override;

  // bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
  //                      const char *ExtraCode, raw_ostream &OS) override;
  // bool PrintAsmMemoryOperand(const MachineInstr *MI, unsigned OpNo,
  //                            const char *ExtraCode, raw_ostream &OS) override;


private:
};
} // namespace

char GankowAsmPrinter::ID = 0;

INITIALIZE_PASS(GankowAsmPrinter, "Gankow-asm-printer", "Gankow Assembly Printer",
                false, false)

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializeGankowAsmPrinter() {
  RegisterAsmPrinter<GankowAsmPrinter> X(getTheGankowTarget());
}

PreservedAnalyses GankowAsmPrinterBeginPass::run(Module &M,
                                                ModuleAnalysisManager &MAM) {
  GankowAsmPrinter &AsmPrinter = static_cast<GankowAsmPrinter &>(
      MAM.getResult<AsmPrinterAnalysis>(M).getPrinter());
  setupModuleAsmPrinter(M, MAM, AsmPrinter);
  AsmPrinter.doInitialization(M);
  return PreservedAnalyses::all();
}

PreservedAnalyses
GankowAsmPrinterPass::run(MachineFunction &MF,
                         MachineFunctionAnalysisManager &MFAM) {
  GankowAsmPrinter &AsmPrinter = static_cast<GankowAsmPrinter &>(
      MFAM.getResult<ModuleAnalysisManagerMachineFunctionProxy>(MF)
          .getCachedResult<AsmPrinterAnalysis>(*MF.getFunction().getParent())
          ->getPrinter());
  setupMachineFunctionAsmPrinter(MFAM, MF, AsmPrinter);
  AsmPrinter.runOnMachineFunction(MF);
  return PreservedAnalyses::all();
}

PreservedAnalyses GankowAsmPrinterEndPass::run(Module &M,
                                              ModuleAnalysisManager &MAM) {
  GankowAsmPrinter &AsmPrinter = static_cast<GankowAsmPrinter &>(
      MAM.getResult<AsmPrinterAnalysis>(M).getPrinter());
  setupModuleAsmPrinter(M, MAM, AsmPrinter);
  AsmPrinter.doFinalization(M);
  return PreservedAnalyses::all();
}
