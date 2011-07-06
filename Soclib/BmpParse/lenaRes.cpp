/*
 * Copyright (C) 2000 STC.sigma . All right reserved.
 * @file lenaRes.cpp
 * @brief
 * @author sigmax6
 * @version 0.1
 * @date 2011-04-05
 *
 */
#include "EasyBMP.h"
#include <fstream>
//#define DEBUG 1
#define  PSIZE  512

using namespace std;

int main (int argc, char const* argv[])
{
    ifstream p_res_in;
    BMP      p_bmp_out;

    int p_size  = PSIZE;
    int t_pix   = 0;
    char buff;
    RGBApixel p_pix ;

    p_res_in.open("lena.out",ios::binary|ios::in);
    p_res_in.seekg(0,ios::beg);

#ifdef DEBUG
    printf("Buff is :%d\n",buff);
    printf("Size is :%d\n",p_size);
#endif

    p_bmp_out.SetSize(p_size,p_size);
    for(int i = 0; i < p_size ; i++)
    {
        for (int j = 0 ; j < p_size ; j++)
        {
             p_res_in.get(buff);
             t_pix=buff;
             p_pix.Red=p_pix.Blue=p_pix.Green=t_pix;
             p_bmp_out.SetPixel(j,i,p_pix);
        }
    }

    p_bmp_out.WriteToFile("lenaResOut.bmp");
    p_res_in.close();
}

