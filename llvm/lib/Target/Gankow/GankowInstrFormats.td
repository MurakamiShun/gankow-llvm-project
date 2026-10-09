// GankowInstrFormats.td
//
// The real 64-bit Gankow instruction encodings (two 32-bit words, stored
// little-endian). Field layout (MSB -> LSB), from isa/InstructionType.md:
//
//   bits   63-57  56-53  52-46   45-39   38-32  31-25  24-22  21-14   13-7   6-0
//   name   hint   f4     A       B       C      hint   f3     rs1     rd     opcode
//
// where A/B/C are per-format (rs2, immediates, funct7, ...). The two 7-bit
// hint fields are always 0 here and are fixed in the base class.
//
// Operand fields:
//   rd    {13-7}   7 bits -> GPR (R0-R127) or uni-rd (U0-U7)
//   rs1   {21-14}  8 bits -> RS1 (a GPR or a special register; bit 7 selects)
//   rs2   {52-46}  7 bits -> GPR (R0-R127)
//
// Immediate encodings (sign bit always at bit 32, magnitude split across two
// fields -- see isa/InstructionType.md "Immediate" table):
//   I/S-type : 14-bit signed  imm[13]@32, imm[12:6]@{52-46}(I)/{13-7}(S),
//                                  imm[5:0]@{38-33}
//   branch   : 21-bit signed  imm[20]@32, imm[19:13]@{45-39},
//                                  imm[12:6]@{13-7}, imm[5:0]@{38-33}
//   UniS/Sys : 24-bit signed  imm[23]@32, imm[22:20]@{24-22},
//                                  imm[19:13]@{45-39}, imm[12:6]@{52-46},
//                                  imm[5:0]@{38-33}
//   li.u     : 32-bit signed  imm[31]@32, imm[30:23]@{21-14},
//                                  imm[22:20]@{24-22}, imm[19:13]@{45-39},
//                                  imm[12:6]@{52-46}, imm[5:0]@{38-33}




// Base class for all Gankow instructions.
class GankowInstr : Instruction {
  let Namespace = "Gankow";
  field bits<64> Inst;
  let Size = 8;   // 64-bit instructions

  // Gankow.td sets guessInstructionProperties = false, so every instruction
  // must define its side-effect flags explicitly. Default to a pure,
  // non-memory instruction; load/store/atomic/flow subclasses override below.
  let mayLoad = 0;
  let mayStore = 0;
  let hasSideEffects = 0;

  // The two 7-bit hint fields are always zero.
  let Inst{63-57} = 0;
  let Inst{31-25} = 0;
}

class GankowUImmLeafOp<int bitsNum> :
  Operand<i32>, ImmLeaf<i32, "return isUInt<" # bitsNum # ">(Imm);">;
class GankowSImmLeafOp<int bitsNum> :
  Operand<i32>, ImmLeaf<i32, "return isInt<" # bitsNum # ">(Imm);">;

def immSExt14 : GankowSImmLeafOp<14>;
def immSExt21 : GankowSImmLeafOp<21>;
def immSExt24 : GankowSImmLeafOp<24>;
def immSExt32 : GankowSImmLeafOp<32>;

def immZExt5  : GankowUImmLeafOp<5>;

// R-type: rd = f(rs1, rs2). opcode 011 0000 (48). attr7 {45-39} = 0.
class GankowRType<bits<4> funct4, bits<3> funct3, bits<7> funct7,
                  string opcodestr, list<dag> pattern = []> : GankowInstr {
  let OutOperandList = (outs GPR:$rd);
  let InOperandList  = (ins RS1:$rs1, GPR:$rs2);
  let AsmString = opcodestr # "\t$rd, $rs1, $rs2";
  let Pattern   = pattern;

  bits<7> rd; bits<8> rs1; bits<7> rs2;
  let Inst{6-0}   = 48;
  let Inst{13-7}  = rd;
  let Inst{21-14} = rs1;
  let Inst{24-22} = funct3;
  let Inst{38-32} = funct7;
  let Inst{45-39} = 0;        // attr7
  let Inst{52-46} = rs2;
  let Inst{56-53} = funct4;
}

