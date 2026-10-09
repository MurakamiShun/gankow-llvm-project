//===-- GankowISelLowering.h - SelectionDAG Lowering --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// SelectionDAG lowering for Gankow. This is currently a buildable skeleton:
// the instruction-selection patterns in GankowInstrInfo.td are empty
// placeholders, so real lowering will land together with the final ISA
// encoding.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GANKOW_GANKOWISELLOWERING_H
#define LLVM_LIB_TARGET_GANKOW_GANKOWISELLOWERING_H

#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

class GankowSubtarget;

class GankowTargetLowering : public TargetLowering {
  const GankowSubtarget &Subtarget;

public:
  explicit GankowTargetLowering(const TargetMachine &TM,
                               const GankowSubtarget &STI);

  const GankowSubtarget &getSubtarget() const { return Subtarget; }


  // Lower incoming arguments, copy physregs into vregs
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &DL, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;
};

} // namespace llvm

#endif
