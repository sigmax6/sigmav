#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main (int argc, char const* argv[])
{
    pid_t _pid;
    pid_t child_pid;
    time_t _time;
    int timer=0;
    int  exprFlag=0;

    if (argc != 3)
    {
        printf("Argument Error!\nUsage : %s bin_name time \n",argv[0]);
        return 0;
    }

    if( (_pid=fork()) < 0)
    {
        perror("fork exec process faild!");
        exit(1);
    }

    if( _pid == 0 )
    {
        //CHILD PID
        execlp(argv[1],argv[1],NULL);
        perror("EXECPID: exec error!");
        exit(1);
    }
    else
    {
       //PARENT PID
       _time = time((time_t *)0);
       // printf("Time %ld : Process start!\n",_time);
       while((child_pid=waitpid(child_pid,(int *)0,WNOHANG)) == 0)
       {
           timer=time((time_t *)0)-_time;
           if(timer >= atoi(argv[2]))
           {
              exprFlag=1;
              break;
           }
       }
       if(exprFlag==1)
       {
           printf("PID %d was killed for time limition!\n",child_pid);
           kill(child_pid,SIGQUIT);
       }
       else
       {
           if(child_pid == -1)
           {
               printf("Execute returnd error!\n");
           }
           else
           {
               printf("Execute terminated normally, run time is %d\n",timer);
           }
       }
       return 0;
    }
}

