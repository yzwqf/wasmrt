#pragma once

#include <cstdio>
#include <cstdlib>

namespace wasmrt {
namespace support {
namespace output {

template <typename... Types>
void Error(const char *Function, const char *Format, Types... Args) {
    printf("%s: ", Function);
    printf(Format, Args...);
    exit(-1);
}

template <typename... Types>
void Log(const char *Function, const char *Format, Types... Args) {
    printf("%s: ", Function);
    printf(Format, Args...);
    printf("\n");
}

} // namespace output
} // namespace support
} // namespace wasmrt