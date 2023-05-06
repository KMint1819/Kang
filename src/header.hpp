#pragma once

#include "parser.hpp"
#include <fmt/format.h>

namespace kang
{

static void initialize()
{
    myContext = std::make_unique<llvm::LLVMContext>();
    myModule = std::make_unique<llvm::Module>("KModule", *myContext);
    myBuilder = std::make_unique<llvm::IRBuilder<>>(*myContext);
}

static void handleDef()
{
    if (auto funcAst = parseFunction())
    {
        if (auto *funcIR = funcAst->emit())
        {
            fmt::print("Reading function definition: ");
            funcIR->print(llvm::outs());
            fmt::print("\n\n");
        }
    }
    else
        eatToken();
}

static void handleExtern()
{
    if (auto protoAst = parseExtern())
    {
        if (auto *protoIR = protoAst->emit())
        {
            fmt::print("Reading extern: ");
            protoIR->print(llvm::outs());
            fmt::print("\n\n");
        }
    }
    else
        eatToken();
}

static void handleExpr()
{
    if (auto fAst = parseAnonymousFunction())
    {
        if (auto *fIR = fAst->emit())
        {
            fmt::print("Reading extern: ");
            fIR->print(llvm::outs());
            fmt::print("\n\n");

            fIR->removeFromParent();
        }
    }
    else
        eatToken();
}

void mainLoop()
{
    while (true)
    {
        printf("Enter:> ");
        switch (kang::curToken)
        {
        case Token::MY_EOF:
            return;
        case ';':
            eatToken();
            break;
        case Token::DEF:
            handleDef();
            break;
        case Token::EXTERN:
            handleExtern();
            break;
        default:
            handleExpr();
            break;
        }
    }
}
} // namespace kang
