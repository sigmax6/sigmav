#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int setfl(int fd,int flags)
{
    int val;

    if ((val = fcntl(fd,F_GETFL,0)) < 0)
    {
        printf ("fcntl F_GETFL error!\n");
        return -1;
    }

    val |= flags;

    if (fcntl(fd,F_SETFL,val) < 0)
    {
        printf ("fcntl F_SETFL error\n");
        return -1;
    }

    return 0;
}

int clrfl(int fd,int flags)
{
    int val;

    if ((val = fcntl(fd,F_GETFL,0)) < 0)
    {
        printf ("fcntl F_GETFL error!\n");
        return -1;
    }

    val &= ~flags;

    if (fcntl(fd,F_SETFL,val) < 0)
    {
        printf ("fcntl F_SETFL error\n");
        return -1;
    }

    return 0;
}
