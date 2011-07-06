#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#define MAXLINE 100

int main(void)
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    printf("%% ");
    while(fgets(buf,MAXLINE,stdin)!=NULL)
    {
        if(buf[strlen(buf)-1]=='\n')
            buf[strlen(buf)-1]=0;
        if((pid=fork())<0)//error
        {
            perror("fork error");
        }
        else if(pid==0)   //child
        {
            execlp(buf,buf,(char *)0);
            printf("coulden't execute:%s",buf);
            exit(127);
        }
        if((pid=waitpid(pid,&status,0))<0) //parent
        {
            perror("waitpid error");
        }
        printf("%% ");
    }
    exit(0);
}
