#include "Interpreter/TemplateInterpreter.h"
#include "Support/Output.h"

#include "Assembler.h"

using namespace wasmrt;
using namespace wasmrt::adt;
using namespace wasmrt::support;

namespace wasmrt {
namespace target {
namespace x86_64 {

class X86_64TemplateInterpreter : public TemplateInterpreter {
public:
    X86_64TemplateInterpreter(runtime::Function &Func, code_buffer::CodeBuffer &CB)
        : ASM(CB, ExpectInterpretSIze(Func)) {}

    size_t ExpectInterpretSIze(runtime::Function &Func);
    void RuntimeCall();
    code_buffer::CodeBlob CodeGen() final;

    Assembler ASM;
}

code_buffer::CodeBlob
X86_64TemplateInterpreter::CodeGen() {
    int InstIdx = 0;
    for (auto &Inst : Func) {
        switch (Inst.getOpcode()) {
            case Unreachable : RuntimeCall(); break;// unreachable
            case Nop         : break;// nop
            case Block       : break;// block rt in* end
            case Loop        : break;// loop rt in* end
            case If          : break;// if rt in* else in* end
            case Else_       : break;// else
            case End_        : break;// end
            case Br          : break;// br l
            case BrIf        : break;// br_if l
            case BrTable     : break;// br_table l* lN
            case Return      : break;// return
            case Call        : break;// call x
            case CallIndirect: break;// call_indirect x
            case Drop        : break;// drop
            case Select      : break;// select
            case LocalGet    : break;// local.get x
            case LocalSet    : break;// local.set x
            case LocalTee    : break;// local.tee x
            case GlobalGet   : break;// global.get x
            case GlobalSet   : break;// global.set x
            case I32Load     : break;// i32.load m
            case I64Load     : break;// i64.load m
            case F32Load     : break;// f32.load m
            case F64Load     : break;// f64.load m
            case I32Load8S   : break;// i32.load8_s m
            case I32Load8U   : break; // i32.load8_u m
            case I32Load16S  : break; // i32.load16_s m
            case I32Load16U  : break; // i32.load16_u m
            case I64Load8S   : break; // i64.load8_s m
            case I64Load8U   : break; // i64.load8_u m
            case I64Load16S  : break; // i64.load16_s m
            case I64Load16U  : break; // i64.load16_u m
            case I64Load32S  : break; // i64.load32_s m
            case I64Load32U  : break; // i64.load32_u m
            case I32Store    : break; // i32.store m
            case I64Store    : break; // i64.store m
            case F32Store    : break;// f32.store m
            case F64Store    : break;// f64.store m
            case I32Store8   : break;// i32.store8 m
            case I32Store16  : break;// i32.store16 m
            case I64Store8   : break;// i64.store8 m
            case I64Store16  : break;// i64.store16 m
            case I64Store32  : break;// i64.store32 m
            case MemorySize  : break;// memory.size
            case MemoryGrow  : break;// memory.grow
            case I32Const    : break;// i32.const n
            case I64Const    : break;// i64.const n
            case F32Const    : break;// f32.const z
            case F64Const    : break;// f64.const z
            case I32Eqz      : break;// i32.eqz
            case I32Eq       : break;// i32.eq
            case I32Ne       : break;// i32.ne
            case I32LtS      : break;// i32.lt_s
            case I32LtU      : break;// i32.lt_u
            case I32GtS      : break; // i32.gt_s
            case I32GtU      : break; // i32.gt_u
            case I32LeS      : break; // i32.le_s
            case I32LeU      : break; // i32.le_u
            case I32GeS      : break; // i32.ge_s
            case I32GeU      : break;// i32.ge_u
            case I64Eqz      : break;// i64.eqz
            case I64Eq       : break;// i64.eq
            case I64Ne       : break;// i64.ne
            case I64LtS      : break;// i64.lt_s
            case I64LtU      : break;// i64.lt_u
            case I64GtS      : break;// i64.gt_s
            case I64GtU      : break;// i64.gt_u
            case I64LeS      : break;// i64.le_s
            case I64LeU      : break;// i64.le_u
            case I64GeS      : break;// i64.ge_s
            case I64GeU      : break;// i64.ge_u
            case F32Eq       : break;// f32.eq
            case F32Ne       : break;// f32.ne
            case F32Lt       : break;// f32.lt
            case F32Gt       : break;// f32.gt
            case F32Le       : break;// f32.le
            case F32Ge       : break;// f32.ge
            case F64Eq       : break;// f64.eq
            case F64Ne       : break;// f64.ne
            case F64Lt       : break;// f64.lt
            case F64Gt       : break;// f64.gt
            case F64Le       : break;// f64.le
            case F64Ge       : break;// f64.ge
            case I32Clz      : break;// i32.clz
            case I32Ctz      : break;// i32.ctz
            case I32PopCnt   : break;// i32.popcnt
            case I32Add      : break;// i32.add
            case I32Sub      : break;// i32.sub
            case I32Mul      : break;// i32.mul
            case I32DivS     : break;// i32.div_s
            case I32DivU     : break;// i32.div_u
            case I32RemS     : break;// i32.rem_s
            case I32RemU     : break;// i32.rem_u
            case I32And      : break;// i32.and
            case I32Or       : break;// i32.or
            case I32Xor      : break;// i32.xor
            case I32Shl      : break;// i32.shl
            case I32ShrS     : break;// i32.shr_s
            case I32ShrU     : break;// i32.shr_u
            case I32Rotl     : break;// i32.rotl
            case I32Rotr     : break;// i32.rotr
            case I64Clz      : break;// i64.clz
            case I64Ctz      : break;// i64.ctz
            case I64PopCnt   : break;// i64.popcnt
            case I64Add      : break;// i64.add
            case I64Sub      : break;// i64.sub
            case I64Mul      : break;// i64.mul
            case I64DivS     : break;// i64.div_s
            case I64DivU     : break;// i64.div_u
            case I64RemS     : break;// i64.rem_s
            case I64RemU     : break;// i64.rem_u
            case I64And      : break;// i64.and
            case I64Or       : break;// i64.or
            case I64Xor      : break;// i64.xor
            case I64Shl      : break;// i64.shl
            case I64ShrS     : break;// i64.shr_s
            case I64ShrU     : break;// i64.shr_u
            case I64Rotl     : break;// i64.rotl
            case I64Rotr     : break;// i64.rotr
            case F32Abs      : break;// f32.abs
            case F32Neg      : break;// f32.neg
            case F32Ceil     : break;// f32.ceil
            case F32Floor    : break;// f32.floor
            case F32Trunc    : break;// f32.trunc
            case F32Nearest  : break;// f32.nearest
            case F32Sqrt     : break;// f32.sqrt
            case F32Add      : break;// f32.add
            case F32Sub      : break;// f32.sub
            case F32Mul      : break;// f32.mul
            case F32Div      : break;// f32.div
            case F32Min      : break;// f32.min
            case F32Max      : break;// f32.max
            case F32CopySign : break;// f32.copysign
            case F64Abs      : break;// f64.abs
            case F64Neg      : break;// f64.neg
            case F64Ceil     : break;// f64.ceil
            case F64Floor    : break;// f64.floor
            case F64Trunc    : break;// f64.trunc
            case F64Nearest  : break;// f64.nearest
            case F64Sqrt     : break;// f64.sqrt
            case F64Add      : break;// f64.add
            case F64Sub      : break;// f64.sub
            case F64Mul      : break;// f64.mul
            case F64Div      : break;// f64.div
            case F64Min        : break;// f64.min
            case F64Max        : break;// f64.max
            case F64CopySign   : break;// f64.copysign
            case I32WrapI64    : break;// i32.wrap_i64
            case I32TruncF32S  : break;// i32.trunc_f32_s
            case I32TruncF32U  : break;// i32.trunc_f32_u
            case I32TruncF64S  : break;// i32.trunc_f64_s
            case I32TruncF64U  : break;// i32.trunc_f64_u
            case I64ExtendI32S : break;// i64.extend_i32_s
            case I64ExtendI32U : break;// i64.extend_i32_u
            case I64TruncF32S  : break;// i64.trunc_f32_s
            case I64TruncF32U  : break;// i64.trunc_f32_u
            case I64TruncF64S  : break;// i64.trunc_f64_s
            case I64TruncF64U  : break;// i64.trunc_f64_u
            case F32ConvertI32S: break;// f32.convert_i32_s
            case F32ConvertI32U: break;// f32.convert_i32_u
            case F32ConvertI64S: break;// f32.convert_i64_s
            case F32ConvertI64U: break;// f32.convert_i64_u
            case F32DemoteF64     : break;// f32.demote_f64
            case F64ConvertI32S   : break;// f64.convert_i32_s
            case F64ConvertI32U   : break;// f64.convert_i32_u
            case F64ConvertI64S   : break;// f64.convert_i64_s
            case F64ConvertI64U   : break;// f64.convert_i64_u
            case F64PromoteF32    : break;// f64.promote_f32
            case I32ReinterpretF32: break;// i32.reinterpret_f32
            case I64ReinterpretF64: break;// i64.reinterpret_f64
            case F32ReinterpretI32: break;// f32.reinterpret_i32
            case F64ReinterpretI64: break;// f64.reinterpret_i64
            case I32Extend8S      : break;// i32.extend8_s
            case I32Extend16S     : break;// i32.extend16_s
            case I64Extend8S      : break;// i64.extend8_s
            case I64Extend16S     : break;// i64.extend16_s
            case I64Extend32S     : break;// i64.extend32_s
            case TruncSat         : break;// <i32|64>.trunc_sat_<f32|64>_<s|u>
            default:
                output::Error("X86_64TemplateInterpreter::CodeGen", "Bad Opcode: %d\n", Inst.getopcode());
        }
        ++InstIdx;
    }
    return ASM.Blob;
}

} // namespace x86_64
} // namespace target
} // namespace wasmrt