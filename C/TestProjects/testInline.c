#include<stdio.h>
inline swap(int * x,int * y);
int main (int argc, char const* argv[])
{
    int i=10;
    int j=23;
    printf("i:%d,j:%d\n",i,j);
    swap(&i,&j);
    printf("i:%d,j:%d\n",i,j);
}
