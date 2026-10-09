//===-- GankowInstrInfo.cpp - Gankow Instruction Info ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Gankow instruction information. The generated GankowGenInstrInfo constructor
// and destructor live in GankowGenInstrInfo.inc.
//
//===----------------------------------------------------------------------===//

#include "GankowInstrInfo.h"
#include "GankowSubtarget.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "GankowGenInstrInfo.inc"

using namespace llvm;

GankowInstrInfo::GankowInstrInfo(const GankowSubtarget &STI)
    : GankowGenInstrInfo(STI, RegInfo), RegInfo() {}
