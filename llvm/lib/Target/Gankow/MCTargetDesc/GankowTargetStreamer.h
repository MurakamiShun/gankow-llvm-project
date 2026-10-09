//===-- GankowTargetStreamer.h - Gankow Target Streamer ---------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Gankow_MCTARGETDESC_GankowTARGETSTREAMER_H
#define LLVM_LIB_TARGET_Gankow_MCTARGETDESC_GankowTARGETSTREAMER_H

#include "Gankow.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"

namespace llvm {

class formatted_raw_ostream;

enum class GankowOptionArchArgType {
  Full,
  Plus,
  Minus,
};

struct GankowOptionArchArg {
  GankowOptionArchArgType Type;
  std::string Value;

  GankowOptionArchArg(GankowOptionArchArgType Type, std::string Value)
      : Type(Type), Value(Value) {}
};

class GankowTargetStreamer : public MCTargetStreamer {
  //GankowABI::ABI TargetABI = GankowABI::ABI_Unknown;

public:
  GankowTargetStreamer(MCStreamer &S);
  void finish() override;
  virtual void reset();

  virtual void emitAttribute(unsigned Attribute, unsigned Value);
  virtual void finishAttributeSection();
  virtual void emitTextAttribute(unsigned Attribute, StringRef String);
  virtual void emitIntTextAttribute(unsigned Attribute, unsigned IntValue,
                                    StringRef StringValue);
};

// This part is for ascii assembly output
class GankowTargetAsmStreamer : public GankowTargetStreamer {
  formatted_raw_ostream &OS;

  void finishAttributeSection() override;
  void emitAttribute(unsigned Attribute, unsigned Value) override;
  void emitTextAttribute(unsigned Attribute, StringRef String) override;
  void emitIntTextAttribute(unsigned Attribute, unsigned IntValue,
                            StringRef StringValue) override;

public:
  GankowTargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);
};

}
#endif
