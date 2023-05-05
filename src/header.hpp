#pragma once

#include <parser.hpp>

namespace kang
{

void handleDef()
{
    if (parseFunction())
        printf("Parsed a function\n");
    else
        eatToken();
}

void handleExtern()
{
    if (parseExtern())
        printf("Parsed an extern\n");
    else
        eatToken();
}

void handleExpr()
{
    if (parseAnonymousFunction())
        printf("Parsed an anonymous function\n");
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
        case Token::MY_EOF: return;
        case ';': eatToken(); break;
        case Token::DEF: handleDef(); break;
        case Token::EXTERN: handleExtern(); break;
        default: handleExpr(); break;
        }
    }
}
} // namespace kang
