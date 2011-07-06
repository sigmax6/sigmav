#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char const* argv[])
{
    struct stat buf;
    int counter;
    for (counter=1;counter<argc;counter++)
    {
        if(lstat(argv[counter],&buf)<0)
        {
            printf("lstat %s error!\n",argv[counter]);
            return -1;
        }
        printf("file %s is a ",argv[counter]);
        if(S_ISREG(buf.st_mode)) printf("regular file\n");
        else if(S_ISDIR(buf.st_mode)) printf("directory \n");
        else if(S_ISCHR(buf.st_mode)) printf("cariacter special\n");
        else if(S_ISBLK(buf.st_mode)) printf("block special\n");
        else if(S_ISFIFO(buf.st_mode)) printf("fifo\n");
#ifdef S_ISLNK
        else if(S_ISLNK(buf.st_mode)) printf("symbolic link\n");
#endif
#ifdef S_ISSOCK
        else if(S_ISSOCK(buf.st_mode)) printf("socket\n");
#endif
        else printf("unknown mode\n");
    }
    return 0;
}

