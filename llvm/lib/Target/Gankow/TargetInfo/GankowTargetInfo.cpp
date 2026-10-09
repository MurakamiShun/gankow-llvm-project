//===-- GankowTargetInfo.cpp - Target Registration for Gankow --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/GankowTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"

using namespace llvm;

Target &llvm::getTheGankowTarget() {
  static Target TheGankowTarget;
  return TheGankowTarget;
}

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializeGankowTargetInfo() {
  RegisterTarget<Triple::gankow, /*HasJIT=*/true> X(
    getTheGankowTarget(), "gankow", "Gankow 32-bit address",
                                   "Gankow");
}

