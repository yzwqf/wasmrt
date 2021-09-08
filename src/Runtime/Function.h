#pragma once

#include "Parser/Module.h"

#include "Module.h"

using namespace wasmrt;

namespace wasmrt {
namespace runtime {

class Function {
public:
    Function(Module &M, parser::module::Code &Code);
};

} // namespace runtime
} // namespace wasmrt