//===-- GankowMCTargetDesc.cpp - Gankow Target Descriptions -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides Gankow-specific target MC descriptions and wires the MC
// layer into the target registry.
//
//===----------------------------------------------------------------------===//

#include "GankowMCTargetDesc.h"
#include "GankowInstPrinter.h"
#include "GankowMCAsmInfo.h"
#include "GankowTargetStreamer.h"
#include "TargetInfo/GankowTargetInfo.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#define GET_INSTRINFO_NAMED_OPS
#include "GankowGenInstrInfo.inc"

#define GET_REGINFO_MC_DESC
#include "GankowGenRegisterInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "GankowGenSubtargetInfo.inc"

static MCInstrInfo *createGankowMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitGankowMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createGankowMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitGankowMCRegisterInfo(X, /*RA=*/Gankow::R0);
  return X;
}

static MCAsmInfo *createGankowMCAsmInfo(const MCRegisterInfo &MRI,
                                        const Triple &TT,
                                        const MCTargetOptions &Options) {
  // Gankow is ELF-only for now.
  return new GankowMCAsmInfo(TT, Options);
}

namespace {
class GankowMCObjectFileInfo : public MCObjectFileInfo {};
} // end anonymous namespace

static MCObjectFileInfo *createGankowMCObjectFileInfo(MCContext &Ctx, bool PIC,
                                                      bool LargeCodeModel) {
  MCObjectFileInfo *MOFI = new GankowMCObjectFileInfo();
  MOFI->initMCObjectFileInfo(Ctx, PIC, LargeCodeModel);
  return MOFI;
}

static MCSubtargetInfo *createGankowMCSubtargetInfo(const Triple &TT,
                                                    StringRef CPU,
                                                    StringRef FS) {
  return createGankowMCSubtargetInfoImpl(TT, CPU, /*TuneCPU=*/CPU, FS);
}

static MCInstPrinter *createGankowMCInstPrinter(const Triple &T,
                                                unsigned SyntaxVariant,
                                                const MCAsmInfo &MAI,
                                                const MCInstrInfo &MII,
                                                const MCRegisterInfo &MRI) {
  if (SyntaxVariant == 0)
    return new GankowInstPrinter(MAI, MII, MRI);
  return nullptr;
}

static MCTargetStreamer *
createGankowAsmTargetStreamer(MCStreamer &S, formatted_raw_ostream &OS,
                             MCInstPrinter *InstPrint) {
  return new GankowTargetAsmStreamer(S, OS);
}

static MCTargetStreamer *createGankowNullTargetStreamer(MCStreamer &S) {
  return new GankowTargetStreamer(S);
}

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializeGankowTargetMC() {
  for (Target *T : {&getTheGankowTarget()}) {
    TargetRegistry::RegisterMCAsmInfo(*T, createGankowMCAsmInfo);
    TargetRegistry::RegisterMCObjectFileInfo(*T, createGankowMCObjectFileInfo);
    TargetRegistry::RegisterMCInstrInfo(*T, createGankowMCInstrInfo);
    TargetRegistry::RegisterMCRegInfo(*T, createGankowMCRegisterInfo);
    TargetRegistry::RegisterMCAsmBackend(*T, createGankowAsmBackend);
    TargetRegistry::RegisterMCCodeEmitter(*T, createGankowMCCodeEmitter);
    TargetRegistry::RegisterMCInstPrinter(*T, createGankowMCInstPrinter);
    TargetRegistry::RegisterMCSubtargetInfo(*T, createGankowMCSubtargetInfo);
    TargetRegistry::RegisterELFStreamer(*T, createGankowELFStreamer);

    // Register the asm target streamer.
    TargetRegistry::RegisterAsmTargetStreamer(*T, createGankowAsmTargetStreamer);
    // Register the null target streamer.
    TargetRegistry::RegisterNullTargetStreamer(*T,
                                               createGankowNullTargetStreamer);
    
  }
}
