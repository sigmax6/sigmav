#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define BUFF_SIZE 4096

int setfl(int fd,int flags);

int clrfl(int fd,int flags);

int main (int argc, char const* argv[])
{
    char buf[BUFF_SIZE];
    int ssize;

    setfl(STDOUT_FILENO,O_SYNC);

    while((ssize = read(STDIN_FILENO,buf,BUFF_SIZE)) >0)
    {
        if(write(STDOUT_FILENO,buf,ssize) != ssize)
        {
            printf("write error!\n");
            return -1;
        }
    }

    if(ssize < 0)
    {
        printf ("read error!\n");
        return -1;
    }

    return 0;
}

