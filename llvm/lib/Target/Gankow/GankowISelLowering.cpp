//===-- GankowISelLowering.cpp - Instruction Selection Lowering for Gankow --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// SelectionDAG lowering for Gankow. Currently a buildable skeleton: the DAG
// selection patterns in GankowInstrInfo.td are empty, so the target-specific
// lowering (call/return ABI, legalization, custom lowerings) will be added
// together with the final ISA encoding.
//
//===----------------------------------------------------------------------===//

#include "GankowISelLowering.h"
#include "GankowSubtarget.h"

using namespace llvm;

GankowTargetLowering::GankowTargetLowering(const TargetMachine &TM,
                                         const GankowSubtarget &STI)
    : TargetLowering(TM, STI), Subtarget(STI) {
  
  // Set up the register classes.
  addRegisterClass(MVT::i32, &Gankow::GPRRegClass);

  // Compute derived properties from the register classes.
  computeRegisterProperties(STI.getRegisterInfo());
}


// Transform physical registers into virtual registers.
SDValue GankowTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

  MachineFunction &MF = DAG.getMachineFunction();

  return Chain;
}