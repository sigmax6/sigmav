#include <stdio.h>

void diamond(int d,char c)
{
    if (d%2 == 0)
    {
        printf("error num!\n");
        return ;
    }
    int mid=d/2;
    int i,j;
    int count;
    for (i=0;i < d;i++)
    {
        if(i <= mid)
        {
            count = mid-i;
#ifdef DEBUG/*{{{*/
            printf("%d:%d-%d",count,mid,i);
#endif/*}}}*/
        }
        else
        {
            count = i-mid;
#ifdef DEBUG/*{{{*/
            printf("%d:%d-%d",count,i,mid);
#endif/*}}}*/
        }
        for (j=0;j < count;j++)
        {
            printf("\t");
        }
        for (j=0;j < d-2*count;j++)
        {
            printf("%c\t",c);
        }
        for (j=0;j < count;j++)
        {
            printf("\t");
        }
        puts("");
    }
}

int main (int argc, char const* argv[])
{
    diamond(3,'*');
    puts("");
    diamond(5,'+');
    return 0;
}
