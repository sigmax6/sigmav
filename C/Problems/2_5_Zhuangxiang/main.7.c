#include<stdio.h>

    int boxAmount[6];                           //The Amount of Boxes;
    int box11,box22;                              //The Amount of 1*1 and 2*2 Boxes;
    int boxNeeded;                                //The Amount of boxes that needed totally;
    int boxLeft22[4]={0,5,3,1};               //The Amount of 2*2 boxes left when a 3*3 box putted in;
    
void main(void)
{
    int counter;
    while(1)
    {
        for(counter=0;counter<6;counter++)
        {
            scanf("%d",boxAmount[counter]);
        }
        boxNeeded=boxAmount[5]+boxAmount[4]+boxAmount[3]+boxAmount[2]
    }
}
