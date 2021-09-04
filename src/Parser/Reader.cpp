#include "Support/Output.h"
#include "Reader.h"

#include <ifstream>
#include <string>
#include <tuple>

using namespace wasmrt;

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
    return {0, 0};
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
    return {0, 0};    
}

template <typename T>
static T trans(T Val) {
    T Result;
    int Size = sizeof(T);
    uint8_t *Dst = reinterpret_cast<uint8_t*>(&Result);
    uint8_t *Src = reinterpret_cast<uint8_t*>(&Val) + Size - 1;
    for (auto *End = Dst + Size; Dst != End; ++Dst) *Dst = *Src--;
    return Result;
}

template <bool translate = false>
class ModuleParser {
public:
    ModuleParser(const SimpleBuffer &SB) : SB(SB) {}

    inline size_t remaining() const { return SB.Size - Idx; }
    float transF32(float f) { return translate ? f : trans(f); }
    uint32_t transU32(uint32_t u) { return translate ? u : trans(u); }
    double transF64(double d) { return translate ? d : trans(d); }

    uint8_t readByte();
    uint32_t readU32();
    float readF32();
    double readF64();

    Module *parse();

    size_t              Idx{0};
    const SimpleBuffer& SB;
};

uint8_t ModuleParser::readByte() {
    if (remaining() < 1) {
        support::output::Error("ModuleParser::readByte", "Remaining len(Bytes) < 1!");
        return -1;
    }
    return *SB.Buffer[Idx++];
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

Module *ModuleParser::parse() {
    if (remaining() < 4) {
        support::output::Note("ModuleParser::parse", "Unexpected end of magic header!");
        return nullptr;
    }

}

Module *ReadFromBuffer(SimpleBuffer &SB) {
    constexpr uint16_t mf = 0x1234;
    constexpr bool translate = *reinterpret_cast<uint8_t*>(&mf) == 0x12;
    return ModuleParser<translate>(SB).parse();
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