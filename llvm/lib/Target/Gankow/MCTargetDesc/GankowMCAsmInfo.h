//===-- GankowMCAsmInfo.h - Gankow Asm Info ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the GankowMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GANKOW_MCTARGETDESC_GANKOWMCASMINFO_H
#define LLVM_LIB_TARGET_GANKOW_MCTARGETDESC_GANKOWMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class GankowMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit GankowMCAsmInfo(const Triple &TT, const MCTargetOptions &Options);
  void printSpecifierExpr(raw_ostream &OS,
                          const MCSpecifierExpr &Expr) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_GANKOW_MCTARGETDESC_GANKOWMCASMINFO_H
