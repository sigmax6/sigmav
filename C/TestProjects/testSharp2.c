#include<stdio.h>
#define str(x,y) x##y
int main (int argc, char const* argv[])
{
    int ab=10;
    printf("%d\n",str(a,b));
    return 0;
}