// I-type: rd = f(rs1, imm14). opcode 001 0000 (16).
class GankowIType<bits<4> funct4, bits<3> funct3, string opcodestr,
                  list<dag> pattern = []> : GankowInstr {
  let OutOperandList = (outs GPR:$rd);
  let InOperandList  = (ins RS1:$rs1, immSExt14:$imm);
  let AsmString = opcodestr # "\t$rd, $rs1, $imm";
  let Pattern   = pattern;

  bits<7> rd; bits<8> rs1; bits<14> imm;
  let Inst{6-0}   = 16;
  let Inst{13-7}  = rd;
  let Inst{21-14} = rs1;
  let Inst{24-22} = funct3;
  let Inst{38-33} = imm{5-0};
  let Inst{32}    = imm{13};
  let Inst{45-39} = 0;        // attr7
  let Inst{52-46} = imm{12-6};
  let Inst{56-53} = funct4;
}

// I-type shift: rd = shift(rs1, shamt). 5-bit shamt at {37-33}, bits 38/32 = 0.
class GankowIShiftType<bits<4> funct4, bits<3> funct3, string opcodestr,
                       list<dag> pattern = []> : GankowInstr {
  let OutOperandList = (outs GPR:$rd);
  let InOperandList  = (ins RS1:$rs1, immZExt5:$shamt);
  let AsmString = opcodestr # "\t$rd, $rs1, $shamt";
  let Pattern   = pattern;

  bits<7> rd; bits<8> rs1; bits<5> shamt;
  let Inst{6-0}   = 16;
  let Inst{13-7}  = rd;
  let Inst{21-14} = rs1;
  let Inst{24-22} = funct3;
  let Inst{37-33} = shamt;
  let Inst{38}    = 0;
  let Inst{32}    = 0;
  let Inst{45-39} = 0;
  let Inst{52-46} = 0;
  let Inst{56-53} = funct4;
}

// I-type, no immediate: rd = f(rs1). funct4 = 0101. The {38-32} field holds a
// fixed 7-bit opcode fragment (0 for sext/zext, 0b1 for clz, {frm,op} for
// fcvt). {52-46} = 0.
class GankowIConstType<bits<4> funct4, bits<3> funct3, bits<7> low7,
                       string opcodestr, list<dag> pattern = []> : GankowInstr {
  let OutOperandList = (outs GPR:$rd);
  let InOperandList  = (ins RS1:$rs1);
  let AsmString = opcodestr # "\t$rd, $rs1";
  let Pattern   = pattern;

  bits<7> rd; bits<8> rs1;
  let Inst{6-0}   = 16;
  let Inst{13-7}  = rd;
  let Inst{21-14} = rs1;
  let Inst{24-22} = funct3;
  let Inst{38-32} = low7;
  let Inst{45-39} = 0;
  let Inst{52-46} = 0;
  let Inst{56-53} = funct4;
}

// Load: rd = [rs1 + imm14]. opcode 001 0100 (20).
class GankowLoadType<bits<4> funct4, bits<3> funct3, string opcodestr,
                     list<dag> pattern = []> : GankowInstr {
  let OutOperandList = (outs GPR:$rd);
  let InOperandList  = (ins RS1:$rs1, immSExt14:$imm);
  let AsmString = opcodestr # "\t$rd, ${imm}(${rs1})";
  let Pattern   = pattern;
  let mayLoad = 1;
  let mayStore = 0;
  let hasSideEffects = 0;

  bits<7> rd; bits<8> rs1; bits<14> imm;
  let Inst{6-0}   = 20;
  let Inst{13-7}  = rd;
  let Inst{21-14} = rs1;
  let Inst{24-22} = funct3;
  let Inst{38-33} = imm{5-0};
  let Inst{32}    = imm{13};
  let Inst{45-39} = 0;        // attr7
  let Inst{52-46} = imm{12-6};
  let Inst{56-53} = funct4;
}

// Store: [rs1 + imm14] = rs2. opcode 010 0100 (36).
class GankowSType<bits<4> funct4, bits<3> funct3, string opcodestr,
                  list<dag> pattern = []> : GankowInstr {
  let OutOperandList = (outs);
  let InOperandList  = (ins RS1:$rs1, GPR:$rs2, immSExt14:$imm);
  let AsmString = opcodestr # "\t$rs2, ${imm}(${rs1})";
  let Pattern   = pattern;
  let mayLoad = 0;
  let mayStore = 1;
  let hasSideEffects = 1;

  bits<8> rs1; bits<7> rs2; bits<14> imm;
  let Inst{6-0}   = 36;
  let Inst{13-7}  = imm{12-6};
  let Inst{21-14} = rs1;
  let Inst{24-22} = funct3;
  let Inst{38-33} = imm{5-0};
  let Inst{32}    = imm{13};
  let Inst{45-39} = 0;        // attr7
  let Inst{52-46} = rs2;
  let Inst{56-53} = funct4;
}

