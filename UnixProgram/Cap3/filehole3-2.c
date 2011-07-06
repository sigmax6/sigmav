#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//Default file access permissions for new files
#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

int main (int argc, char const* argv[])
{
    off_t offset;
    char buff1[]="abcdefghij";
    char buff2[]="ABCDEFGHIJ";
    int fd;

    if((fd = creat("file.hole",FILE_MODE)) < 0)
    {
        printf ("creat file hole error!\n");
        return -1;
    }

    if(write(fd,buff1,10) != 10)
    {
        printf ("write buff1 to file error!\n");
        return -1;
    }

    if((offset = lseek(fd,40,SEEK_CUR)) == -1)
    {
        printf ("seek file error!\n");
        return -1;
    }

    if(write(fd,buff2,10) != 10)
    {
        printf ("write buff2 to file error!\n");
        return -1;
    }

    return 0;
}

