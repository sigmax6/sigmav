#include<stdio.h>

static int i=10;
int j;
int *h;
void func1()
{
    static int j=20;
    int * h=&j;
}

void main()
{
    printf("i:%d\n",i);
  //  printf("j:%d\n",j);
    printf("*h:%d\n",*h);
}
