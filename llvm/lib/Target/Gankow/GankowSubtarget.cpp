//===-- GankowSubtarget.cpp - Define Subtarget for Gankow -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The Gankow subtarget and its placeholder frame lowering. The generated
// GankowGenSubtargetInfo constructor and schedule tables live in
// GankowGenSubtargetInfo.inc.
//
//===----------------------------------------------------------------------===//

#include "GankowSubtarget.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/Target/TargetMachine.h"

// In this LLVM revision the generated ParseSubtargetFeatures definition lives
// under GET_SUBTARGETINFO_TARGET_DESC (not the legacy GET_SUBTARGET_FEATURES).
#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "GankowGenSubtargetInfo.inc"

using namespace llvm;

GankowFrameLowering::GankowFrameLowering(const GankowSubtarget &STI)
    // Placeholder frame layout: stack grows down, 4-byte GPR word alignment,
    // no reserved local area. TODO: model the Gankow stack frame (SP-based,
    // ALLOC_SP/FREE_SP) once the ABI is finalized.
    : TargetFrameLowering(StackGrowsDown, /*StackAlignment*/ Align(4),
                          /*LocalAreaOffset*/ 0) {}

void GankowFrameLowering::emitPrologue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  // TODO: emit the Gankow prologue (ALLOC_SP, save callee-saved regs).
}

void GankowFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  // TODO: emit the Gankow epilogue (FREE_SP, restore callee-saved regs).
}

bool GankowFrameLowering::hasFPImpl(const MachineFunction &MF) const {
  return false;
}

GankowSubtarget::GankowSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                                 const TargetMachine &TM)
    : GankowGenSubtargetInfo(TT, CPU, "gankow", FS),
      FrameLowering(*this), InstrInfo(*this), TLInfo(TM, *this) {}


GankowSubtarget &
GankowSubtarget::initializeSubtargetDependencies(const Triple &TT, StringRef CPU,
                                                StringRef TuneCPU, StringRef FS,
                                                StringRef ABIName) {

  ParseSubtargetFeatures(CPU, TuneCPU, FS);
  return *this;
}