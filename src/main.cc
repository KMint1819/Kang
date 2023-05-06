#include "header.hpp"
#include <cstdio>
#include <iostream>
int main()
{
    kang::initialize();

    printf("Enter:> ");
    kang::eatToken();

    kang::mainLoop();
    return 0;
}