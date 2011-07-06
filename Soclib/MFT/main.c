/*
 *
 * SOCLIB_GPL_HEADER_BEGIN
 *
 * This file is part of SoCLib, GNU GPLv2.
 *
 * SoCLib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * SoCLib is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SoCLib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * SOCLIB_GPL_HEADER_END
 *
 * Copyright (c) UPMC, Lip6, SoCVFED
 *         Nicolas Pouillon <nipo@ssji.net>, 2006-2007
 *         Francois Pecheux <francois.pecheux@lip6.fr>, 2006-2007
 */

//#define NCPU 2     // 1 2 4 8 is allowed here ==> Now set by ncpu.h


#define PSIZE 512
#define WSIZE 3
#define WLENG ((WSIZE)*(WSIZE))

//#define ZEROBUG
//#define DEBUG

#define PALSE while(1) \
                ;
#define CALCL 2    //2 line calc

/* algorithm setting */
/*#define QSORTV2*/
#define OPTWIN

#ifdef OPTWIN
#define MINMAX 0
#define MIDMID 1
#define MAXMIN 2
#endif

#include "soclib/timer.h"
#include "system.h"
#include "stdio.h"
#include "fcntl.h"
#include "lena512.h"
#include "../segmentation.h"
#include "ncpu.h"

typedef unsigned char uchar;

static int interrupt_counter = 0;

static uchar pic[PSIZE][PSIZE]={0};

static int finish_bit[NCPU] = {0};

//Define the timer:
static unsigned int  time = 0;

//Irq handler

void irq_handler(int irq)
{
    printf("IRQ handle\n\n");
	uint32_t ti;
	int count = atomic_add(&interrupt_counter, 1);

	ti = soclib_io_get(
		base(TIMER),
		procnum()*TIMER_SPAN+TIMER_VALUE);
	printf("IRQ %d received at cycle %d on cpu %d, %d interrupts received\n\n", irq, ti, procnum(), count);
	soclib_io_set(
		base(TIMER),
		procnum()*TIMER_SPAN+TIMER_RESETIRQ,
		0);
}

/*{{{* 测试/
/*
// Fibo func
int fibo(int n) {
  if (n==0)
    return 1;
  else if (n==1)
    return 1;
  else return fibo(n-1) + fibo(n-2);
}
*/
/*}}}*/

void qswap (uchar * a ,uchar * b)
{
    /*{{{* 失败的三次异或/
    *   *a = (*a)^(*b);
    *b = (*a)^(*b);
    *a = (*a)^(*b);
    *//*}}}*/

    uchar tmp;
    tmp = *a;
    *a  = *b;
    *b  = tmp;
}

int qpart (uchar * window, int p,  int r)
{

    uchar middle = window[r];
#ifdef DEBUG
    printf("middle:%x\n",middle);
#endif
    int psmall = p;
    int plarge = p;
    for (; plarge < r; plarge ++)
    {
        if(window[plarge] <= middle)
        {
            qswap(&window[plarge],&window[psmall++]);
#ifdef DEBUG
            printf("Swap:%x<>%x\n",window[plarge],window[psmall-1]);
#endif
        }
    }
    qswap(&window[r],&window[psmall]);
    return psmall;
}

void qsort(uchar * window, int p , int r)
{
    if(p<r)
    {
           int q = 0;
           q = qpart(window,p,r);
           qsort(window,p,q-1);
           qsort(window,q+1,r);
        }
}

void qsort_m(uchar * window, int p , int r ,int m)
{
    if(p<r)
    {
       int q = 0;
       q = qpart(window,p,r);
       if (q == m)
       {
           return ;
       }
       else if ( q > m )
       {
          return  qsort_m(window,p,q-1,m);
       }
       else
       {
          return  qsort_m(window,q+1,r,m);
       }
    }
}

void trisort(uchar * triwindow ,uchar * min,uchar * mid,uchar * max)
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

#ifdef OPTWIN
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
#endif

uchar mfsort_m (uchar * window , int size)
{
    qsort_m(window,0,size-1,size/2);
    return window[size/2];
}

uchar mfsort (uchar * window , int size)
{
    qsort(window,0,size-1);
    return window[size/2];
}


