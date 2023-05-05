# Problems
1. Why do we need llvm IR instead of executing the binops directly?
    - llvm might be doing some optimizations behind the scene. For example, removing unnecessary operations etc.
2. What does name in argument list of llvm::IRBuilder::CreateFAdd() mean?
    > Local value names for instructions are purely optional, but it makes it much easier to read the IR dumps. [source](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl03.html#expression-code-generation)