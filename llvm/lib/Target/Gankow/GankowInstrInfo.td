// GankowInstrInfo.td
//
// Included by Gankow.td, which includes Target.td, the register files, and
// GankowInstrFormats.td first. Do not re-include those here.
//
// This defines the full Gankow instruction set with the real encodings (see
// GankowInstrFormats.td) plus the SelectionDAG patterns that map integer
// arithmetic, logic, shifts, compares, and memory ops onto it.
//
// The operand fields, in the order each format class expects:
//   R-type : (outs GPR:$rd), (ins RS1:$rs1, GPR:$rs2)
//   I-type : (outs GPR:$rd), (ins RS1:$rs1, i32imm:$imm)
//   Load   : (outs GPR:$rd), (ins RS1:$rs1, i32imm:$imm)
//   Store  : (outs),         (ins RS1:$rs1, GPR:$rs2, i32imm:$imm)

//===----------------------------------------------------------------------===//
// R-type: arithmetic / logic / compare / multiply  (opcode 48)
//===----------------------------------------------------------------------===//

def ADD   : GankowRType<0, 0, 0, "add", [
            (set GPR:$rd, (add RS1:$rs1, GPR:$rs2))]>;
def SH1ADD  : GankowRType<0, 1, 0, "sh1add">;
def SH2ADD  : GankowRType<0, 2, 0, "sh2add">;
def SH3ADD  : GankowRType<0, 3, 0, "sh3add">;
def SH16ADD : GankowRType<0, 4, 0, "sh16add">;
def SR16ADD : GankowRType<0, 5, 0, "sr16add">;

def : Pat<(add (shl RS1:$rs1, (i32 1)), GPR:$rs2), (SH1ADD $rs1, $rs2)>;
def : Pat<(add (shl RS1:$rs1, (i32 2)), GPR:$rs2), (SH2ADD $rs1, $rs2)>;
def : Pat<(add (shl RS1:$rs1, (i32 3)), GPR:$rs2), (SH3ADD $rs1, $rs2)>;
def : Pat<(add (shl RS1:$rs1, (i32 16)), GPR:$rs2), (SH16ADD $rs1, $rs2)>;
def : Pat<(add (srl RS1:$rs1, (i32 16)), GPR:$rs2), (SR16ADD $rs1, $rs2)>;

def SUB   : GankowRType<0, 0, 1, "sub", [
            (set GPR:$rd, (sub RS1:$rs1, GPR:$rs2))]>;
def XOR   : GankowRType<1, 0, 0, "xor", [
            (set GPR:$rd, (xor RS1:$rs1, GPR:$rs2))]>;
def AND   : GankowRType<1, 1, 0, "and", [
            (set GPR:$rd, (and RS1:$rs1, GPR:$rs2))]>;
def OR    : GankowRType<1, 2, 0, "or ", [
            (set GPR:$rd, (or  RS1:$rs1, GPR:$rs2))]>;
def SLL   : GankowRType<2, 0, 0, "sll", [
            (set GPR:$rd, (shl RS1:$rs1, GPR:$rs2))]>;
def SRL   : GankowRType<2, 1, 0, "srl", [
            (set GPR:$rd, (srl RS1:$rs1, GPR:$rs2))]>;
def SRA   : GankowRType<2, 2, 0, "sra", [
            (set GPR:$rd, (sra RS1:$rs1, GPR:$rs2))]>;
def BEXT  : GankowRType<2, 7, 0, "bext">;

def : Pat<(and (srl RS1:$rs1, GPR:$rs2), (i32 1)), (BEXT $rs1, $rs2)>;

// Compares write 0/1 to rd.
def EQ    : GankowRType<3, 0, 0, "eq  ", [
            (set GPR:$rd, (i32 (seteq RS1:$rs1, GPR:$rs2)))]>;
def NE    : GankowRType<3, 4, 0, "ne  ", [
            (set GPR:$rd, (i32 (setne RS1:$rs1, GPR:$rs2)))]>;
def SLT   : GankowRType<3, 1, 0, "slt ", [
            (set GPR:$rd, (i32 (setlt RS1:$rs1, GPR:$rs2)))]>;
def SLTU  : GankowRType<3, 3, 0, "sltu", [
            (set GPR:$rd, (i32 (setult RS1:$rs1, GPR:$rs2)))]>;
