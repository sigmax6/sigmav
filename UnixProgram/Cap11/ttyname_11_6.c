#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define DEV "/dev/"
#define DEVLEN sizeof(DEV)-1          //sizeof include NULL at end

char * ttyname(int fd)
{
    struct stat fdstat,devstat;      //structs to get the states;
    DIR *dp;                         //Directory name pointer;
    struct dirent *dirp;             //Directory pointer;
    static char pathname[_POSIX_PATH_MAX+1];
    char * rval;

    if(!isatty(fd))
    {
        return (NULL);              //Not a tty then return NULL
    }

    if(fstat(fd,&fdstat)<0)
    {
        return (NULL);             //Cannot get the state of fd then return NULL
    }

    if(S_ISCHR(fdstat.st_mode)==0)
    {
        return (NULL);
    }

    strcpy(pathname,DEV);

    if((dp=opendir(DEV))==NULL)
    {
        return (NULL);
    }

    rval=NULL;

    while((dirp=readdir(dp))!=NULL)
    {
        if(dirp->d_ino!=fdstat.st_ino)      //If the file inode is not the same with the device
        {
            continue;                      //Jump out of the circle once. fast test can skip most entires
        }

        strncpy(pathname+DEVLEN,dirp->d_name,_POSIX_PATH_MAX-DEVLEN); // /dev/<- put the file name here

        if(stat(pathname,&devstat)<0)      //Is not a device file
        {
            continue;                      //Jump out once
        }

        if(devstat.st_ino==fdstat.st_ino && devstat.st_dev==fdstat.st_dev)
        {
            rval=pathname;                //find device and match
            break;                        //End of the circle
        }

    }

    closedir(dp);
    return (rval);

}

