#include <malloc.h>
#include <limits.h>
int merge(int begin,int middle,int end,int num[])
{
    int dstCounter=begin;
    int leftCounter=leftLength=middle-begin+1;
    int rightCounter=rightLength=end-middle;
    int * leftArray=(int *)malloc(sizeof(int)*(leftLength+1));
    int * rightArray=(int *)malloc(sizeof(int)*(rightLength+1));
    leftArray[leftCounter--]=INT_MAX;
    rightArray[rightCounter--]=INT_MAX;
    for(;leftCounter>=0;leftCounter--)
    {
        leftArray[leftCounter]=num[begin+leftCounter];
    }
    for(;rightCounter>=0;rightCounter--)
    {
        rightArray[rightCounter]=num[middle+1+rightCounter];
    }
    int i=0,j=0;
    for(;dstCounter<=end;dstCounter++)
    {
        if(leftArray[i]<rightArray[j])
        {
            num[dstCounter]=leftArray[i++];
        }
        else
        {
            num[dstCounter]=rightArray[j++];
        }
    }
}
int mergeSortRange(int begin,int end,int num[])
{
    if(begin<end)
    {
        int middle=(begin+end)/2;
        mergeSortRange(begin,middle,num);
        mergeSortRange(middle+1,end,num);
        merge(begin,middle,end,num);
    }
    return 0;
}
int mergeSort(int number,int num[])
{
    
}
