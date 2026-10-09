//===-- GankowInstPrinter.cpp - Convert Gankow MCInst to asm syntax -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class prints a Gankow MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/GankowMCTargetDesc.h"
#include "GankowInstPrinter.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCInstPrinter.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"


#define PRINT_ALIAS_INSTR
#include "GankowGenAsmWriter.inc"

void GankowInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                  StringRef Annot, const MCSubtargetInfo &STI,
                                  raw_ostream &O) {
  if (PrintAliases && printAliasInstr(MI, Address, O)) {
    printAnnotation(O, Annot);
    return;
  }
  printInstruction(MI, Address, O);
  printAnnotation(O, Annot);
}

void GankowInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) {
  OS << getRegisterName(Reg);
}

void GankowInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                     raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }
  if (Op.isImm()) {
    O << formatImm(static_cast<int32_t>(Op.getImm()));
    return;
  }
  assert(Op.isExpr() && "Expected an expression operand");
  MAI.printExpr(O, *Op.getExpr());
}
