//===-- GankowMCObjectFileInfo.h - Gankow object file Info ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Gankow MC-layer object file info (ELF). Section/symbol naming follows the
// generic ELF defaults for now.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GANKOW_MCTARGETDESC_GANKOWMCOBJECTFILEINFO_H
#define LLVM_LIB_TARGET_GANKOW_MCTARGETDESC_GANKOWMCOBJECTFILEINFO_H

#include "llvm/MC/MCObjectFileInfo.h"

namespace llvm {

class GankowMCObjectFileInfo : public MCObjectFileInfo {};

} // namespace llvm

#endif // LLVM_LIB_TARGET_GANKOW_MCTARGETDESC_GANKOWMCOBJECTFILEINFO_H
