/*插入排序法函数 2010-03-14*/
#include <malloc.h>
#include <limits.h>
//规则排序
int merge(int begin,int middle,int end,int num[])
{
    int dstCounter=begin;
    int leftLength=middle-begin+1;
    int rightLength=end-middle;
    int * leftArray=(int *)malloc(sizeof(int)*(leftLength+1));
    int * rightArray=(int *)malloc(sizeof(int)*(rightLength+1));
    int leftCounter=leftLength;
    int rightCounter=rightLength;
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
    return 0;
}

//范围分治排序算法，根据输入的起始位置进行该段的排序
int mergeSortRange(int begin,int end,int num[])
{
    //当起始位置与结束位置不一致时，排序（排除单元素问题）
    if(begin<end)
    {
        //获取中点
        int middle=(begin+end)/2;
        //左侧排序
        mergeSortRange(begin,middle,num);
        //右侧排序
        mergeSortRange(middle+1,end,num);
        //排好序的左右进行规则排序
        merge(begin,middle,end,num);
    }
    return 0;
}

//总算法
int mergeSort(int number,int num[])
{
    //转化为范围分治排序算法
    mergeSortRange(0,number-1,num);
    return 0;
}