def SGE   : GankowRType<3, 5, 0, "sge ", [
            (set GPR:$rd, (i32 (setge RS1:$rs1, GPR:$rs2)))]>;
def SGEU  : GankowRType<3, 7, 0, "sgeu", [
            (set GPR:$rd, (i32 (setuge RS1:$rs1, GPR:$rs2)))]>;

def MUL16U  : GankowRType<4, 0, 0, "mul16u  ">;
def MUL16HU : GankowRType<4, 1, 0, "mul16hu ">;
def MUL16HHU: GankowRType<4, 2, 0, "mul16hhu">;
def MUL16HSU: GankowRType<4, 3, 0, "mul16hsu">;
def MUL16HH : GankowRType<4, 4, 0, "mul16hh ">;

//===----------------------------------------------------------------------===//
// I-type: immediate arithmetic / logic / compare  (opcode 16)
//===----------------------------------------------------------------------===//

def ADDI  : GankowIType<0, 0, "addi",
            [(set GPR:$rd, (add RS1:$rs1, immSExt14:$imm))]>;
def XORI  : GankowIType<1, 0, "xori",
            [(set GPR:$rd, (xor RS1:$rs1, immSExt14:$imm))]>;
def ANDI  : GankowIType<1, 1, "andi",
            [(set GPR:$rd, (and RS1:$rs1, immSExt14:$imm))]>;
def ORI   : GankowIType<1, 2, "ori ",
            [(set GPR:$rd, (or  RS1:$rs1, immSExt14:$imm))]>;
def EQI   : GankowIType<3, 0, "eqi ",
            [(set GPR:$rd, (i32 (seteq RS1:$rs1, immSExt14:$imm)))]>;
def NEI   : GankowIType<3, 4, "nei ",
            [(set GPR:$rd, (i32 (setne RS1:$rs1, immSExt14:$imm)))]>;
def SLTI  : GankowIType<3, 1, "slti",
            [(set GPR:$rd, (i32 (setlt RS1:$rs1, immSExt14:$imm)))]>;
def SLTIU : GankowIType<3, 3, "sltiu",
            [(set GPR:$rd, (i32 (setult RS1:$rs1, immSExt14:$imm)))]>;
def SGEI  : GankowIType<3, 5, "sgei",
            [(set GPR:$rd, (i32 (setge RS1:$rs1, immSExt14:$imm)))]>;
def SGEIU : GankowIType<3, 7, "sgeiu",
            [(set GPR:$rd, (i32 (setuge RS1:$rs1, immSExt14:$imm)))]>;

// I-type shifts: 5-bit shift amount.
def SLLI  : GankowIShiftType<2, 0, "slli",
            [(set GPR:$rd, (shl RS1:$rs1, immZExt5:$shamt))]>;
def SRLI  : GankowIShiftType<2, 1, "srli",
            [(set GPR:$rd, (srl RS1:$rs1, immZExt5:$shamt))]>;
def SRAI  : GankowIShiftType<2, 2, "srai",
            [(set GPR:$rd, (sra RS1:$rs1, immZExt5:$shamt))]>;
def BEXTI : GankowIShiftType<2, 7, "bexti", []>;
def : Pat<(and (srl RS1:$rs1, immZExt5:$shamt), (i32 1)), (BEXTI $rs1, $shamt)>;

// I-type, no immediate (funct4 = 0101).
def SEXT_B: GankowIConstType<5, 1, 0x00, "sext.b",
            [(set GPR:$rd, (sext_inreg RS1:$rs1, i8))]>;
def SEXT_H: GankowIConstType<5, 2, 0x00, "sext.h",
            [(set GPR:$rd, (sext_inreg RS1:$rs1, i16))]>;
def ZEXT_H: GankowIConstType<5, 6, 0x00, "zext.h">;
def CLZ   : GankowIConstType<5, 0, 0x01, "clz",
            [(set GPR:$rd, (ctlz RS1:$rs1))]>;

def : Pat<(and RS1:$rs1, 0xFFFF), (ZEXT_H RS1:$rs1)>;
def : InstAlias<"zext.b $rd, $rs1", (ANDI GPR:$rd, RS1:$rs1, 0xFF)>;

//===----------------------------------------------------------------------===//
// Loads (opcode 20)
//===----------------------------------------------------------------------===//

