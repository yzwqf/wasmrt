cmake_minimum_required(VERSION 3.14)

project(wasmrt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-O3)

include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

enable_testing()

add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(test)
