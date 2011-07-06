#include<stdio.h>
#define MAX 32
#define U   -1
int main(void)
{
    int m[MAX][MAX];
    int x,y,t,temp,xptr,yptr;
    while(scanf("%d%d%d",&y,&x,&t)!=EOF)
    {
        temp=1;
        for(xptr=0;xptr<=x;xptr++)
        {
          for(yptr=0;yptr<=y;yptr++)
          {
            m[xptr][yptr]=U;
          }
        }
        xptr=yptr=1;
        m[xptr][yptr]=temp;
        while((xptr<x&&m[xptr+1][yptr]==U)||(yptr<y&&m[xptr][yptr+1]==U)||(xptr>1&&m[xptr-1][yptr]==U)||(yptr>1&&m[xptr][yptr-1]==U))
        {
          if(!t)
          {
              if(xptr<x&&m[xptr+1][yptr]==U)
              {
                  while(xptr<x&&m[xptr+1][yptr]==U)
                  m[++xptr][yptr]=++temp;
              }
              else if(yptr<y&&m[xptr][yptr+1]==U)
              {
                  while(yptr<y&&m[xptr][yptr+1]==U)
                  m[xptr][++yptr]=++temp;
              }
              else if(xptr>1&&m[xptr-1][yptr]==U)
              {
                  while(xptr>1&&m[xptr-1][yptr]==U)
                  m[--xptr][yptr]=++temp;
              }
              else if(yptr>1&&m[xptr][yptr-1]==U)
              {
                  while(yptr>1&&m[xptr][yptr-1]==U)
                  m[xptr][--yptr]=++temp;
              }
          }
          else
          {

              if(yptr<y&&m[xptr][yptr+1]==U)
              {
                  while(yptr<y&&m[xptr][yptr+1]==U)
                  m[xptr][++yptr]=++temp;
              }
              else if(xptr<x&&m[xptr+1][yptr]==U)
              {
                  while(xptr<x&&m[xptr+1][yptr]==U)
                  m[++xptr][yptr]=++temp;
              }
              else if(yptr>1&&m[xptr][yptr-1]==U)
              {
                  while(yptr>1&&m[xptr][yptr-1]==U)
                  m[xptr][--yptr]=++temp;
              }
              else if(xptr>1&&m[xptr-1][yptr]==U)
              {
                  while(xptr>1&&m[xptr-1][yptr]==U)
                  m[--xptr][yptr]=++temp;
              }
          }
         }
         for(xptr=1;xptr<=x;xptr++)
         {
             for(yptr=1;yptr<=y;yptr++)
             {
                 printf("%4d",m[xptr][yptr]);
             }
             putchar('\n');
         }
         putchar('\n');
      }
    return 0;
}