int main(void)
{
	const int cpu = procnum();
    int i=0;
    int j=0;

#ifndef OPTWIN
    uchar window[WLENG] = {0};
#endif

	printf("Processor %d is ready \n", procnum());
	set_irq_handler(irq_handler);

/*
//  IRQS:
    irq_set_pil(0);
	enable_hw_irq(0);
	irq_enable();
*/
/*
    printf("TIMER_BASE:%x\n",base(TIMER));
    printf("TIMER_SPAN:%d\n",TIMER_SPAN);
    printf("TIMER_RERIOD:%d\n",TIMER_PERIOD);
   	time = soclib_io_get(
		base(TIMER),
		procnum()*TIMER_SPAN+TIMER_VALUE);
    printf("timer value:%d\n",time );
*/

//  Timer set:
	printf("Processor %d is setting the timer\n", procnum());
	soclib_io_set(
		base(TIMER),
		procnum()*TIMER_SPAN+TIMER_PERIOD,
        10000);

	soclib_io_set(
		base(TIMER),
		procnum()*TIMER_SPAN+TIMER_MODE,
		TIMER_RUNNING|TIMER_IRQ_ENABLED);
/*{{{*/
/*
    for(;i<100000;i++)
    {
        for(;j<10000;j++)
            ;
    }
*/
/*    for (i=0;i<20;i++)
    {
           printf("Fifo %d : %d\n",i,fibo(i));
    }

	time = soclib_io_get(
		base(TIMER),
		procnum()*TIMER_SPAN+TIMER_VALUE);
*/
/*   finish_bit[cpu]=1;

    while(1)
    {
       int flag=0;
        printf("==========\n");
        for(i=0;i<NCPU;i++)
        {
            flag+=finish_bit[cpu];
            printf("%d ",finish_bit[cpu]);
        }
        printf("\n");
        if (flag == 4)
        {
            break;
        }
    }
*//*}}}*/

	printf("Processor %d is calculating MFT,PLS wait ......\n", procnum());
    int rdistance = 0,cdistance = 0,cpux = 0, cpuy = 0;
    if (NCPU >= CALCL)
    {
        rdistance = (PSIZE)/(CALCL);
        cdistance = (PSIZE)*(CALCL)/(NCPU);
        cpux      = cpu/((NCPU)/(CALCL));
        cpuy      = cpu%((NCPU)/(CALCL));
    }
    else
    {
        rdistance = PSIZE;
        cdistance = PSIZE;
        cpux      = 0;
        cpuy      = 0;
    }
    for (i=cpux*rdistance;i<(cpux+1)*rdistance;i++)
    {
        for(j=cpuy*cdistance;j<(cpuy+1)*cdistance;j++)
        {
            // Detect the boundary of the picture
            if(i<((PSIZE)/2)||i>((PSIZE)/2)||j<((PSIZE)/2)||j>((PSIZE)/2))
            {
                pic[i][j]=lenaPic[i][j];
                continue;
            }

#ifndef OPTWIN
//Pixel calc stuff here:
            int p=0;
  #ifdef ZEROBUG
            uchar window_debug[WLENG];
  #endif
             for(p=0;p<WLENG;p++)
             {
  #ifdef DEBUG
                 int x = 0, y = 0;

                 x = i-((WSIZE/2))+(p/(WSIZE));
                 y = j-((WSIZE/2))+(p%(WSIZE));
                 window[p]=(uchar)lenaPic[x][y];

                 printf("%d:%d,%d ",p,x,y);
  #else
                 window[p]=lenaPic[i-((WSIZE)/2)+(p/(WSIZE))][j-((WSIZE)/2)+(p%(WSIZE))];
    #ifdef ZEROBUG
               //  if(window[p]==0)/*{{{*/
               //  {
               //      puts("0");
               //  }/*}}}*/
                 window_debug[p]=window[p];

     #endif

  #endif
             }
#endif


#ifdef DEBUG
             int pdebug;
             puts("\n");
             for (pdebug=0;pdebug<WLENG;pdebug++)
             {
                 printf("%3x ",window[pdebug]);
             }
             puts("\n");
#endif

//             buf = mfsort(window,WLENG);

#ifndef OPTWIN
               pic[i][j]=
#ifdef QSORTV2
                   mfsort_m
#else
                   mfsort
#endif
                   (window,WLENG);

#else
               pic[i][j] = owsort(i,j);
#endif

#ifdef ZEROBUG
               if(pic[i][j]== 0)
               {
                   printf("\n pos:%d,%d\n",i,j);
                   int pdebug2;
                   for(pdebug2=0;pdebug2<WLENG;pdebug2++)
                   {
                       printf("%3x ",window_debug[pdebug2]);
                   }
                   puts("\n");
                   PALSE
               }
#endif

#ifdef DEBUG
             for (pdebug=0;pdebug<WLENG;pdebug++)
             {
                 printf("%3x ",window[pdebug]);
             }
//             printf("\n%3x->%3x ",(uchar)lenaPic[i][j],buf);
             PALSE
#endif
//             Write the calc output here
//             write(fd,&buf,1);
        }
        printf(".");
    }

    finish_bit[cpu] = 1;

//Wait for all calc complete:

    while(1)
    {
        int flag=0;
        for(i=0;i<NCPU;i++)
        {
            flag+=finish_bit[cpu];
//          printf("%d ",finish_bit[cpu]);
        }
//      printf("\n");
        if (flag == NCPU)
        {
            break;
        }
    }

//Use processor 0 to show timer and output to file
//
if(cpu == 0)
{
//Calc the run timer  :
	time = soclib_io_get(
		base(TIMER),
		procnum()*TIMER_SPAN+TIMER_VALUE);
    printf("\nComplete!\nRUN_TIME:%d\n",time );

    int fd = open("lena.out",O_WRONLY|O_CREAT, 0644);
    if (fd < 0)
    {
        printf("Open lena.out failed!\n");
    }
//    uchar buf = PSIZE;

//    write(fd,&buf,1);

    printf("Writing the result to file ...\n");
    for(i=0;i<PSIZE;i++)
    {
        for(j=0;j<PSIZE;j++)
        {
            write(fd,&pic[i][j],1);
        }
    }
//Close file :
    close(fd);
}

    puts("Done!\n");

	while (1)
        ;
	//	pause();
	return 0;
}

