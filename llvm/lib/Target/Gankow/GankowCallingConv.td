//===-- GankowCallingConv.td - Calling Conventions Gankow ---*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the Gankow architecture.
//
//===----------------------------------------------------------------------===//

// The Gankow calling convention is handled with custom code in
// GankowISelLowering.cpp (CC_Gankow).


def CSR_LP32 : CalleeSavedRegs<(add
              (sequence "R%u", 4, 5),    // s0-1
              (sequence "R%u", 12, 13),  // s2-3
              (sequence "R%u", 16, 23),  // s4-11
              (sequence "R%u", 64, 127)  // s12-75
            )>;

