#include <iostream>

struct A
{
    static const int b = 42;
    static const A *a;
    int c[b];
    int c[a -> b];
};

int main(){ }