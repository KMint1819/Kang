#include <cstdio>
#include <iostream>
#include "header.hpp"

int main()
{
    int token = kang::getToken();
    while (token != kang::Token::MY_EOF)
    {
        printf("%d ", token);
        token = kang::getToken();
    }
    printf("\n");
    return 0;
}