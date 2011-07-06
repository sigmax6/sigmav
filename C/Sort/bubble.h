/* bubble.h Ã°ÅİÅÅĞòËã·¨ */
#include "swap.h"
//Ã°ÅİÅÅĞòËã·¨
int bubbleSort(int number,int num[])
{
    int dstPointer,Counter;
    for(dstPointer=0;dstPointer<number;dstPointer++)
    {
        for(Counter=number-1;Counter>dstPointer;Counter--)
        {
            if(num[Counter]<num[Counter-1])
            {
                swap(&num[Counter],&num[Counter-1]);
            }
        }
    }
    return 0;
}
