#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>

int main (int argc, char const* argv[])
{
    int accmode , val;

    if(argc != 2)
    {
        printf ("Usage: accmode <description>\n");
        return -1;
    }

    if ((val = fcntl(atoi(argv[1]),F_GETFL,0)) < 0)
    {
        printf ("fcntl error for fd %d",atoi(argv[1]));
        return -1;
    }

    accmode = val & O_ACCMODE;

    if (accmode == O_RDONLY)
    {
        printf ("read only");
    }
    else if (accmode == O_WRONLY)
    {
        printf ("write only");
    }
    else if (accmode == O_RDWR)
    {
        printf ("read write");
    }
    else
    {
        printf ("unknown accmode !\n");
        return -1;
    }

    if (val & O_APPEND)
    {
        printf (",append");
    }
    if (val & O_NONBLOCK)
    {
        printf (",nonblock");
    }
    if (val & O_SYNC)
    {
        printf (",sychronous writes");
    }

    putchar('\n');

    return 0;

}

