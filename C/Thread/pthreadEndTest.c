#include <stdio.h>
#include <pthread.h>
char * str="Test\n";
void print_pthread(void)
{
    while(1)
    {
        malloc(40960);
       printf(str);
    }
}
int main (int argc, char const* argv[])
{
    pthread_t fd;
    int res;
    void * thread_result;
//    char * str="Test str\n";
    pthread_create(&fd,NULL,(void *)&print_pthread,NULL);
    res=pthread_join(fd,&thread_result);
    if(res<0)
    {
        printf("Error!\n");
    }
    exit(0);
}

