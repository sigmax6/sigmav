#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOWSIZE 9

void qswap (int * a ,int * b)
{
 /*   *a = (*a)^(*b);
  *       *b = (*a)^(*b);
  *           *a = (*a)^(*b);
  *               */
    int tmp;
    tmp = *a;
    *a  = *b;
    *b  = tmp;
}

int qpart (int * window, int p,  int r)
{

    int middle = window[r];
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

void myqsort(int * window, int p , int r)
{
    if(p<r)
    {
               int q = 0;
               q = qpart(window,p,r);
               myqsort(window,p,q-1);
               myqsort(window,q+1,r);
            }
}

void myqsort_m(int * window, int p , int r ,int m)
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
                     return  myqsort_m(window,p,q-1,m);
                  }
           else
           {
                     return  myqsort_m(window,q+1,r,m);
                  }
        }
}

char mfsort (int * window , int size)
{
    myqsort(window,0,size-1);
    return window[size/2];
}

char mfsort_m (int * window , int size)
{
    myqsort_m(window,0,size-1,size/2);
    return window[size/2];
}

int main (int argc, char const* argv[])
{
    int window[WINDOWSIZE]={0};
    int window_dist[WINDOWSIZE]={0};
    int i;
    clock_t time;
    while(1)
    {
        puts("========================================\n");
        for (i=0;i<WINDOWSIZE;i++)
        {
            window[i]=window_dist[i]=rand();
        }
        puts("window before sort :\n");
        for (i=0;i<WINDOWSIZE;i++)
        {
            printf("%d ",window[i]);
        }
        puts("\nwindow after myqsort :\n");
        time = clock();
        mfsort(window,WINDOWSIZE);
        for(i=0;i<WINDOWSIZE;i++)
        {
            printf("%d ",window[i]);
        }
        time = clock()-time;
        printf("\nmyqsort time : %ld",time);
        puts("\nwindow after myqsort_m :\n");
        time = clock();
        mfsort_m(window_dist,WINDOWSIZE);
        for(i=0;i<WINDOWSIZE;i++)
        {
            printf("%d ",window_dist[i]);
        }
        time = clock()-time;
        printf("\nmyqsort_m time : %ld",time);
        getchar();
    }

    return 0;
}

