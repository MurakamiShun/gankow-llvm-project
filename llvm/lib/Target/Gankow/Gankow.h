//===-- Gankow.h - Top-level interface for Gankow ----------------- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// Gankow back-end, plus the GankowTargetMachine class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TARGET_GANKOW_H
#define LLVM_TARGET_GANKOW_H

#include "GankowSubtarget.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include "llvm/Target/TargetMachine.h"
#include <memory>
#include <optional>

namespace llvm {

class TargetLoweringObjectFile;

void initializeGankowAsmPrinterPass(PassRegistry &);

} // namespace llvm

#endif
