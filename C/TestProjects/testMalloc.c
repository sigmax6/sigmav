#include<stdio.h>
#include<stdlib.h>

int main (int argc, char const* argv[])
{
    int size;
    int * arr;
    printf("Input a size:\n");
    scanf("%d",&size);
    if((arr=(int *)malloc(size*sizeof(int)))==NULL)
    {
        printf("Cannot allocate memory!\n");
        return -1;
    }
    else
    {
        int i=0;
        for(i=0;i<size;i++)
        {
            arr[i]=i;
        }
        for(i=0;i<size;i++)
        {
            printf("%4d",arr[i]);
        }
        printf("\n");
        return 0;
    }
    free(arr);
}
