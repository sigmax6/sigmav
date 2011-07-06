/* 选择排序法函数 2010-03-14*/
/*  修订 更改swap 2010-07-14*/
#include<stdio.h>
int selectSort(int number,int num[])
{
    int minNum,counter,comPointer;
    for (counter=0;counter<number;counter++)
    {
        comPointer=counter+1;
        minNum=counter;
        for (;comPointer<number;comPointer++)
        {
            if(num[comPointer]<num[minNum])
            {
                minNum=comPointer;
            }
        }
        if (minNum!=counter)
        {
            num[counter]=num[counter]^num[minNum];
            num[minNum]=num[counter]^num[minNum];
            num[counter]=num[counter]^num[minNum];
        }
    }
    return 0;
}
