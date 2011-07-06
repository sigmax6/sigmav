#include<stdio.h>
#include "selection.h"
#include "insertion.h"

#define MAX 10

int main(void)
{
    int num[MAX];
    int Counter;
    int SortType;
    printf("please input 10 numbers：\n");

    //数据输入
    for (Counter=0;Counter<MAX;Counter++)
    {
        scanf("%d",&num[Counter]);
    }
    
    printf("please input the type of sort algorithm:\n");
    printf("1\) selectSort\n");
    printf("2\) Sort\n");
    //算法选择输入
    scanf("%d")
    selectSort(MAX,num);

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



