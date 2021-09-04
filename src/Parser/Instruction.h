#pragma once

namespace wasmrt {
namespace parser {
namespace instruction {

virtual class Instruction {
public:
    const uint8_t opcode;

    inline uint8_t getOpcode() const { return opcode; }
};

} // namespace instruction
} // namespace parser
} // namespace wasmrt