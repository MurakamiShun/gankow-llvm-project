//===-- GankowAsmPrinter.h - Gankow Assembly Printer ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The Gankow assembly printer (a MachineFunctionPass). It relies on the base
// AsmPrinter, which in turn uses the target's AsmWriter / MCCodeEmitter for
// the actual per-instruction text and binary output.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GANKOW_GANKOWASMPRINTER_H
#define LLVM_LIB_TARGET_GANKOW_GANKOWASMPRINTER_H

#include "llvm/CodeGen/MachineFunctionAnalysisManager.h"
#include "llvm/IR/Analysis.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"

namespace llvm {

class GankowAsmPrinterBeginPass
    : public RequiredPassInfoMixin<GankowAsmPrinterBeginPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM);
};

class GankowAsmPrinterPass : public RequiredPassInfoMixin<GankowAsmPrinterPass> {
public:
  PreservedAnalyses run(MachineFunction &MF,
                        MachineFunctionAnalysisManager &MFAM);
};

class GankowAsmPrinterEndPass
    : public RequiredPassInfoMixin<GankowAsmPrinterEndPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM);
};

} // namespace llvm

#endif
