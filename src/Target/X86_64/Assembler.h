#pragma once

#include "ADT/CodeBuffer.h"

using namespace wasmrt;
using namespace wasmrt::adt;

namespace wasmrt {
namespace target {
namespace x86_64 {

class Assembler {
public:
    Assembler(code_buffer::CodeBuffer &CB, size_t InitSize)
        : Blob(CB.Allocate(InitSize)) {}

    inline code_buffer::CodeBlob Expand() {
        return CB.Expand(Blob)
    }

    size_t Idx{0};
    code_buffer::CodeBuffer &CB;
    code_buffer::CodeBlob Blob;
};

} // namespace x86_64
} // namespace target
} // namespace wasmrt