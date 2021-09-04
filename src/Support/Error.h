#pragma once

#include <cstdio>
#include <cstdlib>

namespace wasmrt {
namespace support {
namespace error {

template <typename... Types>
void Error(const char *Function, const char *Format, Types... Args) {
    printf("%s: ", Function);
    printf(Format, Args...);
    exit(-1);
}

} // namespace error
} // namespace support
} // namespace wasmrt