#include <stdio.h>

int ap2[2][5];

int * * test()
{
    return ap2;
}

int main()
{
    int (*a)[5];
    int ap[2][5];
    a=ap;

    a=test();
}
