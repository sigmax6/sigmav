#include <stdio.h>

int main (int argc, char const* argv[])
{
    int d,r,i,j,h,k;
    while((scanf("%d",&d) != EOF) && ((d&1) != 0))
    {
        r=d/2;
        for(i=0;i<d;i++)
        {
            h=((h=(r-i))>0)?h:-h;
            k=d-2*(h+1);

            for(j=0;j<h;j++)
                putchar(' ');

            if(k==-1)
                putchar('*');
            else
            {
                putchar('*');
                for(j=0;j<k;j++)
                    putchar(' ');
                putchar('*');
            }

            for(j=0;j<h;j++)
                putchar(' ');

            putchar('\n');
        }
    }

    return 0;
}

