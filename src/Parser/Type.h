#pragma once

#include "Support/Error.h"

#include <functional>
#include <string>
#include <vector>

using namespace wasmrt;

namespace wasmrt {
namespace parser {
namespace type {

using ValType   = uint8_t;  
using BlockType = int32_t;

inline constexpr ValueType ValTypeI32 = 0x7F; // i32
inline constexpr ValueType ValTypeI64 = 0x7E; // i64
inline constexpr ValueType ValTypeF32 = 0x7D; // f32
inline constexpr ValueType ValTypeF64 = 0x7C; // f64

inline constexpr BlockType BlockTypeI32 = -1;  // ()->(i32)
inline constexpr BlockType BlockTypeI64 = -2;  // ()->(i64)
inline constexpr BlockType BlockTypeF32 = -3;  // ()->(f32)
inline constexpr BlockType BlockTypeF64 = -4;  // ()->(f64)
inline constexpr BlockType BlockTypeEmpty = -64; // ()->()

inline constexpr uint8_t FtTag   = 0x60;
inline constexpr uint8_t FuncRef = 0x70;

inline constexpr uint8_t MutConst = 0;
inline constexpr uint8_t MutVar   = 1;

static const char *ValTypeToStr(ValType Type) {
	switch Type {
        case ValTypeI32: return "i32";
        case ValTypeI64: return "i64";
        case ValTypeF32: return "f32";
        case ValTypeF64: return "f64";
        default:
            support::error::Error("ValTypeToStr", "Invalid ValType: %s!\n", Type);
	}
    return nullptr; // unreachable.
}

struct FuncType {
	uint8_t               Tag;
	std::vector<ValType>  ParamTypes;
	std::vector<ValType>  ResultTypes;

    bool equals(const FuncType &other) const {
        if (ParamTypes.size() != other.ParamTypes.size())
            return false

        if (ResultTypes.size() != other.ResultTypes.size())
            return false

        for (int i = 0, Size = ParamTypes.size(); i < Size; ++i) {
            if (ParamTypes[i] != other.ParamTypes[i])
                return false;
        }


        for (int i = 0, Size = ResultTypes.size(); i < Size; ++i) {
            if (ResultTypes[i] != other.ResultTypes[i])
                return false;
        }
        return true
    }

    std::string getSignature() const {
        static auto append = [] (std::string &Signature, const std::vector<ValType> &Types) {
            for (int i = 0, Size = Types.size(); i < Size; ++i) {
                if (i > 0)
                    Signature.append(',');
                Signature.append(ValTypeToStr(ParamTypes[i]));
            }
        };
        std::string Signature("(");
        append(Signature, ParamTypes);
        Signature.append(") -> (");
        append(Signature, ResultTypes);
        return Signature;
    }

    inline std::string str() const { return getSignature(); }
};

struct RangeType {
	uint8_t  Tag;
	uint32_t Min;
	uint32_t Max;

    std::string str() const {
        std::string str("{min: ");
        str.append(Min).append(", max: ").append(Max).append('}');
        return str;
    }
};

using MemType = RangeType;

struct TableType {
	uint8_t   ElemType;
	RangeType Limits;
};

struct GlobalType {
	bool     Mut;
	ValType  Type;

    std::string str() const {
        std::string str("{type: ");
        str.append(ValTypeToStr(Type)).append(", mut: ");
            .append(Mut ? "true" : "false").append('}');
        return str;
    }
}

} // namespace type
} // namespace parser
} // namespace wasmrt