#pragma once

using namespace wasmrt;

namespace wasmrt {
namespace adt {
namespace code_buffer {

class CodeBuffer;
struct CodeBlob {
    inline void Free() { CB->Free(Address); }

    size_t Size;
    CodeBuffer *CB;
    uintptr_t  *Address;
} 

class CodeBuffer {
public:
    CodeBlob Allocate(size_t Size);
    void Free(CodeBlob Blob);
    CodeBlob Expand(CodeBlob Blob);
}

} // namespace code_buffer
} // namespace adt
} // namespace wasmrt