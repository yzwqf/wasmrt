#pragma once

#include "Module.h"

#include <string>

namespace wasmrt {
namespace parser {
namespace reader {

struct SimpleBuffer {
    SimpleBuffer(size_t Size);
    ~SimpleBuffer();

    const size_t   Size{0};
    const uint8_t* Buffer{nullptr};
};

Module *ReadFromBuffer(SimpleBuffer &SB);
Module *ReadFromFile(const std::string FileName);

} // namespace reader
} // namespace parser
} // namespace wasmrt