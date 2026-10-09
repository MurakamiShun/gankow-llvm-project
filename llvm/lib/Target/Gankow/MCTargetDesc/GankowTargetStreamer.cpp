//===-- GankowTargetStreamer.cpp - RISC-V Target Streamer Methods ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides RISC-V specific target streamer methods.
//
//===----------------------------------------------------------------------===//
#include "GankowTargetStreamer.h"
#include "GankowMCTargetDesc.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;


GankowTargetStreamer::GankowTargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}

void GankowTargetStreamer::finish() { finishAttributeSection(); }
void GankowTargetStreamer::reset() {}

void GankowTargetStreamer::emitAttribute(unsigned Attribute, unsigned Value) {}
void GankowTargetStreamer::finishAttributeSection() {}
void GankowTargetStreamer::emitTextAttribute(unsigned Attribute,
                                            StringRef String) {}
void GankowTargetStreamer::emitIntTextAttribute(unsigned Attribute,
                                               unsigned IntValue,
                                               StringRef StringValue) {}

// This part is for ascii assembly output
GankowTargetAsmStreamer::GankowTargetAsmStreamer(MCStreamer &S,
                                               formatted_raw_ostream &OS)
    : GankowTargetStreamer(S), OS(OS) {}


void GankowTargetAsmStreamer::emitAttribute(unsigned Attribute, unsigned Value) {
  OS << "\t.attribute\t" << Attribute << ", " << Twine(Value) << "\n";
}

void GankowTargetAsmStreamer::emitTextAttribute(unsigned Attribute,
                                               StringRef String) {
  OS << "\t.attribute\t" << Attribute << ", \"" << String << "\"\n";
}

void GankowTargetAsmStreamer::emitIntTextAttribute(unsigned Attribute,
                                                  unsigned IntValue,
                                                  StringRef StringValue) {}

void GankowTargetAsmStreamer::finishAttributeSection() {}
