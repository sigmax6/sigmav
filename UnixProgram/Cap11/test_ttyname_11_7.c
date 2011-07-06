#include <stdio.h>
#include <stdlib.h>
extern int isatty(int fd);
extern char * ttyname(int fd);
int main (int argc, char const* argv[])
{
    int counter;
    for(counter=0;counter<10;counter++)
    {
        printf("fd %d: %s \n",counter,isatty(counter)?ttyname(counter):"not a tty!");
    }
    exit(0);
}
