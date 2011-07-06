//---- ELEVATOR V2 @Sigmav.net---------//
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

//-------同步资源区----------
sem_t bin_sem1;  //同步信号量1 :输入-进行处理信号量
sem_t bin_sem2;  //同步信号量2 :处理完毕-可以输入
#define FLOORLEVEL 3
int floor_order[FLOORLEVEL]={0};
int floor_order_rec[FLOORLEVEL]={0};
int floor_ptr=0;
//---------------------------

//-------按键控制线程--------
void * ele_button()
{
    int key;
    while(1)
    {
        scanf("%d",&key);
        sem_wait(&bin_sem2);
        if(key>=1&&key<=3&&!floor_order_rec[key])
        {
            floor_order_rec[key]=1;    //记录该楼层命令
            floor_order[floor_ptr]=key; //记录目标楼层
            floor_ptr=(++floor_ptr)%3;  //循环使用存储空间
            sem_post(&bin_sem1);       //释放信号量，可以进行读操作
        }
    }
}
void * ele_print()
{
    int floor=1;                    //当前楼层计数器
    int floor_dst=0;                //目标楼层计数器

    while(1)
    {
        sem_wait(&bin_sem1);        //等待释放读指令信号量
        if(floor_order[floor_ptr]!=0&&floor_order[floor_ptr]!=floor&&floor_order_rec[floor_order[floor_ptr]-1]!=0)
        {
            floor_dst=floor_order[floor_ptr];   //设置目标楼层
            floor_order[floor_ptr]=0;           //清空指令
            floor_order_rec[floor_order[floor_ptr]-1]=0;
            sem_post(&bin_sim2);      //本次处理完毕，可以继续输入处理
        }
        else
        {
            sem_post(&bin_sim2);
            continue;
        }


    }

}
int main (int argc, char const* argv[])
{

}
