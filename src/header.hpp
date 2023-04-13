#pragma once
#include <string>

namespace kang
{
extern std::string identifierStr; // Filled in if tok_identifier
extern double numVal;
extern char lastChar;

enum Token
{
    MY_EOF = -1,

    DEF = -2,
    EXTERN = -3,

    IDENTIFIER = -4,
    NUMBER = -5,
};

int getToken();

} // namespace kang