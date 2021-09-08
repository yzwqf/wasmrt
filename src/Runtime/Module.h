#pragma once

using namespace wasmrt;

namespace wasmrt {
namespace runtime {

class Module {
public:
    Module(parser::module::Module &M);
};

} // namespace runtime
} // namespace wasmrt