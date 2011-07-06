#include <stdio.h>
int main (int argc, char const* argv[])
{
    char * str="This is the first C program in Gentoo";
    int i,j,p;
    printf("%s\n",str);
    for (i=0;i<1000;i++)
    {
        for(j=0;j<1000;j++)
        {
           p=i*j;
        }
    }
    return 0;
}

