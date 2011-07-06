#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    printf("the pid of this program is %d\n",getpid());
    exit(0);
}