// Atomic read-modify-write: rd = op([rs1], rs2). opcode 011 0100 (52). No imm.
class GankowAtomicType<bits<4> funct4, bits<3> funct3, string opcodestr,
                       list<dag> pattern = []> : GankowInstr {
  let OutOperandList = (outs GPR:$rd);
  let InOperandList  = (ins RS1:$rs1, GPR:$rs2);
  let AsmString = opcodestr # "\t$rd, $rs2, (${rs1})";
  let Pattern   = pattern;
  let mayLoad = 1;
  let mayStore = 1;
  let hasSideEffects = 1;

  bits<7> rd; bits<8> rs1; bits<7> rs2;
  let Inst{6-0}   = 52;
  let Inst{13-7}  = rd;
  let Inst{21-14} = rs1;
  let Inst{24-22} = funct3;
  let Inst{38-32} = 0;
  let Inst{45-39} = 0;        // attr7
  let Inst{52-46} = rs2;
  let Inst{56-53} = funct4;
}

// Branch with two source registers and a 21-bit signed offset (fork.*, lpbk.*).
// opcode 010 0000 (32).
class GankowBranchRegType<bits<4> funct4, bits<3> funct3, string opcodestr,
                          list<dag> pattern = []> : GankowInstr {
  let OutOperandList = (outs);
  let InOperandList  = (ins RS1:$rs1, GPR:$rs2, immSExt21:$imm);
  let AsmString = opcodestr # "\t$rs1, $rs2, $imm";
  let Pattern   = pattern;
  let hasSideEffects = 1;

  bits<8> rs1; bits<7> rs2; bits<21> imm;
  let Inst{6-0}   = 32;
  let Inst{13-7}  = imm{12-6};
  let Inst{21-14} = rs1;
  let Inst{24-22} = funct3;
  let Inst{38-33} = imm{5-0};
  let Inst{32}    = imm{20};
  let Inst{45-39} = imm{19-13};
  let Inst{52-46} = rs2;
  let Inst{56-53} = funct4;
}

// Branch with only a 21-bit signed offset (swch).
class GankowBranchImmType<bits<4> funct4, bits<3> funct3, string opcodestr,
                          list<dag> pattern = []> : GankowInstr {
  let OutOperandList = (outs);
  let InOperandList  = (ins immSExt21:$imm);
  let AsmString = opcodestr # "\t$imm";
  let Pattern   = pattern;
  let hasSideEffects = 1;

  bits<21> imm;
  let Inst{6-0}   = 32;
  let Inst{13-7}  = imm{12-6};
  let Inst{21-14} = 0;
  let Inst{24-22} = funct3;
  let Inst{38-33} = imm{5-0};
  let Inst{32}    = imm{20};
  let Inst{45-39} = imm{19-13};
  let Inst{52-46} = 0;
  let Inst{56-53} = funct4;
}

// Branch with two source registers, no offset (setp.*).
class GankowSetpType<bits<4> funct4, bits<3> funct3, string opcodestr,
                     list<dag> pattern = []> : GankowInstr {
  let OutOperandList = (outs);
  let InOperandList  = (ins RS1:$rs1, GPR:$rs2);
  let AsmString = opcodestr # "\t$rs1, $rs2";
  let Pattern   = pattern;
  let hasSideEffects = 1;

  bits<8> rs1; bits<7> rs2;
  let Inst{6-0}   = 32;
  let Inst{13-7}  = 0;
  let Inst{21-14} = rs1;
  let Inst{24-22} = funct3;
  let Inst{38-32} = 0;
  let Inst{45-39} = 0;
  let Inst{52-46} = rs2;
  let Inst{56-53} = funct4;
}

// Branch with no operands / no offset (join, lpst).
class GankowBranchNoOpType<bits<4> funct4, bits<3> funct3, string opcodestr>
    : GankowInstr {
  let OutOperandList = (outs);
  let InOperandList  = (ins);
  let AsmString = opcodestr;
  let Pattern = [];
  let hasSideEffects = 1;

  let Inst{6-0}   = 32;
  let Inst{13-7}  = 0;
  let Inst{21-14} = 0;
  let Inst{24-22} = funct3;
  let Inst{38-32} = 0;
  let Inst{45-39} = 0;
  let Inst{52-46} = 0;
  let Inst{56-53} = funct4;
}

