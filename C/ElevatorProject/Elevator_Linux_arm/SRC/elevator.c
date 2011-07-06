/**********************************/
/*    电梯程序-状态机-@sigmax6    */
/*         ARM Embest 版本        */
/**********************************/

#include<stdio.h>
#include<pthread.h>

#define MAX_FLOOR 3

extern int get_key();
extern int led(int led_num);

/**********************************************************/ //数据结构定义.

typedef enum Status{              // STATUS 类型: 记录电梯状态
    open,
    close,
    run,
    stop
}STATUS;

typedef enum OutReq{              // QUTREQ 类型: 记录外部请求
    up_req,
    down_req,
    both,
    no
}OUTREQ;

typedef enum Dir{                 //DIRECTION 类型: 记录运行方向
    up,
    down,
    still
}DIRECTION;

typedef struct Requests{         //REQURST  类型:  记录请求,Req表示请求类型,-1无情求,0为内部请求,1为外部请求
    int    Req;
    OUTREQ outReq;
}REQUEST;


typedef struct ElevatorInfo{     //ELEINFO_T 类型:  存放当前电梯状态
    DIRECTION Direction;
    int       Floor;
    STATUS    Statu;
    REQUEST   Request[MAX_FLOOR];
}ELEINFO_T;

typedef struct Key{               //KEY_T    类型:  存放当前按键信息
    char       typeFlag;
    int        floor;
    char       updownFlag;
}KEY_T;
//--------------------------------
ELEINFO_T ELEINFO;                //ELEINFO  存储电梯状态
KEY_T KEY;                        //KEY      存储按键信息


/**********************************************************/ //电梯初始化函数
void initEle(void)
{
    int temp;                      //计数器
    ELEINFO.Direction=still;       //电梯初始状态,在1层,无方向,状态为停止.
    ELEINFO.Floor=0;
    ELEINFO.Statu=stop;
    for(temp=0;temp<MAX_FLOOR;temp++)
    {
        ELEINFO.Request[temp].Req=-1;
        ELEINFO.Request[temp].outReq=no;
    }
}

/**********************************************************/  //打印当前状态函数
void printEle(void)
{
    printf("%d : ",ELEINFO.Floor+1);  //楼层:
    led(ELEINFO.Floor);
    switch(ELEINFO.Statu)
    {
    case stop:
        printf("Stopping!\n");
        break;
    case run:
        printf("Running!\n");
        break;
    case open:
        printf("Door Opened!\n");
        break;
    case close:
        printf("Door Closed!\n");
        break;
    }
}

/**********************************************************/  //线程1:获取输入(KEY)并处理为请求(REQUEST)
void * getKey(void)
{
    int key;
    while(1)
    {
        //scanf("%d",&KEY.floor);      //这里方便测试,没有设计复杂的按键处理
        /*
         * 可扩展按键处理
         */
        key=get_key();
        switch(key)
        {
        case 1:
        case 2:
        case 3:                         //1,2,3按键对应1-3层内部请求.
             ELEINFO.Request[KEY.floor-1].Req=0;;
             break;
        case 4:
        case 5:
        case 6:                         //4,5,6对应1-3层外部向上请求
            (ELEINFO.Request[KEY.floor-4].outReq==down_req)?(ELEINFO.Request[KEY.floor-4].outReq=both):(ELEINFO.Request[KEY.floor-4].outReq=up_req);
             ELEINFO.Request[KEY.floor-4].Req=1;
             break;
        case 7:
        case 8:
        case 9:                         //7,8,9对应1-3层外部向下请求
             (ELEINFO.Request[KEY.floor-7].outReq==up_req)?(ELEINFO.Request[KEY.floor-7].outReq=both):(ELEINFO.Request[KEY.floor-7].outReq=down_req);
             ELEINFO.Request[KEY.floor-7].Req=1;
             break;
        default:
             printf("ERROR!\n");
             break;
        }
    }
}

