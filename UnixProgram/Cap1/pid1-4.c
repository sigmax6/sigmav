#include <stdio.h>
#include <unistd.h>

int main (int argc, char const* argv[])
{
    printf("The pid of this process is :%d \n",getpid());
    return 0;
}

