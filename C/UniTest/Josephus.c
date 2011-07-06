#include <stdio.h>
#include "Josephus.h"

int main (int argc, char const* argv[])
{
    int n,k,j;
    printf("Input the scope amount of the Josephus problem:");
    scanf("%d",&n);
    printf("Input the number of k:");
    scanf("%d",&k);
    j=Josephus(n,k,n);
    printf("The josephus number is :%d\n",j);
    return 0;
}

int Josephus(int n,int k,int i)
{
    if(i==1)
    {
        return (k-1)%n;
    }
    else
    {
        return (k+Josephus(n-1,k,i-1))%(n);
    }
}

