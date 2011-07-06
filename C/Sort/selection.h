/* 选择排序法函数 2010-03-14*/
/*  修订 更改swap 2010-07-14*/
#include "swap.h"
int selectSort(int number,int num[])
{
    int minNum,counter,comPointer;
    for (counter=0;counter<number-1;counter++)
    {
        minNum=counter;
        for (comPointer=counter+1;comPointer<number;comPointer++)
        {
            if(num[comPointer]<num[minNum])
            {
                minNum=comPointer;
            }
        }
        if (minNum!=counter)
        {
            swap(&num[counter],&num[minNum]);
        }
    }
    return 0;
}
