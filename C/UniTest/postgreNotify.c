#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <errno.h>

/*#define DEBUG*/

#define FILENAME_LEN 256
#define CONTENT_LEN 1024
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024*(EVENT_SIZE + 16))

int main (int argc, char const* argv[])
{
    int length,i=0;
    int fd;
    int wd;

    int ifd;

    char buffer[BUF_LEN];
    char * dirpath="/var/log/";
    char * logfile="remotepostgre.log";

    DIR * dir;
    struct dirent * pdir;

    time_t newest = 0;
    char   newestFile[FILENAME_LEN];
    struct stat  fileStat;
    off_t  newestFileSize;
    char   newContentBuf[CONTENT_LEN];

    //Init inotify
    fd=inotify_init();

    if(fd < 0)
    {
        perror("inotify_init error!");
        exit(1);
    }

    if((dir=opendir(dirpath)) == NULL)
    {
        perror("Open dir error!");
        exit(1);
    }

    if(chdir(dirpath) == -1)
    {
        perror("change dir error!");
        exit(1);
    }
    wd=inotify_add_watch(fd,dirpath
                         ,IN_MODIFY|IN_CREATE|IN_DELETE);


    while(1)
    {

        i=0; //counter

        length=read(fd,buffer,BUF_LEN);

        if (length < 0)
        {
            perror("read error!");
            continue;
        }

        while(i<length)
        {
            struct inotify_event *event = (struct inotify_event *)  &buffer[i];
            if(event->len)
            {
                if(event->mask & IN_CREATE)
                {
                    if(!(event->mask & IN_ISDIR))
                    {
                        if( strcmp(event->name,logfile) == 0)
                        {
                            if((ifd=open(event->name,O_RDONLY)) == -1)
                            {
                                perror("Open new file error");
                                continue;
                            }
                            if(fstat(ifd,&fileStat) == -1)
                            {
                                perror("Get file stat error!");
                                continue;
                            }
                            close(ifd);
                            if(fileStat.st_mtime > newest)
                            {
                                newest=fileStat.st_mtime;
                                newestFileSize=fileStat.st_size;
                                printf("newest file size:%d\n",newestFileSize);
                            }
                        }
                    }
                }
                else if(event->mask & IN_MODIFY)
                {
                    if(!(event->mask & IN_ISDIR))
                    {
                        if( strcmp(event->name,logfile) == 0)
                        {
                            if((ifd=open(event->name,O_RDONLY)) == -1)
                            {
                                perror("Open modified file error");
                                continue;
                            }
                            if(fstat(ifd,&fileStat) == -1)
                            {
                                perror("Get file stat error!");
                                continue;
                            }

                            off_t newContentSize = fileStat.st_size - newestFileSize;
                            if(lseek(ifd,-newContentSize,SEEK_END) == -1)
                            {
                                perror("Lseek error!");
                                continue;
                            }
                            if(read(ifd,&newContentBuf,newContentSize) == -1)
                            {
                                perror("Read error!");
                                continue;
                            }
                            if(lseek(ifd,0,SEEK_END) == -1)
                            {
                                perror("Lseek error!");
                                continue;
                            }
                            close(ifd);

                            newestFileSize = fileStat.st_size;
                            newest = fileStat.st_mtime;

                            printf("===================================\n");
                            newContentBuf[newContentSize]='\0';
                            printf(newContentBuf);

                            //Do your stuff here
                            //with newContentBuf

                        }
                    }
                }
            }
            i +=EVENT_SIZE + event->len;
        }//while i<length
    } //while 1

    inotify_rm_watch(fd,wd);
    close(fd);

    exit(0);
}


