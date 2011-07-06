/*　Linux系统下的多线程遵循POSIX线程接口，称为pthread。编写Linux下的多线程程序，需要使用头文件pthread.h，连接时需要使用库libpthread.a。顺便说一下，Linux下pthread的实现是通过系统调用clone（）来实现的。clone（）是Linux所特有的系统调用，它的使用方式类似fork，关于clone（）的详细情况，有兴趣的读者可以去查看有关文档说明。下面我们展示一个最简单的多线程程序example1.c。*/
#include<stdio.h>
#include<pthread.h>
void thread(void)
{
    int i;
    for(i=0;i<3;i++)
        printf("This is a pthread.\n");
}
int main(void)
{
    pthread_t id;            //typedef unsigned long int pthread_t;
    int i,ret;
    ret=pthread_create(&id,NULL,(void *)thread,NULL);
    if(ret!=0)
    {
        printf("Creat pthread error!\n");
       // exit(1);
       return -1 ;
    }
    for(i=0;i<3;i++)
        printf("This is the main process.\n");
    pthread_join(id,NULL);
    return 0;
}
