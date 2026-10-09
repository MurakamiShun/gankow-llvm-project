//===-- GankowMCAsmInfo.cpp - Gankow Asm properties -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Gankow assembly-language properties (ELF, 32-bit, little-endian).
//
//===----------------------------------------------------------------------===//

#include "GankowMCAsmInfo.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

void GankowMCAsmInfo::anchor() {}

GankowMCAsmInfo::GankowMCAsmInfo(const Triple &TT, const MCTargetOptions &Options)
    : MCAsmInfoELF(Options) {
  IsLittleEndian = TT.isLittleEndian();
  // 32-bit datapath / pointers.
  CodePointerSize = CalleeSaveStackSlotSize = 4;
  CommentString = "#";
  // The frame lowering is a placeholder and does not yet emit CFI; keep the
  // skeleton free of exception/debug output until the ABI is finalized.
  ExceptionsType = ExceptionHandling::None;
  SupportsDebugInformation = false;
}

void GankowMCAsmInfo::printSpecifierExpr(raw_ostream &OS,
                                        const MCSpecifierExpr &Expr) const {
  // auto S = Expr.getSpecifier();
  // bool HasSpecifier = S != RISCV::S_None && S != RISCV::S_CALL_PLT;
  // if (HasSpecifier)
  //   OS << '%' << RISCV::getSpecifierName(S) << '(';
  // printExpr(OS, *Expr.getSubExpr());
  // if (HasSpecifier)
  //   OS << ')';
}
