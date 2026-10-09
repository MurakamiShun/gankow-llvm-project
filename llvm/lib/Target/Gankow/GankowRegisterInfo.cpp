//===-- GankowRegisterInfo.cpp - Gankow Register Info ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Pulls in the tblgen-generated register information for Gankow (enums, MC
// register-class storage, and the target-side TargetRegisterInfo members) and
// implements the small number of target-specific TargetRegisterInfo hooks that
// are not generated.
//
//===----------------------------------------------------------------------===//

#include "GankowRegisterInfo.h"
#include "GankowFrameLowering.h"
#include "GankowSubtarget.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

#define GET_REGINFO_TARGET_DESC
#include "GankowGenRegisterInfo.inc"

using namespace llvm;

const MCPhysReg *
GankowRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_LP32_SaveList;
}
const uint32_t *
GankowRegisterInfo::getCallPreservedMask(const MachineFunction & MF,
                                        CallingConv::ID CC) const {
  return CSR_LP32_RegMask;
}

BitVector GankowRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  auto &Subtarget = MF.getSubtarget<GankowSubtarget>();
  // Reserve the thread-local stack pointer; the remaining fixed/special
  // registers (pc, cf, ...) will be reserved as the ABI is finalized.
  markSuperRegs(Reserved, Gankow::CFSP);
  markSuperRegs(Reserved, Gankow::SP);
  markSuperRegs(Reserved, Gankow::GP);
  markSuperRegs(Reserved, Gankow::PRDC);
  markSuperRegs(Reserved, Gankow::CARRY);
  markSuperRegs(Reserved, Gankow::PC);
  markSuperRegs(Reserved, Gankow::CF);

  for (MCPhysReg Reg = Gankow::TID_X; Reg <= Gankow::BDIM_Z; Reg++)
    markSuperRegs(Reserved, Reg);

  if (!Subtarget.hasR128()){
    MCPhysReg Reg = Gankow::R64;
    if(Subtarget.hasR32())
      Reg = Gankow::R32;
    if(Subtarget.hasR16())
      Reg = Gankow::R16;
    if(Subtarget.hasR8())
      Reg = Gankow::R8;
    for (;Reg <= Gankow::R127; Reg++)
      markSuperRegs(Reserved, Reg);
  }
  return Reserved;
}

bool GankowRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI,
                                             int SPAdj, unsigned FIOperandNum,
                                             RegScavenger *RS) const {
  // The skeleton cannot eliminate frame indices.
  return false;
}

Register GankowRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  // No dedicated frame register for the skeleton (SP is reserved, not a frame
  // pointer).
  return Register(MCRegister::NoRegister);
}
