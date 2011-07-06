#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 4096
#define MAX_ARGV 3

int main (int argc, char const* argv[])
{
    char buff[MAX_LINE];
    pid_t pid;
    int status;
    char * argvs[MAX_ARGV];
    char * tmp;
    int argcounter;
    int flag;
    int ret;

    printf("%% ");
    while(fgets(buff,MAX_LINE,stdin) != NULL)
    {
        flag=0;
        argcounter = 0;
        tmp = buff;

        //Count the argvs
        while (*tmp != '\n')
        {
           if(* tmp == ' ')
           {
               * tmp = 0;
               flag = 0;
           }
           else if(flag == 0 )
           {
               argvs[argcounter ++] = tmp;
               flag = 1;
           }

           if (argcounter == 3)
           {
               break;
           }

           tmp ++;
        }
        *tmp = 0;

        //Fork a new pid
        if((pid = fork()) < 0 )
        {
            printf ("fork error!\n");
            return -1;
        }

        //Child
        else if (pid == 0)
        {
            switch(argcounter)
            {
            case 1:
                ret=execlp(argvs[0],argvs[0],(char *)0);
                break;
            case 2:
                ret=execlp(argvs[0],argvs[0],argvs[1],(char *)0);
                break;
            case 3:
                ret=execlp(argvs[0],argvs[0],argvs[1],argvs[2],(char *)0);
                break;
            case 0:
                ret=execlp(argvs[0],argvs[0],(char *)0);
                ret=0;
                break;
            default:
                printf("argv error!\n");
                break;
            }
            if (ret == -1)
            {
                printf("cannot execute :%s\n",buff);
                return -1;
            }
        }

        //Parent
        if ((pid = waitpid(pid,&status,0)) < 0)
        {
            printf ("wait pid error !\n");
            return -1;
        }

        printf ("%% ");

    }

    return 0;

}

