//===-- GankowAsmBackend.cpp - Gankow Assembler Backend -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Minimal assembler backend for Gankow: fixed-size (8-byte) instructions, no
// relaxation, and no symbol-relocation support yet (see applyFixup).
//
//===----------------------------------------------------------------------===//

#include "GankowMCTargetDesc.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/EndianStream.h"
#include <cassert>

using namespace llvm;

namespace {

class GankowAsmBackend : public MCAsmBackend {
public:
  GankowAsmBackend() : MCAsmBackend(llvm::endianness::little) {}
  ~GankowAsmBackend() override = default;

  void applyFixup(const MCFragment &F, const MCFixup &Fixup,
                  const MCValue &Target, uint8_t *Data, uint64_t Value,
                  bool IsResolved) override;

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override;

  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override;
};

} // end anonymous namespace

void GankowAsmBackend::applyFixup(const MCFragment &F, const MCFixup &Fixup,
                                  const MCValue &Target, uint8_t *Data,
                                  uint64_t Value, bool IsResolved) {
  // Symbol relocations are deferred: no target fixups are produced, and the
  // code emitter rejects expression operands before they reach this point.
  // Only literal data (e.g. .word/.dword of a resolved constant) is written.
  switch (Fixup.getKind()) {
  case FK_Data_1:
    Data[0] = static_cast<uint8_t>(Value);
    break;
  case FK_Data_2:
    support::endian::write16le(Data, static_cast<uint16_t>(Value));
    break;
  case FK_Data_4:
    support::endian::write32le(Data, static_cast<uint32_t>(Value));
    break;
  case FK_Data_8:
    support::endian::write64le(Data, Value);
    break;
  default:
    report_fatal_error("Gankow: relocations are not yet supported");
  }
}

std::unique_ptr<MCObjectTargetWriter>
GankowAsmBackend::createObjectTargetWriter() const {
  return createGankowELFObjectWriter(ELF::ELFOSABI_NONE);
}

bool GankowAsmBackend::writeNopData(raw_ostream &OS, uint64_t Count,
                                    const MCSubtargetInfo *STI) const {
  // Gankow instructions are a fixed 8 bytes. The all-zero word is a no-op
  // (ADDI zero, zero, 0), so padding is simply zero words.
  if ((Count % 8) != 0)
    return false;
  for (uint64_t i = 0; i < Count; i += 8)
    support::endian::write<uint64_t>(OS, 0, Endian);
  return true;
}

MCAsmBackend *llvm::createGankowAsmBackend(const Target &,
                                           const MCSubtargetInfo &,
                                           const MCRegisterInfo &,
                                           const MCTargetOptions &) {
  return new GankowAsmBackend();
}
