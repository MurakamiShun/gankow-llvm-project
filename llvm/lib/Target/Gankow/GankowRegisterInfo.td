// GankowRegisterInfo.td
// Register definitions for the Gankow architecture (see isa/RegisterMap.md).
//
// The register file:
//   * 128 general-purpose registers R0-R127, 32 bits each. Adjacent even/odd
//     pairs act as one 64-bit register (lower number = LSB).
//   * An 8-bit rs1 field selects an operand: bit 7 = 0 -> GPR R[bit6:0];
//     bit 7 = 1 -> a *special* register whose number is bit6:0. So special
//     registers encode as HWEncoding = 0x80 + number. The rd (7-bit) and
//     rs2 (7-bit) fields are GPR-only (R0-R127).
//   * Special registers (number -> name), all readable through rs1:
//       0-7     u0-u7    (u0 hardwired zero; u1-u7 warp-shared, writable)
//       16-31   tid/tdim/bid/bdim .x/.y/.z  (read-only)
//       32      cfsp     warp-local call-frame stack pointer
//       33      sp       thread-local stack pointer
//       34      gp       kernel-local read-only global pointer
//   * Execution-context virtual registers pc/cf and the 1-bit flags prdc/carry
//     are not part of the rs1 file (HWEncodings below are sentinels, never
//     emitted in an instruction field).
//
// GPR HWEncoding == register number (0-127); special HWEncoding == 0x80+number.

// GPR HWEncoding == register number (0-127); special HWEncoding == 0x80+number.

let Namespace = "Gankow" in {
  class GankowReg<bits<8> Enc, string n, list<string> alt = []> : Register<n> {
    let HWEncoding{7-0} = Enc;
    let AltNames = alt;
  }

  // =============================================================================
  // General Purpose Registers (R0 - R127), 32 bits each
  // =============================================================================
  //   R0-3    a0-3    caller     R16-23   s4-11   callee
  //   R4-5    s0-1    callee     R24-31   t4-11   caller
  //   R6-7    t0-1    caller     R32-63   t12-43  caller
  //   R8-11   a4-7    caller     R64-127  s12-75  callee
  //   R12-13  s2-3    callee
  //   R14-15  t2-3    caller

  def R0  : GankowReg<0,  "r0",  ["a0"]>;
  def R1  : GankowReg<1,  "r1",  ["a1"]>;
  def R2  : GankowReg<2,  "r2",  ["a2"]>;
  def R3  : GankowReg<3,  "r3",  ["a3"]>;
  def R4  : GankowReg<4,  "r4",  ["s0"]>;
  def R5  : GankowReg<5,  "r5",  ["s1"]>;
  def R6  : GankowReg<6,  "r6",  ["t0"]>;
  def R7  : GankowReg<7,  "r7",  ["t1"]>;
  def R8  : GankowReg<8,  "r8",  ["a4"]>;
  def R9  : GankowReg<9,  "r9",  ["a5"]>;
  def R10 : GankowReg<10, "r10", ["a6"]>;
  def R11 : GankowReg<11, "r11", ["a7"]>;
  def R12 : GankowReg<12, "r12", ["s2"]>;
  def R13 : GankowReg<13, "r13", ["s3"]>;
  def R14 : GankowReg<14, "r14", ["t2"]>;
  def R15 : GankowReg<15, "r15", ["t3"]>;
  foreach i = 16-23 in {
    def "R" # i : GankowReg<i, "r" # i, ["s" # !add(i, -12)]>;
  }
  foreach i = 24-63 in {
    def "R" # i : GankowReg<i, "r" # i, ["t" # !add(i, -20)]>;
  }
  foreach i = 64-127 in {
    def "R" # i : GankowReg<i, "r" # i, ["s" # !add(i, -52)]>;
  }

  // =============================================================================
  // Uniform Registers (rs1 number 0-7; HWEnc = 0x80 + number)
  // =============================================================================
  // u0 is hardwired zero (read-only); u1-u7 are warp-shared and writable.
  let isConstant = true in
  def U0 : GankowReg<0x80, "u0", ["zero"]>;  // number 0
  def U1 : GankowReg<0x81, "u1">;
  def U2 : GankowReg<0x82, "u2">;
  def U3 : GankowReg<0x83, "u3">;
  def U4 : GankowReg<0x84, "u4">;
  def U5 : GankowReg<0x85, "u5">;
  def U6 : GankowReg<0x86, "u6">;
  def U7 : GankowReg<0x87, "u7">;

  // =============================================================================
  // Pointers (rs1 number 8-10)
  // =============================================================================
  def CFSP : GankowReg<0x88, "cfsp">;  // number 8
  def SP   : GankowReg<0x89, "sp">;    // number 9
  def GP   : GankowReg<0x8A, "gp">;    // number 10

  // =============================================================================
  // Thread-dimension system registers (rs1 number 16-31, read-only)
  // =============================================================================
  let isConstant = true in {
    def TID_X  : GankowReg<0x90, "tid.x">;   // number 16
    def TID_Y  : GankowReg<0x91, "tid.y">;   // number 17
    def TID_Z  : GankowReg<0x92, "tid.z">;   // number 18
    def TDIM_X : GankowReg<0x94, "tdim.x">;  // number 20
    def TDIM_Y : GankowReg<0x95, "tdim.y">;  // number 21
    def TDIM_Z : GankowReg<0x96, "tdim.z">;  // number 22
    def BID_X  : GankowReg<0x98, "bid.x">;   // number 24
    def BID_Y  : GankowReg<0x99, "bid.y">;   // number 25
    def BID_Z  : GankowReg<0x9A, "bid.z">;   // number 26
    def BDIM_X : GankowReg<0x9C, "bdim.x">;  // number 28
    def BDIM_Y : GankowReg<0x9D, "bdim.y">;  // number 29
    def BDIM_Z : GankowReg<0x9E, "bdim.z">;  // number 30
  }

  // =============================================================================
  // Execution-context / flag virtual registers (not in the rs1 file).
  // these registers are never emitted as an instruction field operand.
  // =============================================================================
  def PRDC  : GankowReg<0, "prdc">;   // predicate flag (1 bit, caller-saved)
  def CARRY : GankowReg<0, "carry">;  // carry flag   (1 bit, caller-saved)
  def PC    : GankowReg<0, "pc">;     // current program counter
  def CF    : GankowReg<0, "cf">;     // warp-local call frame (call/ret/stcf/ldcf)

}
