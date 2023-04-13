#include "header.hpp"
#include <array>
#include <algorithm>

namespace kang
{
std::string identifierStr = "EMPTY"; // Filled in if tok_identifier
double numVal = 0.;
char lastChar = ' ';

int getToken()
{
    while (isspace(lastChar))
        lastChar = getchar();

    if (isalpha(lastChar))
    {
        identifierStr = lastChar;
        lastChar = getchar();
        while (isalnum(lastChar))
        {
            identifierStr += lastChar;
            lastChar = getchar();
        }

        if (identifierStr == "def")
            return Token::DEF;
        if (identifierStr == "extern")
            return Token::EXTERN;

        printf("Is identifier. last char = %c, ascii: %d\n", lastChar, lastChar);
        // Token is a variable name
        return Token::IDENTIFIER;
    }

    // Not robust enough. Will try to make 123.456.789 a number
    if (isdigit(lastChar))
    {
        std::string numberStr = "";

        numberStr += lastChar;
        lastChar = getchar();
        while (isdigit(lastChar) || lastChar == '.')
        {
            numberStr += lastChar;
            lastChar = getchar();
        }
        numVal = std::stod(numberStr);

        return Token::NUMBER;
    }

    // Comment the whole line
    if (lastChar == '#')
    {
        lastChar = getchar();
        static const std::array<char, 3> endTokens = { '\n', '\r', EOF };
        while (std::find(endTokens.begin(), endTokens.end(), lastChar) == endTokens.end())
        {
            lastChar = getchar();
        }

        if (lastChar != EOF)
            return getToken();
    }

    if (lastChar == EOF)
        return Token::MY_EOF;

    // Return the ascii of the last char as it is
    int ascii = lastChar;
    lastChar = getchar();
    return ascii;
}
} // namespace kang