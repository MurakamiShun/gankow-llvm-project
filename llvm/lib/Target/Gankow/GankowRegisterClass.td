// GankowRegisterClass.td
//
// Register classes for Gankow. Data and pointers are 32-bit in the base ISA.
//
// Field/register-class mapping (from InstructionType.md):
//   rd  (7-bit)  -> GPR (R0-R127)
//   rs2 (7-bit)  -> GPR (R0-R127)
//   rs1 (8-bit)  -> RS1 (any GPR or a special register)

let Namespace = "Gankow" in {
class GankowRegisterClass<dag regList, list<ValueType> types = [i32], int align = 4>
    : RegisterClass<"Gankow", types, align, regList> {}

// The 128 general-purpose registers.
// The order of registers represents the preferred allocation sequence.
// Registers are listed in the order caller-save, callee-save, specials.
def GPR : GankowRegisterClass<(add
              (sequence "R%u", 0, 3),    // a0-3
              (sequence "R%u", 6, 7),    // t0-1
              (sequence "R%u", 4, 5),    // s0-1
              (sequence "R%u", 8, 11),   // a4-7
              (sequence "R%u", 14, 15),  // t2-3
              (sequence "R%u", 12, 13),  // s2-3
              (sequence "R%u", 24, 31),   // t4-11
              (sequence "R%u", 16, 23),  // s4-11
              (sequence "R%u", 32, 63),   // t12-43
              (sequence "R%u", 64, 127)  // s12-75
            )> {
}

def GPRCallerSaved : GankowRegisterClass<(add
              (sequence "R%u", 0, 3),    // a0-3
              (sequence "R%u", 6, 7),    // t0-1
              (sequence "R%u", 8, 11),   // a4-7
              (sequence "R%u", 14, 15),  // t2-3
              (sequence "R%u", 24, 63)   // t4-43
            )> {
}

def GPRCalleeSaved : GankowRegisterClass<(add
              (sequence "R%u", 4, 5),    // s0-1
              (sequence "R%u", 12, 13),  // s2-3
              (sequence "R%u", 16, 23),  // s4-11
              (sequence "R%u", 64, 127)  // s12-75
            )> {
}

// Uniform registers (u0 hardwired zero; u1-u7 warp-shared). A separate file
// written only by the uniform (li.u/addi.u) instructions, so not allocatable
// by the GPR allocator.
def UniformReg : GankowRegisterClass<(sequence "U%u", 0, 7)> {
  let isAllocatable = 0;
}

// Thread-dimension system registers (read-only, not allocatable).
def ThreadDimReg : GankowRegisterClass<(add
      TID_X, TID_Y, TID_Z,
      TDIM_X, TDIM_Y, TDIM_Z,
      BID_X, BID_Y, BID_Z,
      BDIM_X, BDIM_Y, BDIM_Z)> {
  let isAllocatable = 0;
}

// Pointers: cfsp / sp / gp (not allocatable; sp/cfsp only via stack ops).
def PtrRegClass : GankowRegisterClass<(add CFSP, SP, GP)> {
  let isAllocatable = 0;
}

// Flag registers (1 bit, caller-saved, not allocatable).
def FlagRegClass : GankowRegisterClass<(add PRDC, CARRY), [i1], 1> {
  let isAllocatable = 0;
}

// Execution-context virtual registers (not allocatable).
def ExecCtxClass : GankowRegisterClass<(add PC, CF)> {
  let isAllocatable = 0;
}

// The 8-bit rs1 operand field: a GPR or a special register.
def RS1 : GankowRegisterClass<(add
      GPR, UniformReg, ThreadDimReg, PtrRegClass)> {
}
}