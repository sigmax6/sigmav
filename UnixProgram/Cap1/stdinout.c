#include <stdio.h>

int main (int argc, char const* argv[])
{
    int c;
    while((c=getc(stdin)) != EOF)
    {
       if(putc(c,stdout) == EOF )
       {
           printf("write stdout error!\n");
           return -1;
       }
    }
    if (ferror(stdin))
    {
        printf("read stdin error!\n");
        return -1;
    }
    return 0;
}

