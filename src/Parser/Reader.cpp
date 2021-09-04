#include "Support/Output.h"
#include "Bytecode.h"
#include "Reader.h"
#include "Type.h"

#include <cstdlib>
#include <bit>
#include <ifstream>
#include <string>
#include <type_traits>
#include <tuple>
#include <utility>

using namespace wasmrt;
using namespace wasmrt::parser;

namespace wasmrt {
namespace parser {
namespace reader {

// https://en.wikipedia.org/wiki/LEB128#Decode_unsigned_integer
static std::tuple<uint64_t, size_t>
decodeVarUint(const SimpleBuffer& SB, size_t Start, size_t Bits) {
    uint64_t Result{0};
    for (int i = Start, e = SB.Size; i < e; ++i) {
        uint8_t Part = SB.Buffer[i];
        if (i == Bits / 7) {
            if (Part & 0x80 != 0)
                support::output::Error("decodeVarUint", "Integer is too long!");
            if ((Part >> (Bits - i * 7)) > 0)
                support::output::Error("decodeVarUint", "Integer is too large!");
        }
        Result |= ((uint64_t)Part & 0x7f) << (i * 7);
        if (Part & 0x80 == 0)
            return {Result, i + 1};
    }
    support::output::Error("decodeVarUint", "Unexpected path!");
}

// https://en.wikipedia.org/wiki/LEB128#Decode_signed_integer
static std::tuple<int64_t, size_t>
decodeVarInt(const SimpleBuffer& SB, size_t Start, size_t Bits) {
    int64_t Result{0};
    for (int i = Start, e = SB.Size; i < e; ++i) {
        uint8_t Part = SB.Buffer[i];
        if (i == Bits / 7) {
            if (Part & 0x80 != 0)
                support::output::Error("decodeVarInt", "Integer is too long!");
            if ((Part & 0x40 == 0) && (Part >> (Bits - i * 7 - 1) != 0) ||
				(Part & 0x40 != 0) && (((int8_t)Part | 0x80) >> (Bits - i * 7 - 1) != -1))
                support::output::Error("decodeVarUint", "Integer is too large!");
        }
        Result |= ((int64_t)Part & 0x7f) << (i * 7);
        if (Part & 0x80 == 0) {
            if ((i * 7 < Bits) && (Part & 0x40 != 0))
				Result |= (-1 << ((i + 1) * 7));
            return {Result, i + 1};
        }
    }
    support::output::Error("decodeVarInt", "Unexpected path!");
}

template<class U, size_t... N>
static U bswap_impl(U Val, std::index_sequence<N...>) {
  return (((Val >> (N << 3) & 0xff) << (sizeof(T)-1-N) << 3) | ...);
}

template<typename T,
	 typename U = std::conditonal<sizeof(T) == 4, uint32_t, uint64_t>::value>
static T bswap(T Val) {
  return *reinterpret_cast<T*>(bswap_impl<U>(
	*reinterpret_cast<U*>(Val), std::make_index_sequence<sizeof(U)>{}));
}

template <bool translate = false>
class ModuleParser {
public:
    ModuleParser(const SimpleBuffer &SB) : SB(SB) {}

    inline size_t remaining() const { return SB.Size - Idx; }
    float transF32(float f) { return translate ? f : bswap(f); }
    uint32_t transU32(uint32_t u) { return translate ? u : bswap(u); }
    double transF64(double d) { return translate ? d : bswap(d); }

    uint8_t readZero();
    uint8_t readByte();
    uint8_t *readBytes();
    uint32_t readU32();
    float readF32();
    double readF64();
    uint32_t readVarU32();
    int32_t readVarS32();
    uint64_t readVarU64();
    std::string readName();
    std::tuple<module::Expr, uint8_t> readInstructions();
    module::Expr readExpr();
    std::vector<uint32_t> readIndices();
    void ReadTypeSec(Module &M);
    void ReadImportSec(Module &M);
    void ReadTableSec(Module &M);
    void ReadMemSec(Module &M);
    void ReadGlobalSec(Module &M);
    void ReadExportSec(Module &M);
    void ReadElemSec(Module &M);
    void ReadCodeSec(Module &M);
    void ReadDataSec(Module &M);
    void ReadNonCustomSec(uint8_t SecID, Module &M);
    type::ValType ReadValType();
    std::vector<type::ValType> ReadValTypes();
    type::FuncType ReadFuncType();
    type::TableType readTableType();
    type::RangeType readRangeType();
    type::GlobalType readGlobalType();
    type::BlockType readBlockType();
    void ReadSections(Module &M);

