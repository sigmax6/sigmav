/*
 * Copyright (C) 2000 STC.sigma . All right reserved.
 */
#include <stdio.h>
#include "lena512.h"

#define SIZE 512
#define WINDOW 3
#define WINDOWS ((WINDOW)*(WINDOW))

int main (int argc, char const* argv[])
{
    int x,y,i;
    unsigned char window[WINDOWS];
    printf("Please input the center pos :x,y \n");
    scanf("%d,%d",&x,&y);

    for (i=0;i<WINDOWS;i++)
    {
          window[i]=(unsigned char)lenaPic[x-((WINDOW)/2)+(i/(WINDOW))][y-((WINDOW)/2)+(i%(WINDOW))];
    }

    for (i=0;i<WINDOWS;i++)
    {
        if(i%WINDOW == 0)
        {
            puts("\n");
        }
        printf("%3x",window[i]);
    }
    puts("\n");

    return 0;
}


