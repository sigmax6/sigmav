/*
 * Copyright (C) 2000 STC.sigma . All right reserved.
 * @file lena.cpp
 * @brief
 * @author sigmax6
 * @version
 * @date 2011-04-03
 */

#include "EasyBMP.h"
#include <stdlib.h>
#include <fstream>

//#define DEBUG 1

using namespace std;

int main (int argc, char const* argv[])
{
  //  int i,j;
    BMP p_bmp;

#ifdef DEBUG
    BMP p_bmp_out;
#endif

    int p_width;
    int p_height;
    int t_pix_int;
    RGBApixel t_pix;

    ofstream p_output_h("lena512.h",ios::out);
    //ofstream p_output_h("lena512.h",ios::out);
    p_bmp.ReadFromFile("lena.bmp");
    p_width = p_bmp.TellWidth();
    p_height = p_bmp.TellHeight();

#ifdef DEBUG
    p_bmp_out.SetSize(p_width,p_height);
    //p_bmp_out.SetBitDepth(8);
#endif

    //p_output_h << "static const int lenaPic ["<<p_width << "][" << p_height << "];" << endl;
    p_output_h << "static unsigned char lenaPic [" << p_width <<"][" << p_height<< "] = { " << endl;
    for(int i=0;i<p_height;i++)
    {
        if(i>0)
        {
            p_output_h<<"," << endl;
        }
        p_output_h << "{" ;
        for(int j=0;j< p_width;j++)
        {
            if(j>0)
            {
                p_output_h<< ",";
            }
            t_pix_int = (((int)p_bmp(j,i)->Red*19595 + (int)p_bmp(j,i)->Green*38469 + (int)p_bmp(j,i)->Blue*7472)>>16) ;
            p_output_h << t_pix_int;
//          p_output_h << (((int)p_bmp(j,i)->Red*19595 + (int)p_bmp(j,i)->Green*38469 + (int)p_bmp(j,i)->Blue*7472)>>16) ;

#ifdef DEBUG
            t_pix.Red=t_pix.Blue=t_pix.Green=t_pix_int;
            p_bmp_out.SetPixel(j,i,t_pix);
#endif

        }
        p_output_h << "}";
    }
    p_output_h << "};" << endl;

    //p_output_h.close();

#ifdef DEBUG
    p_bmp_out.WriteToFile("lenaOut.bmp");
#endif

    p_output_h.close();
}

