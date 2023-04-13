add_executable(llvm1)


target_sources(llvm1 PRIVATE
    src/main.cc
)
target_include_directories(llvm1 PRIVATE
    src/
)