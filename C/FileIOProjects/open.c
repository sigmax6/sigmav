#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

//int open(const char * name,int flags);
//int open(const char * name,int flags, mode_t mode);  //declare open()

int main(int argc,char ** argv)
{
    int fd;
    fd=open("/root/C/fileIO/test",O_RDONLY);//O_WRONLY O_RDWR
    if (-1==fd)
    {
        printf("open failed!\n");
    }
    else
    {
        printf("open succeed!\n");
    }
    return 0;
}
