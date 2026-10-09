//===-- GankowTargetMachine.cpp - Implement TargetMachine for Gankow --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Gankow.h"
#include "GankowTargetMachine.h"
#include "TargetInfo/GankowTargetInfo.h"
#include "GankowMachineFunctionInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/PassRegistry.h"
#include "llvm/Transforms/Scalar.h"

using namespace llvm;

// Each target provides its own TLOF factory. Gankow has no target-specific
// object-file lowering yet, so use the generic per-format implementations.
static std::unique_ptr<TargetLoweringObjectFile> createTLOF(const Triple &TT) {
  if (TT.isOSBinFormatMachO())
    return std::make_unique<TargetLoweringObjectFileMachO>();
  if (TT.isOSBinFormatCOFF())
    return std::make_unique<TargetLoweringObjectFileCOFF>();
  return std::make_unique<TargetLoweringObjectFileELF>();
}

GankowTargetMachine::GankowTargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         std::optional<Reloc::Model> RM,
                                         std::optional<CodeModel::Model> CM,
                                         CodeGenOptLevel OL, bool JIT)
    : CodeGenTargetMachineImpl(T, TT, CPU, FS, Options,
                               RM.value_or(Reloc::Static),
                               CM.value_or(CodeModel::Small), OL),
  TLOF(createTLOF(TT)) {
  initAsmInfo();
}

const GankowSubtarget *
GankowTargetMachine::getSubtargetImpl(const Function &F) const {
  Attribute CPUAttr = F.getFnAttribute("target-cpu");
  Attribute FSAttr = F.getFnAttribute("target-features");

  std::string CPU =
      CPUAttr.isValid() ? CPUAttr.getValueAsString().str() : TargetCPU;
  std::string FS =
      FSAttr.isValid() ? FSAttr.getValueAsString().str() : TargetFS;

  SmallString<64> Key;
  raw_svector_ostream(Key) << CPU << FS;
  auto &I = SubtargetMap[Key];
  if (!I)
    I = std::make_unique<GankowSubtarget>(TargetTriple, CPU, FS, *this);
  return I.get();
}

MachineFunctionInfo *GankowTargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return GankowMachineFunctionInfo::create<GankowMachineFunctionInfo>(Allocator,
                                                                      F, STI);
}
namespace {
/// Gankow Code Generator Pass Configuration Options.
class GankowPassConfig : public TargetPassConfig {
public:
  GankowPassConfig(GankowTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  GankowTargetMachine &getGankowTargetMachine() const {
    return getTM<GankowTargetMachine>();
  }

  // bool addInstSelector() override;
  void addIRPasses() override;
  void addPreEmitPass() override;
};
} // end anonymous namespace

// bool GankowPassConfig::addInstSelector() {
//   addPass(createGankowISelDag(getGankowTargetMachine(), getOptLevel()));
//   return false;
// }

void GankowPassConfig::addIRPasses() {
  addPass(createAtomicExpandLegacyPass());
  TargetPassConfig::addIRPasses();
}

void GankowPassConfig::addPreEmitPass() { addPass(&BranchRelaxationPassID); }

TargetPassConfig *GankowTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new GankowPassConfig(*this, PM);
}


extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializeGankowTarget() {
  RegisterTargetMachine<GankowTargetMachine> X(getTheGankowTarget());

  auto *PR = PassRegistry::getPassRegistry();
  initializeGankowAsmPrinterPass(*PR);
}

