//===-- GankowMCCodeEmitter.h - Gankow Machine Code Emitter -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The Gankow machine code emitter. The class must be named GankowMCCodeEmitter
// because the tblgen-generated GankowGenMCCodeEmitter.inc defines
// GankowMCCodeEmitter::getBinaryCodeForInstr.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TARGET_GANKOW_MCCODEEMITTER_H
#define LLVM_TARGET_GANKOW_MCCODEEMITTER_H

#include "llvm/MC/MCCodeEmitter.h"
#include <cstdint>

namespace llvm {

class MCOperand;
class MCRegisterInfo;

class GankowMCCodeEmitter : public MCCodeEmitter {
  const MCRegisterInfo &MRI;

public:
  GankowMCCodeEmitter(const MCRegisterInfo &MRI) : MRI(MRI) {}
  ~GankowMCCodeEmitter() override = default;

  // MCCodeEmitter interface: append the (fixed 64-bit) encoding of MI to CB.
  void encodeInstruction(const MCInst &MI, SmallVectorImpl<char> &CB,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

  // TableGen-generated (GankowGenMCCodeEmitter.inc): the 64-bit encoding of MI.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

private:
  // Value to place in the instruction field for a single MC operand.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;
};

} // namespace llvm

#endif // LLVM_TARGET_GANKOW_MCCODEEMITTER_H