// System, no immediate: opcode 000 0000 (0). The rs1 field {21-14} and the
// 7-bit field {13-7} are fixed per instruction (they select the operation).
class GankowSysNoImmType<bits<4> funct4, bits<8> rs1field, bits<7> low7,
                         string opcodestr> : GankowInstr {
  let OutOperandList = (outs);
  let InOperandList  = (ins);
  let AsmString = opcodestr;
  let Pattern = [];
  let hasSideEffects = 1;

  let Inst{6-0}   = 0;
  let Inst{13-7}  = low7;
  let Inst{21-14} = rs1field;
  let Inst{24-22} = 0;
  let Inst{38-32} = 0;
  let Inst{45-39} = 0;
  let Inst{52-46} = 0;
  let Inst{56-53} = funct4;
}

// System with a 24-bit signed immediate (jmp, call, alloc.sp, free.sp).
class GankowSysImmType<bits<4> funct4, bits<8> rs1field, bits<7> low7,
                       string opcodestr, list<dag> pattern = []> : GankowInstr {
  let OutOperandList = (outs);
  let InOperandList  = (ins immSExt24:$imm);
  let AsmString = opcodestr # "\t$imm";
  let Pattern   = pattern;
  let hasSideEffects = 1;

  bits<24> imm;
  let Inst{6-0}   = 0;
  let Inst{13-7}  = low7;
  let Inst{21-14} = rs1field;
  let Inst{24-22} = imm{22-20};
  let Inst{38-33} = imm{5-0};
  let Inst{32}    = imm{23};
  let Inst{45-39} = imm{19-13};
  let Inst{52-46} = imm{12-6};
  let Inst{56-53} = funct4;
}

// Uniform load-immediate: U[rd] = imm32. opcode 110 0000 (96).
class GankowUniLIType<bits<4> funct4, string opcodestr, list<dag> pattern = []>
    : GankowInstr {
  let OutOperandList = (outs UniformReg:$rd);
  let InOperandList  = (ins immSExt32:$imm);
  let AsmString = opcodestr # "\t$rd, $imm";
  let Pattern   = pattern;

  bits<7> rd; bits<32> imm;
  let Inst{6-0}   = 96;
  let Inst{13-7}  = rd;
  let Inst{21-14} = imm{30-23};
  let Inst{24-22} = imm{22-20};
  let Inst{38-33} = imm{5-0};
  let Inst{32}    = imm{31};
  let Inst{45-39} = imm{19-13};
  let Inst{52-46} = imm{12-6};
  let Inst{56-53} = funct4;
}

// Uniform add-immediate: U[rd] = R[rs1] + imm24. opcode 111 0000 (112).
class GankowUniIType<bits<4> funct4, string opcodestr, list<dag> pattern = []>
    : GankowInstr {
  let OutOperandList = (outs UniformReg:$rd);
  let InOperandList  = (ins RS1:$rs1, immSExt24:$imm);
  let AsmString = opcodestr # "\t$rd, $rs1, $imm";
  let Pattern   = pattern;

  bits<7> rd; bits<8> rs1; bits<24> imm;
  let Inst{6-0}   = 112;
  let Inst{13-7}  = rd;
  let Inst{21-14} = rs1;
  let Inst{24-22} = imm{22-20};
  let Inst{38-33} = imm{5-0};
  let Inst{32}    = imm{23};
  let Inst{45-39} = imm{19-13};
  let Inst{52-46} = imm{12-6};
  let Inst{56-53} = funct4;
}

// MAC (GK32A64): rd = +/-f(rs1, rs1+1) * f(rs2). opcode 011 1000 (56).
// rs1 is a 64-bit even register pair in the hardware; modelled as a GPR here
// (the even-pair constraint is not enforced by the register class yet).
class GankowMACType<bits<4> funct4, bits<3> funct3, bits<7> funct7,
                    string opcodestr, list<dag> pattern = []> : GankowInstr {
  let OutOperandList = (outs GPR:$rd);
  let InOperandList  = (ins GPR:$rs1, GPR:$rs2);
  let AsmString = opcodestr # "\t$rd, $rs1, $rs2";
  let Pattern   = pattern;

  bits<7> rd; bits<8> rs1; bits<7> rs2;
  let Inst{6-0}   = 56;
  let Inst{13-7}  = rd;
  let Inst{21-14} = rs1;
  let Inst{24-22} = funct3;
  let Inst{38-32} = funct7;
  let Inst{45-39} = 0;
  let Inst{52-46} = rs2;
  let Inst{56-53} = funct4;
}
