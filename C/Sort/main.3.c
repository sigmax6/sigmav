#include<stdio.h>
#include "selection.h"
#include "insertion.h"
#include "merge.h"
//最大排序容量
#define MAX 10

int main(void)
{
    int num[MAX];
    int Counter;
    int SortType;
    printf("please input %d numbers：\n",MAX);

    //数据输入
    for (Counter=0;Counter<MAX;Counter++)
    {
        scanf("%d",&num[Counter]);
    }

    printf("please input the type of sort algorithm:\n");
    printf("1) selectSort\n");
    printf("2) insertSort\n");
    printf("3) mergeSort\n");
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
    printf("numbers after select sort :\n");
    for (Counter=0;Counter<MAX;Counter++)
    {
        printf("%d ",num[Counter]);
    }
    printf("\n");

    //----------------------
    getchar();
    return 0;
}
