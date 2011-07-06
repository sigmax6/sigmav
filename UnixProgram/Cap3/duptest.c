#ifndef UNIFUNC
#define UNIFUNC 1
#endif
#ifndef DEBUG
#define DEBUG 1
#endif

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#if UNIFUNC
int mydup2(int filedes,int filedes2);

int main (int argc, char const* argv[])
{
    int fd;
    int fdup;
    int fdup2;
    int fmydup;
   // if((fd = open(STDIN_FILENO,O_RDWR))<0)
   // {
   //     printf("open files error!\n");
   //     return -1;
   // }
    fd=STDIN_FILENO;
    printf("fd value:%d\n",fd);

    if((fdup = dup(fd)) == -1)
    {
        printf("dup error!\n");
        return -1;
    }
    printf("dup value:%d\n",fdup);

    if((fdup2 = dup2(fd,5)) == -1)
    {
        printf ("dup2 error!\n");
        return -1;
    }
    printf ("dup2 value:%d\n",fdup2);

    if((fmydup = mydup2(fd,6)) == -1)
    {
        printf ("mydup2 error!\n");
        return -1;
    }
    printf ("mydup2 value:%d\n",fmydup);

//    if(close(fd) < 0)
//    {
//        printf ("close fd error!\n");
//        return -1;
//    }

    return 0;

}

#endif

int mydup2(int filedes,int filedes2)
{
    if(filedes2 < filedes)
    {
        //debug
#if DEBUG
        printf("<");
#endif
        return -1;
    }

    if(filedes2 == filedes)
    {
        return filedes;
    }

//    if(close(filedes2) < 0)
//    {
//        //debug
//#if DEBUG
//        printf("close error");
//#endif
//        return -1;
//    }

    close(filedes2);

    int descounter;

    while((descounter=dup(filedes)) != filedes2)
    {
        if(descounter == -1)
        {
            //debug
#if DEBUG
            printf("dup descounter error");
#endif
            return -1;
        }
    }

    for(;descounter > filedes;descounter --)
    {
//        if(close(descounter)<0)
//        {
//#if DEBUG
//            printf("close descounter %d error",descounter);
//#endif
//            return -1;
//        }
       close(descounter);
    }

    return filedes2;

}

