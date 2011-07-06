#include<stdio.h>
int main (int argc, char const* argv[])
{
    char a='a';
    char A='A';

    printf("%c,%d",a,a);
    printf("%c,%d",A,A);
    printf("%d",a-A);
    return 0;
}
