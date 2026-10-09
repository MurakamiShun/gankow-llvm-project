//===-- GankowRegisterInfo.h - Gankow Register Info ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The Gankow register information. The bulk of the implementation (register
// classes, pressure sets, allocatable-register table, ...) is generated into
// GankowGenRegisterInfo.inc by tblgen from GankowRegisterInfo.td; this class
// only adds the target-specific bits.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GANKOW_GANKOWREGISTERINFO_H
#define LLVM_LIB_TARGET_GANKOW_GANKOWREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

// Pull in the generated GankowGenRegisterInfo base-class declaration.
#define GET_REGINFO_HEADER
#include "GankowGenRegisterInfo.inc"
#undef GET_REGINFO_HEADER

namespace llvm {

class GankowRegisterInfo : public GankowGenRegisterInfo {
public:
  GankowRegisterInfo()
      : GankowGenRegisterInfo(0) {}

  // TargetRegisterInfo pure virtuals (skeleton implementations).
  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;
  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;
  bool eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;
  Register getFrameRegister(const MachineFunction &MF) const override;
};

} // namespace llvm

#endif
