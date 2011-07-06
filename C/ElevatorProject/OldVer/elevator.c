#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
/**********************/
int eleorder[4]={0};            //公共资源：存放电梯指令

/**********************/
void getkey(void)
{
  int key;
  while(1)
  {
    key=getchar()-'0';
    //scanf("%d",&key);
    if(key>=1&&key<=4&&eleorder[key-1]!=1)
    {
        eleorder[key-1]=1;
//      printf("#######%d",key);      //DEBUG
    }
  }
}
void eprint(void)
{
  int elefloor=1;             // floor
  int elestate=0;              // 0 closed 1 open
  int elefloorarr[4]={0};
  int upordown=1;              // 1 up 0 down;
  int stopped=0;               // 1 stop
  int elefloortemp;
  int i;
  int j;
  while(1)
  {
    if(1==upordown)
     {
        for(i=elefloor;i<4;i++)
        {
          if(eleorder[i]==1) break;
        }
        if(i>=4)
        {
            upordown=0;
            for(i=elefloor-2;i>=0;i--)
            {
               if(eleorder[i]==1) break;
            }
            if(i<0)
               stopped=1;
            else
               stopped=0;
        }
     }
     else
     {
        for(i=elefloor-1;i>=0;i--)
        {
          if(eleorder[i]==1) break;
        }
        if(i<0)
        {
            upordown=1;
            for(i=elefloor;i<4;i++)
            {
               if(eleorder[i]==1) break;
            }
            if(i==4)
               stopped=1;
            else
                stopped=0;
        }
     }
     elefloortemp=elefloor;
    if(!stopped)
    {
     elefloor=i+1;
     while(elefloortemp!=elefloor)
     {
       for(i=0;i<4;i++)
       {
          elefloorarr[i]=0;
       }
       elefloorarr[elefloortemp-1]=1;
      // system("clear");
       printf(" 1 2 3 4\n");
       printf("%2d%2d%2d%2d\n",elefloorarr[0],elefloorarr[1],elefloorarr[2],elefloorarr[3]);
       printf("%2d%2d%2d%2d\n",eleorder[0],eleorder[1],eleorder[2],eleorder[3]);
       (upordown==1)?elefloortemp++,printf("running up\n"):elefloortemp--,printf("running down\n");
       sleep(1);
     }
      for(i=0;i<4;i++)
       {
          elefloorarr[i]=0;
       }
       elefloorarr[elefloor-1]=1;
    // system("clear");
     printf(" 1 2 3 4\n");
     printf("%2d%2d%2d%2d\n",elefloorarr[0],elefloorarr[1],elefloorarr[2],elefloorarr[3]);
     printf("%2d%2d%2d%2d\n",eleorder[0],eleorder[1],eleorder[2],eleorder[3]);
     printf("Door opened!\n");
    // system("clear");
     sleep(1);
     printf(" 1 2 3 4\n");
     printf("%2d%2d%2d%2d\n",elefloorarr[0],elefloorarr[1],elefloorarr[2],elefloorarr[3]);
     printf("%2d%2d%2d%2d\n",eleorder[0],eleorder[1],eleorder[2],eleorder[3]);
     printf("Door closed!\n");
     sleep(1);
    }
    else
    {
    // system("clear");
     printf(" 1 2 3 4\n");
     printf("%2d%2d%2d%2d\n",elefloorarr[0],elefloorarr[1],elefloorarr[2],elefloorarr[3]);
     printf("%2d%2d%2d%2d\n",eleorder[0],eleorder[1],eleorder[2],eleorder[3]);
     printf("Waiting!\n");
     sleep(1);
    }
    eleorder[elefloor-1]=0;
  }
 }
/***********************主进程函数************************/
int main(int argc,char** argv)
{
//   int res;                                            //存放状态获取结果
//   void * thread_result;                                 //子线程状态结果
   pthread_t getkey_t,print_t;                             //线程ID
   pthread_create(&getkey_t,NULL,(void *)&getkey,NULL);    //键盘获取线程
   pthread_create(&print_t,NULL,(void *)&eprint,NULL);     //逻辑打印线程
/*******************************************
   res=pthread_join(print_t,&thread_result);
   if(res<0)
   {
       printf("Join Error!\n");
   }
********用jion的方式等待线程结束***********/
   pthread_exit(NULL);                                   //等待所有线程结束
   exit(0);
}