/**********************************************************/  //线程2 :电梯状态转换处理函数
void * procEle(void)
{
    int floorPtr;
    while(1)
    {
        switch(ELEINFO.Statu)                                 //依据当前状态判断处理方式
        {
        case stop:                                         //停止状态处理------------------------
            if(ELEINFO.Request[ELEINFO.Floor].Req!=-1)
            {
                ELEINFO.Request[ELEINFO.Floor].Req=-1;
                ELEINFO.Request[ELEINFO.Floor].outReq=no;
                ELEINFO.Statu=open;
            }
            else
            {
                floorPtr=ELEINFO.Floor;
                while(floorPtr<MAX_FLOOR)
                {
                    if(ELEINFO.Request[floorPtr].Req!=-1)
                        break;
                    floorPtr++;
                }
                if(floorPtr<MAX_FLOOR)
                {
                    ELEINFO.Direction=up;
                    ELEINFO.Statu=run;
                }
                else
                {
                    floorPtr=0;
                    while(floorPtr<ELEINFO.Floor)
                    {
                        if(ELEINFO.Request[floorPtr].Req!=-1)
                            break;
                        floorPtr++;
                    }
                    if(floorPtr<ELEINFO.Floor)
                    {
                        ELEINFO.Direction=down;
                        ELEINFO.Statu=run;
                    }
                }
            }
            break;
        case run:                                             //运行状态处理---------------------
            if(ELEINFO.Direction==up)
            {
                if(ELEINFO.Request[ELEINFO.Floor].Req!=-1&&ELEINFO.Request[ELEINFO.Floor].outReq!=down_req)
                {
                    ELEINFO.Statu=open;
                }
                else
                {
                    floorPtr=ELEINFO.Floor;
                    while(floorPtr<MAX_FLOOR)
                    {
                        if(ELEINFO.Request[floorPtr].Req!=-1)
                            break;
                        floorPtr++;
                    }
                    if(floorPtr<MAX_FLOOR)
                    {
                        ELEINFO.Floor++;
                    }
                    else if(ELEINFO.Request[ELEINFO.Floor].Req!=-1&&ELEINFO.Request[ELEINFO.Floor].outReq==down_req)
                    {
                        ELEINFO.Statu=open;
                    }
                    else
                    {
                        floorPtr=0;
                        while(floorPtr<ELEINFO.Floor)
                        {
                            if(ELEINFO.Request[floorPtr].Req!=-1)
                                break;
                            floorPtr++;
                        }
                        if(floorPtr<ELEINFO.Floor)
                        {
                            ELEINFO.Direction=down;
                        }
                    }
                }
            }
            else
            {
                if(ELEINFO.Request[ELEINFO.Floor].Req!=-1&&ELEINFO.Request[ELEINFO.Floor].outReq!=up_req)
                {
                    ELEINFO.Statu=open;
                }
                else
                {
                    floorPtr=0;
                    while(floorPtr<ELEINFO.Floor)
                    {
                        if(ELEINFO.Request[floorPtr].Req!=-1)
                            break;
                        floorPtr++;
                    }
                    if(floorPtr<ELEINFO.Floor)
                    {
                        ELEINFO.Floor--;
                    }
                    else if(ELEINFO.Request[ELEINFO.Floor].Req!=-1&&ELEINFO.Request[ELEINFO.Floor].outReq==up_req)
                    {
                        ELEINFO.Statu=open;
                    }
                    else
                    {
                        floorPtr=ELEINFO.Floor;
                        while(floorPtr<MAX_FLOOR)
                        {
                            if(ELEINFO.Request[floorPtr].Req!=-1)
                                break;
                            floorPtr++;
                        }
                        if(floorPtr<ELEINFO.Floor)
                        {
                            ELEINFO.Direction=up;
                        }
                    }
                }
            }
            break;
        case open:                                             //开门状态处理----------------------
            ELEINFO.Statu=close;
            if(ELEINFO.Direction==up)
            {
                if(ELEINFO.Request[ELEINFO.Floor].Req==1&&ELEINFO.Request[ELEINFO.Floor].outReq==both)
                {
                    ELEINFO.Request[ELEINFO.Floor].outReq=down_req;
                }
                else if(ELEINFO.Request[ELEINFO.Floor].Req!=-1)
                {
                    ELEINFO.Request[ELEINFO.Floor].outReq=no;
                    ELEINFO.Request[ELEINFO.Floor].Req=-1;
                }
            }
            else if(ELEINFO.Direction==down)
            {
                if(ELEINFO.Request[ELEINFO.Floor].Req==1&&ELEINFO.Request[ELEINFO.Floor].outReq==both)
                {
                    ELEINFO.Request[ELEINFO.Floor].outReq=up_req;
                }
                else if(ELEINFO.Request[ELEINFO.Floor].Req!=-1)
                {
                    ELEINFO.Request[ELEINFO.Floor].outReq=no;
                    ELEINFO.Request[ELEINFO.Floor].Req=-1;
                }
            }
            else
            {
                ELEINFO.Request[ELEINFO.Floor].outReq=no;
                ELEINFO.Request[ELEINFO.Floor].Req=-1;
            }
            break;
        case close:                                              //关门状态处理----------------------
            if(ELEINFO.Direction==up)
            {
                floorPtr=ELEINFO.Floor;
                while(floorPtr<MAX_FLOOR)
                {
                    if(ELEINFO.Request[floorPtr].Req!=-1&&ELEINFO.Request[floorPtr].outReq!=down_req)
                      break;
                    floorPtr++;
                }
                if(floorPtr<MAX_FLOOR)
                {
                        ELEINFO.Statu=run;
                }
                else
                {
                    floorPtr=0;
                    while(floorPtr<ELEINFO.Floor)
                    {
                        if(ELEINFO.Request[floorPtr].Req!=-1&&ELEINFO.Request[floorPtr].outReq!=up_req)
                            break;
                        floorPtr++;
                    }
                    if(floorPtr<ELEINFO.Floor)
                    {
                        ELEINFO.Statu=run;
                        ELEINFO.Direction=down;
                    }
                    else
                    {
                        ELEINFO.Statu=stop;
                        ELEINFO.Direction=still;
                    }
                }
            }
            else
            {
                floorPtr=0;
                while(floorPtr<ELEINFO.Floor)
                {
                    if(ELEINFO.Request[floorPtr].Req!=-1&&ELEINFO.Request[floorPtr].outReq!=up_req)
                        break;
                    floorPtr++;
                }
                if(floorPtr<ELEINFO.Floor)
                {
                    ELEINFO.Statu=run;
                }
                else
                {
                    floorPtr=ELEINFO.Floor;
                    while(floorPtr<MAX_FLOOR)
                    {
                        if(ELEINFO.Request[floorPtr].Req!=-1&&ELEINFO.Request[floorPtr].outReq!=down_req)
                            break;
                        floorPtr++;
                    }
                    if(floorPtr<MAX_FLOOR)
                    {
                        ELEINFO.Statu=run;
                        ELEINFO.Direction=up;
                    }
                    else
                    {
                        ELEINFO.Statu=stop;
                        ELEINFO.Direction=still;
                    }
                }
            }
            break;
        }
        //------------------------------------------
        printEle();                              //打印当前状态
        sleep(1);
        //------------------------------------------
    }
}

/**********************************************************/  //主函数 调用初始化,创建调度线程
int main (int argc, char const* argv[])
{
   pthread_t getkey_t,proc_t;           //线程ID
   initEle();                           //初始化
   pthread_create(&getkey_t,NULL,(void *)&getKey,NULL);  //创建键盘获取线程
   pthread_create(&proc_t,NULL,(void *)&procEle,NULL);   //创建电梯处理线程
   pthread_exit(NULL);                  //等待线程结束
   return 0;
}
/*************************************************************************************************/
