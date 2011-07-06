#include <stdio.h>
#define MAX 9

int main (int argc, char const* argv[])
{
    int x,y;
    for (x=1;x <= MAX;x++)
    {
        for (y=1;y < x+1;y++)
        {
            printf("%d\t",x*y);
        }
        puts("");
    }
    return 0;
}

