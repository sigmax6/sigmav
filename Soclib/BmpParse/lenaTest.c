/*
 * Copyright (C) 2000 STC.sigma . All right reserved.
 */
#include <stdio.h>
#include "lena512.h"

int main (int argc, char const* argv[])
{
     int result=0;
     int i,j;
     for(i=0;i<512;i++)
     {
         for(j=0;j<512;j++)
         {
           result += lenaPic[i][j];
         }
     }
     printf("result:%d\n",result);
     return 0;
}

