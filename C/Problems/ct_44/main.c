
#include<stdio.h>
#define MAX 1002
#define setMap(x,y,tx,ty,map) ((tx)>=0&&(tx)<(x)&&(ty)>=0&&(ty)<(y)&&((map)[(tx)][(ty)])>=0)?((map)[(tx)][(ty)])=((map)[(tx)][(ty)])+1:0;
int main(void)
{
    int map[MAX][MAX];
    int x,y,tx,ty;
    while(scanf("%d%d",&y,&x)!=EOF)
    {
        getchar();
        tx=x-1;
        ty=y-1;
        while(tx>=0)
        {
            ty=y-1;
            while(ty>=0)
            {
                map[tx][ty]=(getchar()=='*')?-1:0;
                getchar();
                ty--;
            }
            tx--;
        }
        tx=x-1;
        while(tx>=0)
        {
            ty=y-1;
            while(ty>=0)
            {
                if(map[tx][ty]<0)
                {
                    setMap(x,y,tx-1,ty-1,map);
                    setMap(x,y,tx-1,ty,map);
                    setMap(x,y,tx,ty-1,map);
                    setMap(x,y,tx-1,ty+1,map);
                    setMap(x,y,tx+1,ty-1,map);
                    setMap(x,y,tx+1,ty,map);
                    setMap(x,y,tx,ty+1,map);
                    setMap(x,y,tx+1,ty+1,map);
                }
                ty--;
            }
            tx--;
        }
        ty=y;
        while(x)
        {
            ty=y;
            while(ty)
            {
                (map[x-1][ty-1]<0)?printf("* "):printf("%d ",map[x-1][ty-1]);
                ty--;
            }
            printf("\n");
            x--;
        }
    }
    return 0;
}

