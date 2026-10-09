//===-- GankowFrameLowering.h - Gankow Frame Lowering -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The Gankow frame lowering. The generated register info references a
// GankowFrameLowering type (see GankowGenRegisterInfo.inc), so it must exist
// even though the real stack-frame logic is still a placeholder.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GANKOW_GANKOWFRAMELOWERING_H
#define LLVM_LIB_TARGET_GANKOW_GANKOWFRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {

class GankowSubtarget;

class GankowFrameLowering : public TargetFrameLowering {
public:
  GankowFrameLowering(const GankowSubtarget &STI);

  // TargetFrameLowering pure virtuals (skeleton implementations).
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  bool hasFPImpl(const MachineFunction &MF) const override;
};

} // namespace llvm

#endif
