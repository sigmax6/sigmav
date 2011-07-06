#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char const* argv[])
{
    int fd=1;
    printf("dup2 0:%d\n",dup2(fd,0));
    printf("dup2 1:%d\n",dup2(fd,1));
    printf("dup2 2:%d\n",dup2(fd,2));

    printf("fd:%d",fd);

    if(fd > 2)
    {
        close(fd);
    }

    return 0;
}