    Module *parse();

    size_t              Idx{0};
    const SimpleBuffer& SB;
};

uint8_t ModuleParser::readZero() {
    auto Byte = readByte();
    if (Byte != 0)
        support::output::Error("ModuleParser::readZero", "zero flag expected, got %d", Byte);
    return 0;
}

uint8_t ModuleParser::readByte() {
    if (remaining() < 1) {
        support::output::Error("ModuleParser::readByte", "Remaining len(Bytes) < 1!");
        return -1;
    }
    return *SB.Buffer[Idx++];
}

uint8_t *ModuleParser::readBytes() {
    auto N = readVarU32();
    if (remaining() < N) {
        support::output::Error("ModuleParser::readBytes",
            "Remaining %d bytes, but want %d bytes!", remaining(), N);
    }

    auto *Bytes = new uint8_t[N];
    if (Bytes == nullptr)
        support::output::Error("ModuleParser::readBytes", "Cannot allocate %d bytes!", N);
    memcpy(Bytes, SB.Buffer + Idx, N);
    Idx += N;
    return Bytes;
}

uint32_t ModuleParser::readU32() {
    if (remaining() < 4) {
        support::output::Error("ModuleParser::readU32", "Remaining len(Bytes) < 4!");
        return -1;
    }
    uint32_t Result = *reinterpret_cast<uint32_t*>(&SB.Buffer[Idx]);
    Idx += 4;
    return trans(Result);
}

float ModuleParser::readF32() {
    if (remaining() < 4) {
        support::output::Error("ModuleParser::readF32", "Remaining len(Bytes) < 4!");
        return -1;
    }
    float Result = *reinterpret_cast<float*>(&SB.Buffer[Idx]);
    Idx += 4;
    return trans(Result);
}

double ModuleParser::readF64() {
    if (remaining() < 8) {
        support::output::Error("ModuleParser::readF64", "Remaining len(Bytes) < 8!");
        return -1;
    }
    double Result = *reinterpret_cast<double*>(&SB.Buffer[Idx]);
    Idx += 8;
    return trans(Result);
}

uint32_t ModuleParser::readVarU32() {
    auto [N, Bytes] = decodeVarUint(SB, Idx, 32);
    Idx += Bytes;
    return (uint32_t)N;
}

int32_t ModuleParser::readVarS32() {
    auto [N, Bytes] = decodeVarUint(SB, Idx, 32);
    Idx += Bytes;
    return (int32_t)N;
}

uint64_t ModuleParser::readVarU64() {
    auto [N, Bytes] = decodeVarUint(SB, Idx, 64);
    Idx += Bytes;
    return N;
}

// https://www.jianshu.com/p/a83d398e3606
static bool IsUtf8(unsigned char* data, int len) {
    static auto PreNum = [](uint8_t byte) {
        int num = 0;
        uint8_t mask = 0x80;
        for (int i = 0; i < 8; i++) {
            if ((byte & mask) == mask) {
                mask >>= 1;
                ++num;
            } else
                break;
        }
        return num;
    };

    int i = 0, num = 0;
    while (i < len) {
        if ((data[i] & 0x80) == 0x00) {
            ++i;
            continue;
        }
        
        if ((num = PreNum(data[i])) > 2) {       
            ++i;
            for(int j = 0; j < num - 1; j++) {
                if ((data[i] & 0xc0) != 0x80)
                    return false;
                ++i;
            }
        } else
            return false;
    } 
    return true;
}

std::string ModuleParser::readName() {
    auto RemainingBeforeRead = remaining();
    auto *Bytes = readBytes();
    if (!IsUtf8(Bytes, remaining() - RemainingBeforeRead))
        support::output::Error("ModuleParser::readName", "malformed UTF-8 encoding");
    return std::string(reinterpret_cast<char *>readBytes());
}

type::ValType ModuleParser::ReadValType() {
    auto Tag = readByte();
    switch (Tag) {
        case type::ValTypeI32:
        case type::ValTypeI64:
        case type::ValTypeF32:
        case type::ValTypeF64:
            break;
        default:
            support::output::Error("ModuleParser::ReadValType", "malformed value type: %d", Tag);
    }
    return Tag;
}

std::vector<type::ValType> ModuleParser::ReadValTypes() {
    int N = readVarU32();
    std::vector<type::ValType> Vec(N);
    for (int i = 0; i < N; ++i)
        Vec[i] = ReadValType();
    return Vec;
}

type::FuncType ModuleParser::ReadFuncType() {
    auto Tag = ReadByte();
    if (Tag != type::FtTag)
        support::output::Error("ModuleParser::ReadFuncType", "invalid functype tag: %d", Tag);
    return FuncType(Tag, ReadValTypes(), ReadValTypes());
}

type::TableType ModuleParser::readTableType() {
    type::TableType TT = {readByte(), readRangeType()};
    if (TT.ElemType != type::FuncRef)
        support::output::Error("ModuleParser::ReadFuncType", "invalid elemtype: %d", TT.ElemType);
    return TT;
}

type::RangeType ModuleParser::readRangeType() {
    type::RangeType Range = {readByte(), readVarU32()};
    if (Range.Tag == 1)
        Range.Max = readVarU32();
    return Range;
}

type::GlobalType ModuleParser::readGlobalType() {
    type::GlobalType GT = {readValType(), readByte()};
    if (GT.Mut != type::MutConst || GT.Mut != type::MutVar)
        support::output::Error("ModuleParser::readGlobalType", "malformed mutability: %d", gt.Mut);
    return GT;
}

type::BlockType ModuleParser::readBlockType() {
    auto BT = readVarS32();
    if (BT < 0) {
        switch (BT) {
            case type::BlockTypeI32: case type::BlockTypeI64:
			case type::BlockTypeF32: case type::BlockTypeF64:
			case type::BlockTypeEmpty:
                return BT;
            default:
                support::output::Error("ModuleParser::readGlobalType", "malformed block type: %d", BT);
        }
    }
    return BT;
}

std::vector<uint32_t> ModuleParser::readIndices(Module &M) {
    std::vector<uint32_t> Indices(readVarU32());
    for (auto &Indice : Indices)
        Indice = readVarU32();
    return std::move(Indices);
}

std::tuple<module::Expr, uint8_t>
ModuleParser::readInstructions() {
    module::Expr Instructions;
    while (true) {
        auto opcode = readByte();
        if (bytecode::OpNames.count(opcode) == 0)
            support::output::Error("ModuleParser::readInstructions", "undefined opcode: %d", opcode);
        bytecode::Instruction *Inst;
        switch (opcode) {
            case Block:
            case Loop:
                Inst = new bytecode::BlockInst(opcode, readBlockType(), std::move(readExpr()));
                break;
            case If: {
                auto BT = readBlockType();
                auto [Insts0, End0] = readInstructions();
                Inst = new bytecode::IfInst(BT, std::move(Insts0));
                if (End0 == bytecode::Else_) {
                    auto [Insts1, End1] = readInstructions();
                    Inst->IfFalse = std::move(Insts1);
                    if (End1 != bytecode::End_)
                        support::output::Error("ModuleParser::readInstructions", "invalid block end: %d", End1);
                }
                break;
            }
            case BrTable:
                Inst = new bytecode::BrTableInst(std::move(readIndices()), readVarU32());
                break;
            case CallIndirect:
                Inst = new bytecode::WithArgInst(opcode, readVarU32());
                readZero();
                break;
            case Br:
            case BrIf:
            case LocalGet:
            case LocalSet:
            case LocalTee:
            case GlobalGet:
            case GlobalSet:
            case Call:
                Inst = new bytecode::WithArgInst(opcode, readVarU32());
                break;
            case I32Const:
                Inst = new bytecode::WithArgInst(opcode, readVarS32());
                break;
            case I64Const:
                Inst = new bytecode::WithArgInst(opcode, readVarS64());
                break;
            case F32Const: {
                auto Val = readF32();
                Inst = new bytecode::WithArgInst(opcode, *reinterpret_cast<uint32_t*>(&Val));
                break;
            }
            case F64Const: {
                auto Val = readF64();
                Inst = new bytecode::WithArgInst(opcode, *reinterpret_cast<uint64_t*>(&Val));
                break;
            }
            case TruncSat:
                Inst = new bytecode::WithArgInst(opcode, readByte());
                break;
            default:
                if (opcode >= bytecode::I32Load && opcode <= bytecode::I64Store32) {
                    Inst = new bytecode::MemoryInst(opcode, readVarU32(), readVarU32());
                } else {
                    if (opcode == bytecode::MemorySize || opcode == bytecode::MemoryGrow)
                        readZero();
                    Inst = new Instruction(opcode);
                }
        }
        Instructions.emplace_back(Inst);
        if (opcode == bytecode::Else_ || opcode == bytecode::End_)
            return {Instructions, opcode};
    }
}

module::Expr ModuleParser::readExpr() {
    auto [Instructions, End] = readInstructions();
    if (End != opcode == bytecode::End_)
        support::output::Error("ModuleParser::readExpr", "invalid expr end: %d", End);
    return Instructions;
}

void ModuleParser::ReadTypeSec(Module &M) {
    int N = ReadVarU32();
    for (int i = 0; i < N; ++i)
        M.TypeSec.emplace_back(std::move(ReadFuncType()));
}

void ModuleParser::ReadImportSec(Module &M) {
    int N = readVarU32();
    for (int i = 0; i < N; ++i) {
        auto Module = readName();
        auto Name = readName();
        ImportDesc Desc = {readByte()};
        switch (Desc.Tag) {
            case module::ImportTagFunc:     Desc.Idx.FuncType = readVarU32(); break;
            case module::ImportTagTable:    Desc.Idx.Table = readTableType(); break;
            case module::ImportTagMem:      Desc.Idx.Mem = readRangeType(); break;
            case module::ImportTagGlobal:   Desc.Idx.Global = readGlobalType(); break;
            default:
                support::output::Error("ModuleParser::ReadImportSec", "invalid import desc tag: %d", Desc.Tag);
        }
        M.ImportSec.emplace_back(Import(Module, Name, Desc));
    }
}

void ModuleParser::ReadTableSec(Module &M) {
    M.TableSec.resize(readVarU32());
    for (auto &TT : M.TableSec)
        TT = readTableType();
}

void ModuleParser::ReadMemSec(Module &M) {
    M.MemSec.resize(readVarU32());
    for (auto &Mem : M.MemSec)
        Mem = readRangeType();
}

void ModuleParser::ReadGlobalSec(Module &M) {
    M.GlobalSec.resize(readVarU32());
    for (auto &Global : M.GlobalSec)
        Global = {readGlobalType(), readExpr()};    
}

void ModuleParser::ReadExportSec(Module &M) {
    M.ExportSec.resize(readVarU32());
    for (auto &Export : M.ExportSec) {
        auto Name = readName();
        auto Tag = readByte();
        auto Idx = readVarU32();
        switch Tag {
            case module::ExportTagFunc: // func_idx
            case module::ExportTagTable: // table_idx
            case module::ExportTagMem: // mem_idx
            case module::ExportTagGlobal: // global_idx
                break;
            default:
                support::output::Error("ModuleParser::ReadExportSec", "invalid export desc tag: %d", Tag);
        }
        Export = {std::move(Name), ExportDesc(Tag, Idx)};
    }
}

void ModuleParser::ReadElemSec(Module &M) {
    M.ElemSec.resize(readVarU32());
}

void ModuleParser::ReadCodeSec(Module &M) {

}

void ModuleParser::ReadDataSec(Module &M) {

}

void ModuleParser::ReadNonCustomSec(uint8_t SecID, Module &M) {
    switch (SecID) {
        case SecTypeID:     ReadTypeSec(M); break;
        case SecImportID:   ReadImportSec(M); break;
        case SecFuncID:     M.FuncSec = std::move(readIndices()); break;
        case SecTableID:    ReadTableSec(M); break;
        case SecMemID:      ReadMemSec(M); break;
        case SecGlobalID:   ReadGlobalSec(M); break;
        case SecExportID:   ReadExportSec(M); break;
        case SecStartID:    M.StartSec = readVarU32(); break;
        case SecElemID:     ReadElemSec(M); break;
        case SecCodeID:     ReadCodeSec(M); break;
        case SecDataID:     ReadDataSec(M); break;
        default:
            support::output::Error("ModuleParser::ReadNonCustomSec", "Unexpected Section ID: %d", SecID);
    }
}

void ModuleParser::ReadSections(Module &M) {
    uint8_t PrevSecID = 0;
    while (remaining() > 0) {
        auto SecID = readByte();
        if (SecID == module::SecCustomID) {
            M.CustomSecs.emplace_back(std::move(CustomSec(readName(), readBytes())));
            continue;
        }

        if (SecID > module::SecDataID)
            support::output::Error("ModuleParser::ReadSections", "malformed section id: %d!", SecID);
        if (SecID <= PrevSecID)
            support::output::Error("ModuleParser::ReadSections", "junk after last section, id: %d!", SecID);
        
        PrevSecID = SecID;
        auto N = readVarU32();
        auto RemainingBeforeRead = remaining();
        ReadNonCustomSec(SecID, M);
        if (RemainingBeforeRead - N != remaining())
            support::output::Error("ModuleParser::ReadSections", "section size mismatch, id: %d", SecID);
    }
}

Module *ModuleParser::parse() {
    if (remaining() < 4)
        support::output::Log"ModuleParser::parse", "Unexpected end of magic header!");

