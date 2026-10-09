//===-- GankowELFObjectWriter.cpp - Gankow ELF Writer ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// 32-bit little-endian ELF writer for Gankow (e_machine = EM_GANKOW).
// Symbol relocations are not yet implemented (see getRelocType).
//
//===----------------------------------------------------------------------===//

#include "GankowMCTargetDesc.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
class GankowELFObjectWriter : public MCELFObjectTargetWriter {
public:
  GankowELFObjectWriter(uint8_t OSABI)
      : MCELFObjectTargetWriter(/*Is64Bit=*/false, OSABI, ELF::EM_GANKOW,
                                /*HasRelocationAddend=*/true) {}

protected:
  unsigned getRelocType(const MCFixup &Fixup, const MCValue &Target,
                        bool IsPCRel) const override {
    // Relocation support is deferred for the Gankow skeleton: literal
    // immediates are encoded directly and never reach here. Any symbol
    // reference (branch target, global address, data reloc) is rejected with a
    // clear diagnostic instead of emitting an incorrect relocation.
    report_fatal_error("Gankow: relocations are not yet supported");
  }
};
} // end anonymous namespace

std::unique_ptr<MCObjectTargetWriter>
llvm::createGankowELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<GankowELFObjectWriter>(OSABI);
}