def LW  : GankowLoadType<0, 0, "lw ", []>;
def LB  : GankowLoadType<0, 1, "lb ", []>;
def LH  : GankowLoadType<0, 2, "lh ", []>;
def LBU : GankowLoadType<0, 5, "lbu", []>;
def LHU : GankowLoadType<0, 6, "lhu", []>;

//===----------------------------------------------------------------------===//
// Stores (opcode 36)
//===----------------------------------------------------------------------===//

def SW  : GankowSType<0, 0, "sw", []>;
def SB  : GankowSType<0, 1, "sb", []>;
def SH  : GankowSType<0, 2, "sh", []>;

// Load/store patterns. The address is either a bare base register or a
// base + immediate offset, matching Gankow's single base+offset addresser.

def : Pat<(load (add RS1:$rs1, immSExt14:$imm)), (LW RS1:$rs1, immSExt14:$imm)>;
def : Pat<(sextloadi8 (add RS1:$rs1, immSExt14:$imm)), (LB RS1:$rs1, immSExt14:$imm)>;
def : Pat<(extloadi8 (add RS1:$rs1, immSExt14:$imm)), (LBU RS1:$rs1, immSExt14:$imm)>;
def : Pat<(zextloadi8 (add RS1:$rs1, immSExt14:$imm)), (LBU RS1:$rs1, immSExt14:$imm)>;
def : Pat<(sextloadi16 (add RS1:$rs1, immSExt14:$imm)), (LH RS1:$rs1, immSExt14:$imm)>;
def : Pat<(extloadi16 (add RS1:$rs1, immSExt14:$imm)), (LHU RS1:$rs1, immSExt14:$imm)>;
def : Pat<(zextloadi16 (add RS1:$rs1, immSExt14:$imm)), (LHU RS1:$rs1, immSExt14:$imm)>;

def : Pat<(store (i32 GPR:$rs2), (add RS1:$rs1, immSExt14:$imm)), (SW RS1:$rs1, GPR:$rs2, immSExt14:$imm)>;
def : Pat<(truncstorei8 (i32 GPR:$rs2), (add RS1:$rs1, immSExt14:$imm)), (SB RS1:$rs1, GPR:$rs2, immSExt14:$imm)>;
def : Pat<(truncstorei16 (i32 GPR:$rs2), (add RS1:$rs1, immSExt14:$imm)), (SH RS1:$rs1, GPR:$rs2, immSExt14:$imm)>;

//===----------------------------------------------------------------------===//
// Divergence / control flow (opcode 32)
//===----------------------------------------------------------------------===//

// fork.* : conditional warp branch, two sources + offset.
def FORK_EQ : GankowBranchRegType<0, 0, "fork.eq $rs1, $rs2, $imm", []>;
def FORK_LT : GankowBranchRegType<0, 1, "fork.lt $rs1, $rs2, $imm", []>;
def FORK_LTU: GankowBranchRegType<0, 3, "fork.ltu $rs1, $rs2, $imm", []>;
def FORK_NE : GankowBranchRegType<0, 4, "fork.ne $rs1, $rs2, $imm", []>;
def FORK_GE : GankowBranchRegType<0, 5, "fork.ge $rs1, $rs2, $imm", []>;
def FORK_GEU: GankowBranchRegType<0, 7, "fork.geu $rs1, $rs2, $imm", []>;
def SWCH : GankowBranchImmType<0, 2, "swch $imm", []>;
def JOIN : GankowBranchNoOpType<0, 6, "join">;

// lpbk.* : loop-break, two sources + offset.
def LPBK_EQ : GankowBranchRegType<1, 0, "lpbk.eq $rs1, $rs2, $imm", []>;
def LPBK_LT : GankowBranchRegType<1, 1, "lpbk.lt $rs1, $rs2, $imm", []>;
def LPBK_LTU: GankowBranchRegType<1, 3, "lpbk.ltu $rs1, $rs2, $imm", []>;
def LPBK_NE : GankowBranchRegType<1, 4, "lpbk.ne $rs1, $rs2, $imm", []>;
def LPBK_GE : GankowBranchRegType<1, 5, "lpbk.ge $rs1, $rs2, $imm", []>;
def LPBK_GEU: GankowBranchRegType<1, 7, "lpbk.geu $rs1, $rs2, $imm", []>;
def LPST : GankowBranchNoOpType<1, 2, "lpst">;