    Module *M = new Module();
    if (M == nullptr)
        support::output::Log"ModuleParser::parse", "Cannot allocate Module Object!");

    if ((M->Magic = readU32()) != module::MagicNumber)
        support::output::Log"ModuleParser::parse", "Unsupported Magic!");

    if (remaning() < 4)
        support::output::Log"ModuleParser::parse", "unexpected end of binary version!");
    if ((M->Version = readU32()) != module::SupportVersion)
        support::output::Log"ModuleParser::parse", "Unsupported Version!");
    ReadSections(*M);
    if (M->FuncSec.size() != M->CodeSec.size())
        support::output::Log"ModuleParser::parse", "function and code section have inconsistent lengths!");
    if (remaining() != 0)
        support::output::Log"ModuleParser::parse", "junk after last section!");
    return M;
}

Module *ReadFromBuffer(SimpleBuffer &SB) {
    constexpr bool translate = std::endian::native == std::endian::big;
    return ModuleParser<translate>(SB).parse()
}

Module *ReadFromFile(const std::string FileName) {
    std::ifstream is(FileName, std::ios::binary);
    if (!is.is_open()) {
        support::output::Error("ReadFromFile", "Cannot Open File: %s!\n", FileName.c_str());
        return nullptr; // unreachable.
    }

    is.seekg(0, std::ios::end);
    int Length = is.tellg();

    is.seekg(0, std::ios::begin);
    SimpleBuffer SB(Length + 1);
    is.read(SB.Buffer, Length);
    auto *Module = ReadFromBuffer(SB);
    if (Module == nullptr) {
        support::output::Error("ReadFromFile", "Cannot decode module from file: %s!\n", FileName.c_str());
        return nullptr; // unreachable.
    }
    return Module;
}

SimpleBuffer::SimpleBuffer(size_t Size)
    : Size(Size), Buffer(new uint8_t[Size]) {
    if (Buffer == nullptr)
        support::output::Error("SimpleBuffer::SimpleBuffer", "Cannot allocate memory, size = %d!\n", Size);
}
    
SimpleBuffer::~SimpleBuffer() {
    if (Buffer != nullptr)
        delete[] Buffer;
}

} // namespace reader
} // namespace parser
} // namespace wasmrt