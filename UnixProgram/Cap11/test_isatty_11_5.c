#include <stdio.h>
#include <stdlib.h>
extern int isatty(int fd);
int main (int argc, char const* argv[])
{
    int counter;
    for(counter=0;counter<3;counter++)
    {
        printf("fd %d: %s \n",counter,isatty(counter)?"tty":"not a tty!");
    }
    exit(0);
}
