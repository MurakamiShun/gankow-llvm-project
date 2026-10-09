//===-- GankowELFStreamer.cpp - Gankow ELF Streamer -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Minimal ELF object streamer for Gankow. It is the generic MCELFStreamer;
// target-specific attributes/mapping symbols are not emitted yet.
//
//===----------------------------------------------------------------------===//

#include "GankowMCTargetDesc.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCObjectWriter.h"

using namespace llvm;

namespace {
class GankowELFStreamer : public MCELFStreamer {
public:
  GankowELFStreamer(MCContext &C, std::unique_ptr<MCAsmBackend> MAB,
                    std::unique_ptr<MCObjectWriter> MOW,
                    std::unique_ptr<MCCodeEmitter> MCE)
      : MCELFStreamer(C, std::move(MAB), std::move(MOW), std::move(MCE)) {}
};
} // end anonymous namespace

MCStreamer *llvm::createGankowELFStreamer(
    const Triple &, MCContext &C, std::unique_ptr<MCAsmBackend> &&MAB,
    std::unique_ptr<MCObjectWriter> &&OW, std::unique_ptr<MCCodeEmitter> &&MCE) {
  return new GankowELFStreamer(C, std::move(MAB), std::move(OW),
                               std::move(MCE));
}
