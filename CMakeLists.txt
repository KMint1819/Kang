cmake_minimum_required(VERSION 3.17)

project(KANG)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(llvm1
    src/main.cc
)
target_include_directories(llvm1 PRIVATE
    src/
)