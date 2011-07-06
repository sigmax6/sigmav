/**
 * Copyright (C) 2000 STC.sigma . All right reserved.
 * @brief
 * @author sigmax6
 * @version
 * @date 2011-05-01
 */
#include <stdio.h>
#include <stdlib.h>
#include "lena512.h"
#define WSIZE 3

#define MINMAX 0
#define MIDMID 1
#define MAXMIN 2

typedef unsigned char uchar;

void trisort (uchar * triwindow ,uchar * min,uchar * mid,uchar * max)
{

    if(triwindow[0]>triwindow[1])
    {
        *max=triwindow[0];
        *min=triwindow[1];
    }
    else
    {
        *max=triwindow[1];
        *min=triwindow[0];
    }

    if(triwindow[2]>*max)
    {
        *mid = *max;
        *max = triwindow[2];
    }
    else if (triwindow[2]<*min)
    {
        *mid = *min;
        *min = triwindow[2];
    }
    else
    {
        *mid = triwindow[2];
    }

}

uchar trimax(uchar * triwindow)
{
    uchar max=triwindow[0];
    int counter;
    for(counter = 0;counter < 3;counter ++)
    {
        if(max<triwindow[counter])
        {
            max=triwindow[counter];
        }
    }
    return max;
}

uchar trimin(uchar * triwindow)
{
    uchar min=triwindow[0];
    int counter;
    for(counter = 0;counter < 3;counter ++)
    {
        if(min>triwindow[counter])
        {
            min=triwindow[counter];
        }
    }
    return min;
}

uchar trimid(uchar * triwindow)
{
    uchar mid=triwindow[0];
    uchar max,min;
    if(triwindow[1]>triwindow[2])
    {
        max=triwindow[1];
        min=triwindow[2];
    }
    else
    {
        max=triwindow[2];
        min=triwindow[1];
    }

    if(max<mid)
    {
        return max;
    }
    else if(min>mid)
    {
        return min;
    }
    else
    {
        return mid;
    }
}

uchar owsort(int i,int j)
{
    uchar owMax[WSIZE]={0};
    uchar owMid[WSIZE]={0};
    uchar owMin[WSIZE]={0};
    uchar owRes[WSIZE]={0};//maxMin,midMid,minMax;
    int counter = 0;
    for(; counter < WSIZE ; counter++)
    {
        trisort(&lenaPic[i-(WSIZE/2)+counter][j-(WSIZE/2)],&owMin[counter],&owMid[counter],&owMax[counter]);
    }
    owRes[MAXMIN]=trimax(owMin);
    owRes[MIDMID]=trimid(owMid);
    owRes[MINMAX]=trimin(owMax);
    return trimid(owRes);
}

int main (int argc, char const* argv[])
{
    int i,j,p;
    printf("Input x,y pos:");
    scanf("%d,%d",&i,&j);
    for (p=0;p<(WSIZE*WSIZE);p++)
    {
        if (p%3 == 0)
        {
            puts("\n");
        }
        printf ("%3x",lenaPic[i-((WSIZE)/2)+(p/(WSIZE))][j-((WSIZE)/2)+(p%(WSIZE))]);
    }
    uchar res = owsort(i,j);
    printf ("\n The middle num is %3x ",res);

    return 0;
}

