#include<stdio.h>
#define MAX 1002
#define MINE -10

int main(void)
{
    char mineMap[MAX][MAX];
    int x,y,tx,ty,outTemp;
    while(scanf("%d%d",&y,&x)!=EOF)
    {
        getchar();
        for(tx=1;tx<=x;tx++)
        {
            for(ty=1;ty<=y;ty++)
            {
                mineMap[tx][ty]=(getchar()=='*')?MINE:0;
            }
            getchar();
        }
        for(tx=0;tx<=x+1;tx++)
        {
            mineMap[tx][0]=0;
            mineMap[tx][y+1]=0;
        }
        for(ty=0;ty<=y+1;ty++)
        {
            mineMap[0][ty]=0;
            mineMap[x+1][ty]=0;
        }
        for(tx=1;tx<=x;tx++)
        {
            for(ty=1;ty<=y;ty++)
            {
                if(mineMap[tx][ty]<0)
                {
                    mineMap[tx][ty-1]++;
                    mineMap[tx][ty]++;
                    mineMap[tx][ty+1]++;
                }
            }
        }
        for(tx=1;tx<=x;tx++)
        {
            for(ty=1;ty<=y;ty++)
            {
                if(mineMap[tx][ty]<0)
                {
                    putchar('*');
                }
                else
                {
                    outTemp=mineMap[tx][ty];
                    outTemp+=(mineMap[tx+1][ty]<0)?(mineMap[tx+1][ty]-MINE):(mineMap[tx+1][ty]);
                    outTemp+=(mineMap[tx-1][ty]<0)?(mineMap[tx-1][ty]-MINE):(mineMap[tx-1][ty]);
                    putchar(outTemp+'0');
                }
            }
            putchar('\n');
        }
        putchar('\n');
    }
    return 0;
}



