#include<stdio.h>
#include<time.h>
int delay(double delaytime)      //延时函数 delaytime:延时时间 以S为单位
{
    time_t start,end;            //计时的起至时间
    time(&start);
    do
    {
        time(&end);
    }
    while(difftime(end,start)<delaytime);
    return 0;
}
int main (int argc, char const* argv[])
{
    int i=0;
    while(1)
    {
        /*
        switch(i)
        {
        case 0:
            putchar('-');
            delay(1);
            printf("\r");
            i++;
            break;
        case 1:
            putchar('\\');
            delay(1);
            printf("\r");
            i++;
            break;
        case 2:
            putchar('|');
            delay(1);
            printf("\r");
            i++;
            break;
        case 3:
            putchar('/');
            delay(1);
            printf("\r");
            i=0;
            break;
        default:
            break;
            }
            */
           printf("\rcalculating time.....");
           printf("%c ","-\\|/"[(i++)%4]);
           fflush(stdout);
           delay(0.1);
    }
}
