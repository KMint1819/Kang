cmake_minimum_required(VERSION 3.17)

project(Kang)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE DEBUG)

add_executable(kang
    src/ast.cc
    src/ast.hpp
    src/header.hpp
    src/lexer.cc
    src/lexer.hpp
    src/main.cc
    src/parser.hpp
)
target_include_directories(kang PRIVATE
    src/
    3rdparty/
)