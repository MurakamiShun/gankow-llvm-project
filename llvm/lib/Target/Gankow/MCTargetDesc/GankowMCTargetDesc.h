//===-- GankowMCTargetDesc.h - Gankow Target Descriptions -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Gankow MC-layer target descriptions: the factory functions wired into
// LLVMInitializeGankowTargetMC().
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GANKOW_MCTARGETDESC_GANKOWMCTARGETDESC_H
#define LLVM_LIB_TARGET_GANKOW_MCTARGETDESC_GANKOWMCTARGETDESC_H

#include <cstdint>
#include <memory>

namespace llvm {
class MCAsmBackend;
class MCAsmInfo;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCObjectWriter;
class MCRegisterInfo;
class MCStreamer;
class MCSubtargetInfo;
class MCTargetOptions;
class StringRef;
class Target;
class Triple;

MCCodeEmitter *createGankowMCCodeEmitter(const MCInstrInfo &MCII,
                                         MCContext &Ctx);
MCAsmBackend *createGankowAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                     const MCRegisterInfo &MRI,
                                     const MCTargetOptions &Options);
std::unique_ptr<MCObjectTargetWriter>
createGankowELFObjectWriter(uint8_t OSABI);
MCStreamer *createGankowELFStreamer(
    const Triple &TT, MCContext &Ctx, std::unique_ptr<MCAsmBackend> &&MAB,
    std::unique_ptr<MCObjectWriter> &&OW, std::unique_ptr<MCCodeEmitter> &&MCE);

} // namespace llvm

// Defines symbolic names for Gankow instructions.
#define GET_INSTRINFO_ENUM
#include "GankowGenInstrInfo.inc"

// Defines symbolic names for Gankow registers.
#define GET_REGINFO_ENUM
#include "GankowGenRegisterInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "GankowGenSubtargetInfo.inc"

#endif // LLVM_LIB_TARGET_GANKOW_MCTARGETDESC_GANKOWMCTARGETDESC_H
