cmake_minimum_required(VERSION 3.17)

project(Kang)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE DEBUG)

find_package(LLVM REQUIRED)
llvm_map_components_to_libnames(llvm_libs support core irreader)

add_definitions(-DFMT_HEADER_ONLY)
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
    ${LLVM_INCLUDE_DIRS}
)
target_link_libraries(kang PRIVATE
    ${llvm_libs}
)