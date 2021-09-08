#pragma once

#include "ADT/CodeBuffer.h"
#include "Runtime/Function.h"

using namespace wasmrt;
using namespace wasmrt::adt;

namespace wasmrt {
namespace interpreter {
namespace template_interpreter {

virtual class TemplateInterpreter {
public:
    virtual code_buffer::CodeBlob CodeGen(runtime::Function &Func, code_buffer::CodeBuffer &CB) = 0;
};

} // namespace template_interpreter
} // namespace interpreter
} // namespace wasmrt