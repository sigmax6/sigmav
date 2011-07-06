#include<stdio.h>

int main (int argc, char const* argv[])
{
    int a=100;
    int b=200;
    printf("a:%d,b:%d",a,b);
    a=a^b;
    b=b^a;
    a=a^b;
    printf("a:%d,b:%d",a,b);
}
