#pragma once

namespace wasmrt {
namespace parser {
namespace bytecode {

enum BytecodeOp {
	Unreachable       = 0x00 // unreachable
	Nop               = 0x01 // nop
	Block             = 0x02 // block rt in* end
	Loop              = 0x03 // loop rt in* end
	If                = 0x04 // if rt in* else in* end
	Else_             = 0x05 // else
	End_              = 0x0B // end
	Br                = 0x0C // br l
	BrIf              = 0x0D // br_if l
	BrTable           = 0x0E // br_table l* lN
	Return            = 0x0F // return
	Call              = 0x10 // call x
	CallIndirect      = 0x11 // call_indirect x
	Drop              = 0x1A // drop
	Select            = 0x1B // select
	LocalGet          = 0x20 // local.get x
	LocalSet          = 0x21 // local.set x
	LocalTee          = 0x22 // local.tee x
	GlobalGet         = 0x23 // global.get x
	GlobalSet         = 0x24 // global.set x
	I32Load           = 0x28 // i32.load m
	I64Load           = 0x29 // i64.load m
	F32Load           = 0x2A // f32.load m
	F64Load           = 0x2B // f64.load m
	I32Load8S         = 0x2C // i32.load8_s m
	I32Load8U         = 0x2D // i32.load8_u m
	I32Load16S        = 0x2E // i32.load16_s m
	I32Load16U        = 0x2F // i32.load16_u m
	I64Load8S         = 0x30 // i64.load8_s m
	I64Load8U         = 0x31 // i64.load8_u m
	I64Load16S        = 0x32 // i64.load16_s m
	I64Load16U        = 0x33 // i64.load16_u m
	I64Load32S        = 0x34 // i64.load32_s m
	I64Load32U        = 0x35 // i64.load32_u m
	I32Store          = 0x36 // i32.store m
	I64Store          = 0x37 // i64.store m
	F32Store          = 0x38 // f32.store m
	F64Store          = 0x39 // f64.store m
	I32Store8         = 0x3A // i32.store8 m
	I32Store16        = 0x3B // i32.store16 m
	I64Store8         = 0x3C // i64.store8 m
	I64Store16        = 0x3D // i64.store16 m
	I64Store32        = 0x3E // i64.store32 m
	MemorySize        = 0x3F // memory.size
	MemoryGrow        = 0x40 // memory.grow
	I32Const          = 0x41 // i32.const n
	I64Const          = 0x42 // i64.const n
	F32Const          = 0x43 // f32.const z
	F64Const          = 0x44 // f64.const z
	I32Eqz            = 0x45 // i32.eqz
	I32Eq             = 0x46 // i32.eq
	I32Ne             = 0x47 // i32.ne
	I32LtS            = 0x48 // i32.lt_s
	I32LtU            = 0x49 // i32.lt_u
	I32GtS            = 0x4A // i32.gt_s
	I32GtU            = 0x4B // i32.gt_u
	I32LeS            = 0x4C // i32.le_s
	I32LeU            = 0x4D // i32.le_u
	I32GeS            = 0x4E // i32.ge_s
	I32GeU            = 0x4F // i32.ge_u
	I64Eqz            = 0x50 // i64.eqz
	I64Eq             = 0x51 // i64.eq
	I64Ne             = 0x52 // i64.ne
	I64LtS            = 0x53 // i64.lt_s
	I64LtU            = 0x54 // i64.lt_u
	I64GtS            = 0x55 // i64.gt_s
	I64GtU            = 0x56 // i64.gt_u
	I64LeS            = 0x57 // i64.le_s
	I64LeU            = 0x58 // i64.le_u
	I64GeS            = 0x59 // i64.ge_s
	I64GeU            = 0x5A // i64.ge_u
	F32Eq             = 0x5B // f32.eq
	F32Ne             = 0x5C // f32.ne
	F32Lt             = 0x5D // f32.lt
	F32Gt             = 0x5E // f32.gt
	F32Le             = 0x5F // f32.le
	F32Ge             = 0x60 // f32.ge
	F64Eq             = 0x61 // f64.eq
	F64Ne             = 0x62 // f64.ne
	F64Lt             = 0x63 // f64.lt
	F64Gt             = 0x64 // f64.gt
	F64Le             = 0x65 // f64.le
	F64Ge             = 0x66 // f64.ge
	I32Clz            = 0x67 // i32.clz
	I32Ctz            = 0x68 // i32.ctz
	I32PopCnt         = 0x69 // i32.popcnt
	I32Add            = 0x6A // i32.add
	I32Sub            = 0x6B // i32.sub
	I32Mul            = 0x6C // i32.mul
	I32DivS           = 0x6D // i32.div_s
	I32DivU           = 0x6E // i32.div_u
	I32RemS           = 0x6F // i32.rem_s
	I32RemU           = 0x70 // i32.rem_u
	I32And            = 0x71 // i32.and
	I32Or             = 0x72 // i32.or
	I32Xor            = 0x73 // i32.xor
	I32Shl            = 0x74 // i32.shl
	I32ShrS           = 0x75 // i32.shr_s
	I32ShrU           = 0x76 // i32.shr_u
	I32Rotl           = 0x77 // i32.rotl
	I32Rotr           = 0x78 // i32.rotr
	I64Clz            = 0x79 // i64.clz
	I64Ctz            = 0x7A // i64.ctz
	I64PopCnt         = 0x7B // i64.popcnt
	I64Add            = 0x7C // i64.add
	I64Sub            = 0x7D // i64.sub
	I64Mul            = 0x7E // i64.mul
	I64DivS           = 0x7F // i64.div_s
	I64DivU           = 0x80 // i64.div_u
	I64RemS           = 0x81 // i64.rem_s
	I64RemU           = 0x82 // i64.rem_u
	I64And            = 0x83 // i64.and
	I64Or             = 0x84 // i64.or
	I64Xor            = 0x85 // i64.xor
	I64Shl            = 0x86 // i64.shl
	I64ShrS           = 0x87 // i64.shr_s
	I64ShrU           = 0x88 // i64.shr_u
	I64Rotl           = 0x89 // i64.rotl
	I64Rotr           = 0x8A // i64.rotr
	F32Abs            = 0x8B // f32.abs
	F32Neg            = 0x8C // f32.neg
	F32Ceil           = 0x8D // f32.ceil
	F32Floor          = 0x8E // f32.floor
	F32Trunc          = 0x8F // f32.trunc
	F32Nearest        = 0x90 // f32.nearest
	F32Sqrt           = 0x91 // f32.sqrt
	F32Add            = 0x92 // f32.add
	F32Sub            = 0x93 // f32.sub
	F32Mul            = 0x94 // f32.mul
	F32Div            = 0x95 // f32.div
	F32Min            = 0x96 // f32.min
	F32Max            = 0x97 // f32.max
	F32CopySign       = 0x98 // f32.copysign
	F64Abs            = 0x99 // f64.abs
	F64Neg            = 0x9A // f64.neg
	F64Ceil           = 0x9B // f64.ceil
	F64Floor          = 0x9C // f64.floor
	F64Trunc          = 0x9D // f64.trunc
	F64Nearest        = 0x9E // f64.nearest
	F64Sqrt           = 0x9F // f64.sqrt
	F64Add            = 0xA0 // f64.add
	F64Sub            = 0xA1 // f64.sub
	F64Mul            = 0xA2 // f64.mul
	F64Div            = 0xA3 // f64.div
	F64Min            = 0xA4 // f64.min
	F64Max            = 0xA5 // f64.max
	F64CopySign       = 0xA6 // f64.copysign
	I32WrapI64        = 0xA7 // i32.wrap_i64
	I32TruncF32S      = 0xA8 // i32.trunc_f32_s
	I32TruncF32U      = 0xA9 // i32.trunc_f32_u
	I32TruncF64S      = 0xAA // i32.trunc_f64_s
	I32TruncF64U      = 0xAB // i32.trunc_f64_u
	I64ExtendI32S     = 0xAC // i64.extend_i32_s
	I64ExtendI32U     = 0xAD // i64.extend_i32_u
	I64TruncF32S      = 0xAE // i64.trunc_f32_s
	I64TruncF32U      = 0xAF // i64.trunc_f32_u
	I64TruncF64S      = 0xB0 // i64.trunc_f64_s
	I64TruncF64U      = 0xB1 // i64.trunc_f64_u
	F32ConvertI32S    = 0xB2 // f32.convert_i32_s
	F32ConvertI32U    = 0xB3 // f32.convert_i32_u
	F32ConvertI64S    = 0xB4 // f32.convert_i64_s
	F32ConvertI64U    = 0xB5 // f32.convert_i64_u
	F32DemoteF64      = 0xB6 // f32.demote_f64
	F64ConvertI32S    = 0xB7 // f64.convert_i32_s
	F64ConvertI32U    = 0xB8 // f64.convert_i32_u
	F64ConvertI64S    = 0xB9 // f64.convert_i64_s
	F64ConvertI64U    = 0xBA // f64.convert_i64_u
	F64PromoteF32     = 0xBB // f64.promote_f32
	I32ReinterpretF32 = 0xBC // i32.reinterpret_f32
	I64ReinterpretF64 = 0xBD // i64.reinterpret_f64
	F32ReinterpretI32 = 0xBE // f32.reinterpret_i32
	F64ReinterpretI64 = 0xBF // f64.reinterpret_i64
	I32Extend8S       = 0xC0 // i32.extend8_s
	I32Extend16S      = 0xC1 // i32.extend16_s
	I64Extend8S       = 0xC2 // i64.extend8_s
	I64Extend16S      = 0xC3 // i64.extend16_s
	I64Extend32S      = 0xC4 // i64.extend32_s
	TruncSat          = 0xFC // <i32|64>.trunc_sat_<f32|64>_<s|u>
};

const char *OpNames[TruncSat + 1] = {
	[Unreachable] = "unreachable",
	[Nop] = "nop",
	[Block] = "block",
	[Loop] = "loop",
	[If] = "if",
	[Else_] = "else",
	[End_] = "end",
	[Br] = "br",
	[BrIf] = "br_if",
	[BrTable] = "br_table",
	[Return] = "return",
	[Call] = "call",
	[CallIndirect] = "call_indirect",
	[Drop] = "drop",
	[Select] = "select",
	[LocalGet] = "local.get",
	[LocalSet] = "local.set",
	[LocalTee] = "local.tee",
	[GlobalGet] = "global.get",
	[GlobalSet] = "global.set",
	[I32Load] = "i32.load",
	[I64Load] = "i64.load",
	[F32Load] = "f32.load",
	[F64Load] = "f64.load",
	[I32Load8S] = "i32.load8_s",
	[I32Load8U] = "i32.load8_u",
	[I32Load16S] = "i32.load16_s",
	[I32Load16U] = "i32.load16_u",
	[I64Load8S] = "i64.load8_s",
	[I64Load8U] = "i64.load8_u",
	[I64Load16S] = "i64.load16_s",
	[I64Load16U] = "i64.load16_u",
	[I64Load32S] = "i64.load32_s",
	[I64Load32U] = "i64.load32_u",
	[I32Store] = "i32.store",
	[I64Store] = "i64.store",
	[F32Store] = "f32.store",
	[F64Store] = "f64.store",
	[I32Store8] = "i32.store8",
	[I32Store16] = "i32.store16",
	[I64Store8] = "i64.store8",
	[I64Store16] = "i64.store16",
	[I64Store32] = "i64.store32",
	[MemorySize] = "memory.size",
	[MemoryGrow] = "memory.grow",
	[I32Const] = "i32.const",
	[I64Const] = "i64.const",
	[F32Const] = "f32.const",
	[F64Const] = "f64.const",
	[I32Eqz] = "i32.eqz",
	[I32Eq] = "i32.eq",
	[I32Ne] = "i32.ne",
	[I32LtS] = "i32.lt_s",
	[I32LtU] = "i32.lt_u",
	[I32GtS] = "i32.gt_s",
	[I32GtU] = "i32.gt_u",
	[I32LeS] = "i32.le_s",
	[I32LeU] = "i32.le_u",
	[I32GeS] = "i32.ge_s",
	[I32GeU] = "i32.ge_u",
	[I64Eqz] = "i64.eqz",
	[I64Eq] = "i64.eq",
	[I64Ne] = "i64.ne",
	[I64LtS] = "i64.lt_s",
	[I64LtU] = "i64.lt_u",
	[I64GtS] = "i64.gt_s",
	[I64GtU] = "i64.gt_u",
	[I64LeS] = "i64.le_s",
	[I64LeU] = "i64.le_u",
	[I64GeS] = "i64.ge_s",
	[I64GeU] = "i64.ge_u",
	[F32Eq] = "f32.eq",
	[F32Ne] = "f32.ne",
	[F32Lt] = "f32.lt",
	[F32Gt] = "f32.gt",
	[F32Le] = "f32.le",
	[F32Ge] = "f32.ge",
	[F64Eq] = "f64.eq",
	[F64Ne] = "f64.ne",
	[F64Lt] = "f64.lt",
	[F64Gt] = "f64.gt",
	[F64Le] = "f64.le",
	[F64Ge] = "f64.ge",
	[I32Clz] = "i32.clz",
	[I32Ctz] = "i32.ctz",
	[I32PopCnt] = "i32.popcnt",
	[I32Add] = "i32.add",
	[I32Sub] = "i32.sub",
	[I32Mul] = "i32.mul",
	[I32DivS] = "i32.div_s",
	[I32DivU] = "i32.div_u",
	[I32RemS] = "i32.rem_s",
	[I32RemU] = "i32.rem_u",
	[I32And] = "i32.and",
	[I32Or] = "i32.or",
	[I32Xor] = "i32.xor",
	[I32Shl] = "i32.shl",
	[I32ShrS] = "i32.shr_s",
	[I32ShrU] = "i32.shr_u",
	[I32Rotl] = "i32.rotl",
	[I32Rotr] = "i32.rotr",
	[I64Clz] = "i64.clz",
	[I64Ctz] = "i64.ctz",
	[I64PopCnt] = "i64.popcnt",
	[I64Add] = "i64.add",
	[I64Sub] = "i64.sub",
	[I64Mul] = "i64.mul",
	[I64DivS] = "i64.div_s",
	[I64DivU] = "i64.div_u",
	[I64RemS] = "i64.rem_s",
	[I64RemU] = "i64.rem_u",
	[I64And] = "i64.and",
	[I64Or] = "i64.or",
	[I64Xor] = "i64.xor",
	[I64Shl] = "i64.shl",
	[I64ShrS] = "i64.shr_s",
	[I64ShrU] = "i64.shr_u",
	[I64Rotl] = "i64.rotl",
	[I64Rotr] = "i64.rotr",
	[F32Abs] = "f32.abs",
	[F32Neg] = "f32.neg",
	[F32Ceil] = "f32.ceil",
	[F32Floor] = "f32.floor",
	[F32Trunc] = "f32.trunc",
	[F32Nearest] = "f32.nearest",
	[F32Sqrt] = "f32.sqrt",
	[F32Add] = "f32.add",
	[F32Sub] = "f32.sub",
	[F32Mul] = "f32.mul",
	[F32Div] = "f32.div",
	[F32Min] = "f32.min",
	[F32Max] = "f32.max",
	[F32CopySign] = "f32.copysign",
	[F64Abs] = "f64.abs",
	[F64Neg] = "f64.neg",
	[F64Ceil] = "f64.ceil",
	[F64Floor] = "f64.floor",
	[F64Trunc] = "f64.trunc",
	[F64Nearest] = "f64.nearest",
	[F64Sqrt] = "f64.sqrt",
	[F64Add] = "f64.add",
	[F64Sub] = "f64.sub",
	[F64Mul] = "f64.mul",
	[F64Div] = "f64.div",
	[F64Min] = "f64.min",
	[F64Max] = "f64.max",
	[F64CopySign] = "f64.copysign",
	[I32WrapI64] = "i32.wrap_i64",
	[I32TruncF32S] = "i32.trunc_f32_s",
	[I32TruncF32U] = "i32.trunc_f32_u",
	[I32TruncF64S] = "i32.trunc_f64_s",
	[I32TruncF64U] = "i32.trunc_f64_u",
	[I64ExtendI32S] = "i64.extend_i32_s",
	[I64ExtendI32U] = "i64.extend_i32_u",
	[I64TruncF32S] = "i64.trunc_f32_s",
	[I64TruncF32U] = "i64.trunc_f32_u",
	[I64TruncF64S] = "i64.trunc_f64_s",
	[I64TruncF64U] = "i64.trunc_f64_u",
	[F32ConvertI32S] = "f32.convert_i32_s",
	[F32ConvertI32U] = "f32.convert_i32_u",
	[F32ConvertI64S] = "f32.convert_i64_s",
	[F32ConvertI64U] = "f32.convert_i64_u",
	[F32DemoteF64] = "f32.demote_f64",
	[F64ConvertI32S] = "f64.convert_i32_s",
	[F64ConvertI32U] = "f64.convert_i32_u",
	[F64ConvertI64S] = "f64.convert_i64_s",
	[F64ConvertI64U] = "f64.convert_i64_u",
	[F64PromoteF32] = "f64.promote_f32",
	[I32ReinterpretF32] = "i32.reinterpret_f32",
	[I64ReinterpretF64] = "i64.reinterpret_f64",
	[F32ReinterpretI32] = "f32.reinterpret_i32",
	[F64ReinterpretI64] = "f64.reinterpret_i64",
	[I32Extend8S] = "i32.extend8_s",
	[I32Extend16S] = "i32.extend16_s",
	[I64Extend8S] = "i64.extend8_s",
	[I64Extend16S] = "i64.extend16_s",
	[I64Extend32S] = "i64.extend32_s",
	[TruncSat] = "trunc_sat"
};

} // namespace bytecode
} // namespace parser
} // namespace wasmrt