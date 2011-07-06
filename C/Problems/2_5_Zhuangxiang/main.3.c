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
            scanf("%d",boxAmount[counter]);                  //Get the input
        }
        //Calc the boxNeeded by 6*6 + 5*5 +4*4 +[3*3/4] (round up)
        boxNeeded=boxAmount[5]+boxAmount[4]+boxAmount[3]+(boxAmount[2]+3)/4;
        box22=5*boxAmount[3]+boxLeft22[boxAmount[2]%4];
        if(boxAmount[1]>box22)
        {
            boxNeeded+=(boxAmount[1]-box22+8)/9;    //Rounded Up;
        }
        box11=36*boxNeeded-36*boxAmount[5]-
    }
}
