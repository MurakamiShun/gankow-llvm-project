//===-- GankowMCCodeEmitter.cpp - Binary encoding for Gankow --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Gankow machine code emitter. The per-instruction bit encoding is generated
// into GankowGenMCCodeEmitter.inc (GankowMCCodeEmitter::getBinaryCodeForInstr)
// from GankowInstrFormats.td; encodeInstruction wraps it and appends the bytes.
//
//===----------------------------------------------------------------------===//

#include "GankowMCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"

// The Gankow:: instruction opcodes referenced by the generated emitter.
#define GET_INSTRINFO_ENUM
#include "GankowGenInstrInfo.inc"

// The generated emitter defines GankowMCCodeEmitter::getBinaryCodeForInstr at
// global scope, so it must be pulled in where llvm::GankowMCCodeEmitter is in
// scope.
namespace llvm {
#include "GankowGenMCCodeEmitter.inc"
}

using namespace llvm;

void GankowMCCodeEmitter::encodeInstruction(const MCInst &MI,
                                            SmallVectorImpl<char> &CB,
                                            SmallVectorImpl<MCFixup> &Fixups,
                                            const MCSubtargetInfo &STI) const {
  uint64_t Bits = getBinaryCodeForInstr(MI, Fixups, STI);
  // Gankow instructions are a fixed 64 bits, stored little-endian.
  for (unsigned i = 0; i < 8; ++i)
    CB.push_back(static_cast<char>((Bits >> (8 * i)) & 0xff));
}

unsigned GankowMCCodeEmitter::getMachineOpValue(const MCInst &MI,
                                                const MCOperand &MO,
                                                SmallVectorImpl<MCFixup> &Fixups,
                                                const MCSubtargetInfo &STI) const {
  if (MO.isReg())
    // The HW register field value is the register's HWEncoding, which for the
    // special (uniform/thread/pointer) registers is 0x80 + number and therefore
    // differs from the MC register ID. Look it up via the register info rather
    // than returning the MC number; the generated emitter masks each field.
    return static_cast<unsigned>(MRI.getEncodingValue(MO.getReg()));
  if (MO.isImm())
    return static_cast<unsigned>(MO.getImm());

  // Symbolic/expression operands (branch/call targets, global addresses) are
  // not yet relocated: report a clean error rather than emitting wrong bytes.
  reportUnsupportedOperand(MI, /*OpNum=*/0);
}

namespace llvm {
// Factory wired into LLVMInitializeGankowTargetMC() (GankowMCTargetDesc.cpp).
// The MC pipeline installs the register info into the context before creating
// the code emitter, so pull the MRI from there.
MCCodeEmitter *createGankowMCCodeEmitter(const MCInstrInfo &MCII,
                                         MCContext &Ctx) {
  return new GankowMCCodeEmitter(*Ctx.getRegisterInfo());
}
} // namespace llvm
