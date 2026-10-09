//===-- GankowInstrInfo.h - Gankow Instruction Info ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The Gankow instruction information. The instruction table, names, and the
// GankowGenInstrInfo base class are generated into GankowGenInstrInfo.inc by
// tblgen from GankowInstrInfo.td.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GANKOW_GANKOWINSTRINFO_H
#define LLVM_LIB_TARGET_GANKOW_GANKOWINSTRINFO_H

#include "GankowRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

// Pull in the generated GankowGenInstrInfo base-class declaration.
#define GET_INSTRINFO_HEADER
#include "GankowGenInstrInfo.inc"
#undef GET_INSTRINFO_HEADER

namespace llvm {

class GankowSubtarget;

class GankowInstrInfo : public GankowGenInstrInfo {
public:
  explicit GankowInstrInfo(const GankowSubtarget &STI);

  const GankowRegisterInfo &getRegisterInfo() const { return RegInfo; }

protected:
  GankowRegisterInfo RegInfo;
};

} // namespace llvm

#endif
