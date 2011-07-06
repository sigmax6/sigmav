#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 4096

int main (int argc, char const* argv[])
{
    char buff[MAXLINE];
    pid_t pid;
    int status;
    int ret;

    printf("%%");
    while(fgets(buff,MAXLINE,stdin) != NULL)
    {
        buff[strlen(buff)-1] = 0; //Replace the newline with null

        if((pid = fork()) < 0) //Fork
        {
            printf("fork error!\n");
            return -1;
        }

        else if(pid == 0)   //Child
        {
            if((ret = execlp(buff,buff,(char *)0)) == -1)
            {
                printf("couldn't execute :%s\n",buff);
                return -1;
            }
        }

        if((pid = waitpid(pid,&status,0)) < 0)
        {
            printf("waitpid error\n");
            return -1;
        }

        printf("%%");
    }

    return 0;
}

