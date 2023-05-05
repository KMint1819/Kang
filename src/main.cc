#include <cstdio>
#include <iostream>
#include "header.hpp"
int main()
{
    printf("Enter:> ");
    kang::eatToken();

    kang::mainLoop();
    return 0;
}