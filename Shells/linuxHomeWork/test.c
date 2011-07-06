#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#define MAX_SIZE 512
int main (int argc, char const* argv[])
{
    int fd;
    size_t read_size;
    char buff[MAX_SIZE];

    if((fd=open("tmp",O_RDONLY)) < 0)
    {
        perror("Open File tmp error!");
        exit(1);
    }
    if((read_size=read(fd,buff,20)) < 0)
    {
        perror("Read File tmp error!");
        exit(1);
    }
    printf("Read: %s \n" ,buff);
    return 0;
}

