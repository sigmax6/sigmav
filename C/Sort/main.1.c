#include<stdio.h>
#include "selection.h"
#include "insertion.h"
#include "merge.h"
#include "../Search/binarySearchIterate.h"

//最大排序容量
#define MAX 10
#define TYPE 3

//定义类型库
char * sortTypes[TYPE]=
{
    "selection",
    "insertion",
    "merge"
};

int main(void)
{
    int num[MAX];
    int Counter;
    int SortType;
    int TypeCounter;
    printf("please input %d numbers:\n",MAX);

    //数据输入
    for (Counter=0;Counter<MAX;Counter++)
    {
        scanf("%d",&num[Counter]);
    }

    //算法选择提示信息
    printf("please input the type of sort algorithm:\n");
    for(TypeCounter=0;TypeCounter<TYPE;TypeCounter++)
    {
        printf("%d) %s\n",TypeCounter+1,sortTypes[TypeCounter]);
    }


    //算法选择输入
    scanf("%d",&SortType);

    switch(SortType)
    {
        case 1:
            selectSort(MAX,num);
            break;
        case 2:
            insertSort(MAX,num);
            break;
        case 3:
            mergeSort(MAX,num);
            break;
        default:
            printf("Bad order!\n");
            break;
    }


    //数据输出
    printf("numbers after %s sort :\n",sortTypes[SortType-1]);
    for (Counter=0;Counter<MAX;Counter++)
    {
        printf("%d ",num[Counter]);
    }
    printf("\n");

    //测试二分查找算法
    int searchNum;
    int searchR
    while(1)
    {
        printf("输入要查找的数：\n");
        scanf("%d",&searchNum);
        
    }
    //----------------------
    getchar();
    return 0;
}