// setp.* : compare and set predicate (flag), two sources, no offset.
def SETP_EQ : GankowSetpType<3, 0, "setp.eq $rs1, $rs2", []>;
def SETP_LT : GankowSetpType<3, 1, "setp.lt $rs1, $rs2", []>;
def SETP_LTU: GankowSetpType<3, 3, "setp.ltu $rs1, $rs2", []>;
def SETP_NE : GankowSetpType<3, 4, "setp.ne $rs1, $rs2", []>;
def SETP_GE : GankowSetpType<3, 5, "setp.ge $rs1, $rs2", []>;
def SETP_GEU: GankowSetpType<3, 7, "setp.geu $rs1, $rs2", []>;

//===----------------------------------------------------------------------===//
// System (opcode 0)
//===----------------------------------------------------------------------===//

// exit: terminate the kernel.
let isTerminator = 1, isBarrier = 1 in
  def EXIT : GankowSysNoImmType<0, 0x00, 0x00, "exit">;

// jmp / call: 24-bit relative transfer.
let isBranch = 1, isTerminator = 1, isBarrier = 1 in
  def JMP  : GankowSysImmType<0, 0x80, 0x01, "jmp $imm", []>;
let isCall = 1, isBranch = 1, isTerminator = 1, isBarrier = 1 in
  def CALL : GankowSysImmType<1, 0x80, 0x01, "call $imm", []>;

// ret: return from function.
let isReturn = 1, isTerminator = 1, isBarrier = 1 in
  def RET : GankowSysNoImmType<1, 0x80, 0x03, "ret">;

// def GankowRet : SDNode<"GankowISD::Ret", SDTNone, 
//                 [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

// stcf / ldcf: save / restore the call frame.
def STCF : GankowSysNoImmType<1, 0xA0, 0x10, "stcf">;
def LDCF : GankowSysNoImmType<1, 0xA0, 0x11, "ldcf">;

// alloc.sp / free.sp: grow / shrink the thread-local stack.
def ALLOC_SP : GankowSysImmType<2, 0xA1, 0x00, "alloc.sp $imm", []>;
def FREE_SP  : GankowSysImmType<2, 0xA1, 0x01, "free.sp $imm", []>;

//===----------------------------------------------------------------------===//
// Uniform register ops
//===----------------------------------------------------------------------===//

def LI_U   : GankowUniLIType<0, "li.u   $rd, $imm", []>;
def ADDI_U : GankowUniIType<0, "addi.u", []>;

def : Pat<(immSExt14:$imm), (ORI U0, $imm)>;
def : Pat<(immSExt32:$imm), (LI_U $imm)>;

//===----------------------------------------------------------------------===//
// Atomic read-modify-write (opcode 52)
//===----------------------------------------------------------------------===//

def AMOADD  : GankowAtomicType<0, 0, "amoadd", []>;
def AMOMAX  : GankowAtomicType<0, 1, "amomax", []>;
def AMOMIN  : GankowAtomicType<0, 2, "amomin", []>;
def AMOSWAP : GankowAtomicType<0, 4, "amoswap", []>;
def AMOAND  : GankowAtomicType<0, 5, "amoand", []>;
def AMOOR   : GankowAtomicType<0, 6, "amoor ", []>;
def AMOXOR  : GankowAtomicType<0, 7, "amoxor", []>;

//===----------------------------------------------------------------------===//
// Floating point (fp32 in GPRs), frm = FAST (0b111)
//===----------------------------------------------------------------------===//

def FADD_S : GankowRType<7, 0, 0x70, "fadd.s", []>;
def FSUB_S : GankowRType<7, 0, 0x71, "fsub.s", []>;
def FMUL_S : GankowRType<7, 0, 0x72, "fmul.s", []>;
def FLT_S  : GankowRType<7, 0, 0x73, "flt.s", []>;
def FLE_S  : GankowRType<7, 0, 0x74, "fle.s", []>;

def FCVT_S_W : GankowIConstType<5, 0, 0x72, "fcvt.s.w", []>;
def FCVT_W_S : GankowIConstType<5, 0, 0x73, "fcvt.w.s", []>;

//===----------------------------------------------------------------------===//
// Multiply-accumulate, GK32A64 (opcode 56)
//===----------------------------------------------------------------------===//

def FMACC_S  : GankowMACType<0, 0, 0, "fmacc.s", []>;
def FNMACC_S : GankowMACType<0, 0, 1, "fnmacc.s", []>;
