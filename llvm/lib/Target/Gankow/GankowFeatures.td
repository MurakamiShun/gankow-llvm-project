//===-- GankowFeatures.td - Gankow Features and Extensions --*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Gankow subtarget features and instruction predicates.
//===----------------------------------------------------------------------===//

def FeatureR8 : SubtargetFeature<"reg8", "HasR8", "false", "Instruction Set with 8 GPRs">;
def FeatureR16 : SubtargetFeature<"reg16", "HasR16", "false", "Instruction Set with 16 GPRs">;
def FeatureR32 : SubtargetFeature<"reg32", "HasR32", "false", "Instruction Set with 32 GPRs">;
def FeatureR64 : SubtargetFeature<"reg64", "HasR64", "false", "Instruction Set with 64 GPRs">;
def FeatureR128 : SubtargetFeature<"reg128", "HasR128", "true", "Instruction Set with 128 GPRs">;


def FeatureExtF : SubtargetFeature<"fp32", "HasFP32", "true", "32-bit floating point extension">;
