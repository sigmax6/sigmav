/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                             CORE FUNCTIONS
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File : OS_CORE.C
* By   : Jean J. Labrosse
* 翻译： likee
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE//防止一些文件加入到includes.h当中
#define  OS_GLOBALS
#include "includes.h"
#endif

/*
*********************************************************************************************************
*                              MAPPING TABLE TO MAP BIT POSITION TO BIT MASK
*
* Note: Index into table is desired bit position, 0..7
*       Indexed value corresponds to bit mask
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                映射位位置映射到位掩码的映射表  
备注：表的索引是想得到的位的位置（0到7）
                 索引得到的值是位的掩码
*********************************************************************************************************
*/


INT8U  const  OSMapTbl[]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/*
*********************************************************************************************************
*                                       PRIORITY RESOLUTION TABLE
*
* Note: Index into table is bit pattern to resolve highest priority
*       Indexed value corresponds to highest priority bit position (i.e. 0..7)
*********************************************************************************************************
*/
/*
**********************************************************************************************
                                       优先级分辨表
备注：索引表是最高优先级的位模式
                 索引值对应于最高优先级的位位置（0到7）
***********************************************************************************************
*/

INT8U  const  OSUnMapTbl[] = {
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x00 to 0x0F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x10 to 0x1F                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x20 to 0x2F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x30 to 0x3F                             */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x40 to 0x4F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x50 to 0x5F                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x60 to 0x6F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x70 to 0x7F                             */
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x80 to 0x8F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x90 to 0x9F                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xA0 to 0xAF                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xB0 to 0xBF                             */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xC0 to 0xCF                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xD0 to 0xDF                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xE0 to 0xEF                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0        /* 0xF0 to 0xFF                             */
};//256个东东，干嘛呢？

/*
*********************************************************************************************************
*                                       FUNCTION PROTOTYPES
*********************************************************************************************************
*/
//以下为初始化函数
static  void  OS_InitEventList(void);//初始化事件控制块的空表
static  void  OS_InitMisc(void);//初始化其它约定的值
static  void  OS_InitRdyList(void);//初始化就绪列表
static  void  OS_InitTaskIdle(void);//建立空闲任务
static  void  OS_InitTaskStat(void);//建立一个统计任务
static  void  OS_InitTCBList(void);//初始化任务控制块的空闲表

/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*
* Description: This function is used to initialize the internals of uC/OS-II and MUST be called prior to
*              creating any uC/OS-II object and, prior to calling OSStart().
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                             初始化
 功能：要先在调用  OSStart()后，再初始化uxosII内核，要先建立ucos任何一个任务                                                        
*********************************************************************************************************
*/



void  OSInit (void)
{
#if OS_VERSION >= 204
    OSInitHookBegin();                                           /* Call port specific initialization code   */
//调用特定通信口初始化代码
#endif

    OS_InitMisc();                                               /* Initialize miscellaneous variables       */
//初始化多种变量
    OS_InitRdyList();                                            /* Initialize the Ready List                */
//初始化就绪链表
    OS_InitTCBList();                                            /* Initialize the free list of OS_TCBs      */
//初始化TCB空闲链表
    OS_InitEventList();                                          /* Initialize the free list of OS_EVENTs    */
//初始化事件空闲链表

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
    OS_FlagInit();                                               /* Initialize the event flag structures     */
//初始化事件标志结构
#endif

#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)
    OS_MemInit();                                                /* Initialize the memory manager            */
//初始化存在管理
#endif

#if (OS_Q_EN > 0) && (OS_MAX_QS > 0)
    OS_QInit();                                                  /* Initialize the message queue structures  */
//初始化消息队列结构
#endif

    OS_InitTaskIdle();                                           /* Create the Idle Task                     */
//建立空闲任务
#if OS_TASK_STAT_EN > 0
    OS_InitTaskStat();                                           /* Create the Statistic Task                */
//建立统计任务
#endif

#if OS_VERSION >= 204
    OSInitHookEnd();                                             /* Call port specific init. code            */
//调用专用通信口初始化代码
#endif
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                              ENTER ISR
*
* Description: This function is used to notify uC/OS-II that you are about to service an interrupt
*              service routine (ISR).  This allows uC/OS-II to keep track of interrupt nesting and thus
*              only perform rescheduling at the last nested ISR.
*
* Arguments  : none
*
* Returns    : none
*
* Notes      : 1) This function should be called ith interrupts already disabled
*              2) Your ISR can directly increment OSIntNesting without calling this function because
*                 OSIntNesting has been declared 'global'.  
*              3) You MUST still call OSIntExit() even though you increment OSIntNesting directly.
*              4) You MUST invoke OSIntEnter() and OSIntExit() in pair.  In other words, for every call
*                 to OSIntEnter() at the beginning of the ISR you MUST have a call to OSIntExit() at the
*                 end of the ISR.
*              5) You are allowed to nest interrupts up to 255 levels deep.
*              6) I removed the OS_ENTER_CRITICAL() and OS_EXIT_CRITICAL() around the increment because
*                 OSIntEnter() is always called with interrupts disabled.
                                             进入中断服务程序
描述：通知ucosII将进入中断服务程序，它允许ucos跟踪中断嵌套，所以只有
                 //在最后一层中断嵌套才重新安排。
参数：无
返回：无
注意：1、只有在关中断的时候才调用
                 2、因为中断嵌套定义为全局变量，所以在没有调用此函数的时候
                         也可以增加中断嵌套数
                 3、即使你增加了中断嵌套数，你也要调用OSIntExit()
                 4、你必须成对调用OSIntEnter() and OSIntExit()，即：每次在ISR开始时调用OSIntEnter() 
                        你将在ISR结束时调用OSIntExit()
                 5、你可以嵌套255层深
                 6、因为每次调用时中断是关的，所以我去掉了OS_ENTER_CRITICAL() and OS_EXIT_CRITICAL()
*********************************************************************************************************
*/

void  OSIntEnter (void)
{
    if (OSRunning == TRUE) {
        if (OSIntNesting < 255) {
            OSIntNesting++;                      /* Increment ISR nesting level                        */
        }//增加中断嵌套数
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                               EXIT ISR
*
* Description: This function is used to notify uC/OS-II that you have completed serviving an ISR.  When
*              the last nested ISR has completed, uC/OS-II will call the scheduler to determine whether
*              a new, high-priority task, is ready to run.
*
* Arguments  : none
*
* Returns    : none
*
* Notes      : 1) You MUST invoke OSIntEnter() and OSIntExit() in pair.  In other words, for every call
*                 to OSIntEnter() at the beginning of the ISR you MUST have a call to OSIntExit() at the
*                 end of the ISR.
*                 2) Rescheduling is prevented when the scheduler is locked (see OS_SchedLock())
                                       退出中断
描述：通知ucos你已经完成中断服务程序，当最后一层嵌套完成后，ucos将
                 重新调度看是否一个新的，高优先级任务将就绪。
参数：无
返回：无
备注：1、你必须成对调用OSIntEnter() and OSIntExit()，即：每次在ISR开始时调用OSIntEnter() 
                        你将在ISR结束时调用OSIntExit()
                 2、当调度上锁的话，重新调度是不允许的。见OS_SchedLock()(就在下面)
*********************************************************************************************************
*/

void  OSIntExit (void)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    
    
    if (OSRunning == TRUE) {//如果正在运行
        OS_ENTER_CRITICAL();
        if (OSIntNesting > 0) {                            /* Prevent OSIntNesting from wrapping       */
			//防止减到比0还小
            OSIntNesting--;
        }
        if ((OSIntNesting == 0) && (OSLockNesting == 0)) { /* Reschedule only if all ISRs complete ... */
            OSIntExitY    = OSUnMapTbl[OSRdyGrp];          /* ... and not locked.                      */
            OSPrioHighRdy = (INT8U)((OSIntExitY << 3) + OSUnMapTbl[OSRdyTbl[OSIntExitY]]);
			//看到几遍了，就是不会算啊！哪交要算算
			//终于明白了！！！！！！！！！！！！！！
            if (OSPrioHighRdy != OSPrioCur) {              /* No Ctx Sw if current task is highest rdy */
                OSTCBHighRdy  = OSTCBPrioTbl[OSPrioHighRdy];
				//如果当前任务不是最高优先级，将最高优先级放入最高优先级标志中。
                OSCtxSwCtr++;                              /* Keep track of the number of ctx switches */
				//跟踪任务转换次数
                OSIntCtxSw();                              /* Perform interrupt level ctx switch       */
				//执行中断水平任务切换？？？
            }
        }
        OS_EXIT_CRITICAL();
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                          PREVENT SCHEDULING
*
* Description: This function is used to prevent rescheduling to take place.  This allows your application
*              to prevent context switches until you are ready to permit context switching.
*
* Arguments  : none
*
* Returns    : none
*
* Notes      : 1) You MUST invoke OSSchedLock() and OSSchedUnlock() in pair.  In other words, for every
*                 call to OSSchedLock() you MUST have a call to OSSchedUnlock().
                                                    调度上锁函数
描述：阻止再次调度发生，它让你准备执行任务切换的时候才进行任务切换
//参数：无
返回：无
备注：1、必须调用OSSchedLock() and OSSchedUnlock()成对
*********************************************************************************************************
*/

#if OS_SCHED_LOCK_EN > 0
void  OSSchedLock (void)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    if (OSRunning == TRUE) {                     /* Make sure multitasking is running                  */
//保证多任务在运行
        OS_ENTER_CRITICAL();
        if (OSLockNesting < 255) {               /* Prevent OSLockNesting from wrapping back to 0      */
			//防止嵌套数返回到0
            OSLockNesting++;                     /* Increment lock nesting level                       */
        }
        OS_EXIT_CRITICAL();
    }
}
#endif    

/*$PAGE*/
/*
*********************************************************************************************************
*                                          ENABLE SCHEDULING
*
* Description: This function is used to re-allow rescheduling.
*
* Arguments  : none
*
* Returns    : none
*
* Notes      : 1) You MUST invoke OSSchedLock() and OSSchedUnlock() in pair.  In other words, for every
*                 call to OSSchedLock() you MUST have a call to OSSchedUnlock().
                                                  能使任务调度
描述：用于再次允许任务调度
参数：无
返回：无
备注：1、必须将OSSchedLock() and OSSchedUnlock()成对调用
*********************************************************************************************************
*/

#if OS_SCHED_LOCK_EN > 0//能使包含代码OSSchedLock() and OSSchedUnlock()
void  OSSchedUnlock (void)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    if (OSRunning == TRUE) {                                   /* Make sure multitasking is running    */
		//保证多任务运行
        OS_ENTER_CRITICAL();
        if (OSLockNesting > 0) {                               /* Do not decrement if already 0        */
			//如果是零的话就不要再减了
            OSLockNesting--;                                   /* Decrement lock nesting level         */
            if ((OSLockNesting == 0) && (OSIntNesting == 0)) { /* See if sched. enabled and not an ISR */
				//如果解锁，且中断嵌套数为零，由需要任务调度，看高优先
				//级任务是否就绪
                OS_EXIT_CRITICAL();
                OS_Sched();                                    /* See if a HPT is ready                */
            } else {
                OS_EXIT_CRITICAL();//否则就不要任务调度了
            }
        } else {
            OS_EXIT_CRITICAL();//如果锁定的话，就开中断了。
        }
    }
}
#endif    

/*$PAGE*/
/*
*********************************************************************************************************
*                                          START MULTITASKING
*
* Description: This function is used to start the multitasking process which lets uC/OS-II manages the
*              task that you have created.  Before you can call OSStart(), you MUST have called OSInit()
*              and you MUST have created at least one task.
*
* Arguments  : none
*
* Returns    : none
*
* Note       : OSStartHighRdy() MUST:
*                 a) Call OSTaskSwHook() then,
*                 b) Set OSRunning to TRUE.
*                 c) Load the context of the task pointed to by OSTCBHighRdy.
*                 d_ Execute the task.
                                                  开始多任务处理
描述：开始多任务处理，使ucos管理你建立的任务，在调用OSStart()前，你必须
                 先调用OSInit()，且至少建立了一个任务
参数：无
返回；无
备注：OSStartHighRdy()必须
                 1、先调用Call OSTaskSwHook()
                 2、再设置OSRunning为真
                 3、装载指向OSTCBHighRdy的内容的指针
                 4、执行任务
*********************************************************************************************************
*/

void  OSStart (void)
{
    INT8U y;
    INT8U x;


    if (OSRunning == FALSE) {//如果没有运行
        y             = OSUnMapTbl[OSRdyGrp];        /* Find highest priority's task priority number   */
        x             = OSUnMapTbl[OSRdyTbl[y]];//又遇到你们，真是FUCK。
        OSPrioHighRdy = (INT8U)((y << 3) + x);
        OSPrioCur     = OSPrioHighRdy;//高优先级的任务作为当前任务
        OSTCBHighRdy  = OSTCBPrioTbl[OSPrioHighRdy]; /* Point to highest priority task ready to run    */
		//下一个任务作为即将运行的任务
        OSTCBCur      = OSTCBHighRdy;
        OSStartHighRdy();                            /* Execute target specific code to start task     */
		//执行特定代码去开始任务
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                        STATISTICS INITIALIZATION
*
* Description: This function is called by your application to establish CPU usage by first determining
*              how high a 32-bit counter would count to in 1 second if no other tasks were to execute
*              during that time.  CPU usage is then determined by a low priority task which keeps track
*              of this 32-bit counter every second but this time, with other tasks running.  CPU usage is
*              determined by:
*
*                                                            OSIdleCtr
*                 CPU Usage (%) = 100 * (1 - ————)
*                                                          OSIdleCtrMax
*
* Arguments  : none
*
* Returns    : none
                                                 统计任务初始化
描述：假如没有其它任务在这个时候运行，以在一秒内一个32位计数器能
                 计到多少数来建立CPU使用率CPU使用率由一个每秒跟踪32位计数器的低优先
                 级任务决定，但如果其它任务运行，CPU使用率由下面函数决定：
  *                                                            OSIdleCtr
*                 CPU Usage (%) = 100 * (1 - ————)
*                                                          OSIdleCtrMax      
参数：无
返回：无
*********************************************************************************************************
*/

#if OS_TASK_STAT_EN > 0
void  OSStatInit (void)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    OSTimeDly(2);                                /* Synchronize with clock tick                        */
//与时钟节拍同步
    OS_ENTER_CRITICAL();
    OSIdleCtr    = 0L;                           /* Clear idle counter                                 */
	//清除空闲计算器 
    OS_EXIT_CRITICAL();
    OSTimeDly(OS_TICKS_PER_SEC);                 /* Determine MAX. idle counter value for 1 second     */
    //计算一秒内空闲计数器能计多少
    OS_ENTER_CRITICAL();
    OSIdleCtrMax = OSIdleCtr;                    /* Store maximum idle counter count in 1 second       */
	//保存计数值
    OSStatRdy    = TRUE;//统计任务就绪
    OS_EXIT_CRITICAL();
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                         PROCESS SYSTEM TICK
*
* Description: This function is used to signal to uC/OS-II the occurrence of a 'system tick' (also known
*              as a 'clock tick').  This function should be called by the ticker ISR but, can also be
*              called by a high priority task.
*
* Arguments  : none
*
* Returns    : none
                                               建立系统时钟
描述：这个函数向ucos发信号产生时钟节拍，它能被ISR节拍调用，也可以由
                 高优先级任务调用
参数：无
返回：无
*********************************************************************************************************
*/

void  OSTimeTick (void)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    OS_TCB    *ptcb;


    OSTimeTickHook();                                      /* Call user definable hook                 */
	//用户定义的时钟节拍外连函数，，可将时钟节拍函数OSTimeTick扩展，调用此
	//函数是打算在中断一开始给用户一个可以做点什么的机会，
#if OS_TIME_GET_SET_EN > 0   //能使包含代码OSTimeGet() and OSTimeSet()
    OS_ENTER_CRITICAL();                                   /* Update the 32-bit tick counter           */
    OSTime++;//系统节拍现阶段值，计算自系统上电以来的时钟节拍数
    OS_EXIT_CRITICAL();
#endif
    if (OSRunning == TRUE) {    
        ptcb = OSTCBList;                                  /* Point at first TCB in TCB list           */
		//指向PCB双向链表中的第一个
        while (ptcb->OSTCBPrio != OS_IDLE_PRIO) {          /* Go through all TCBs in TCB list          */
			//将PCB链表中的TCB遍历一遍，一直做到空闲任务
            OS_ENTER_CRITICAL();
            if (ptcb->OSTCBDly != 0) {                     /* Delayed or waiting for event with TO     */
                if (--ptcb->OSTCBDly == 0) {               /* Decrement nbr of ticks to end of delay   */
                    if ((ptcb->OSTCBStat & OS_STAT_SUSPEND) == OS_STAT_RDY) { /* Is task suspended?    */
						//确实被挂起的任务不会进入就绪态
                        OSRdyGrp               |= ptcb->OSTCBBitY; /* No,  Make task R-to-R (timed out)*/
			//如果某任务的TCB中的时间延时项OSTCBDly减为0时，这个任务就进入了就绪态
                        OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;
                    } else {                               /* Yes, Leave 1 tick to prevent ...         */
   //保留一个节拍防止去除挂起的时候任务不稳定
                        ptcb->OSTCBDly = 1;                /* ... loosing the task when the ...        */
                    }                                      /* ... suspension is removed.               */
                }
            }
            ptcb = ptcb->OSTCBNext;                        /* Point at next TCB in TCB list            */
			//指向TCB链表的下一块
            OS_EXIT_CRITICAL();
        }
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                             GET VERSION
*
* Description: This function is used to return the version number of uC/OS-II.  The returned value
*              corresponds to uC/OS-II's version number multiplied by 100.  In other words, version 2.00
*              would be returned as 200.
*
* Arguments  : none
*
* Returns    : the version number of uC/OS-II multiplied by 100.
                                                  版本号
描述：返回UCOS版本号号，返回值为UCOS版本*100，换句话说，2.00版本将
                 返回200
 参数：无
 返回：版本号*100
*********************************************************************************************************
*/

INT16U  OSVersion (void)
{
    return (OS_VERSION);
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                            DUMMY FUNCTION
*
* Description: This function doesn't do anything.  It is called by OSTaskDel().
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                         虚拟函数？
描述：此函数不做任务事情，由OSTaskDel().调用
参数：无
返回：无
*********************************************************************************************************
*/


#if OS_TASK_DEL_EN > 0
void  OS_Dummy (void)
{           //不做任何事
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                             MAKE TASK READY TO RUN BASED ON EVENT OCCURING
*
* Description: This function is called by other uC/OS-II services and is used to ready a task that was
*              waiting for an event to occur.
*
* Arguments  : pevent    is a pointer to the event control block corresponding to the event.
*
*              msg       is a pointer to a message.  This pointer is used by message oriented services
*                        such as MAILBOXEs and QUEUEs.  The pointer is not used when called by other
*                        service functions.
*
*              msk       is a mask that is used to clear the status byte of the TCB.  For example,
*                        OSSemPost() will pass OS_STAT_SEM, OSMboxPost() will pass OS_STAT_MBOX etc.
*
* Returns    : none
*
* Note       : This function is INTERNAL to uC/OS-II and your application should not call it.
                                                     基于事件发生使任务准备运行
描述：该函数由UCOS其它服务调用，用于使一个任务就绪，等待一个事件发生
参数：pevent：指向对应事件的ECB的指针
                 msg：消息指针，由邮箱队列等消息定向服务使用，该指针不能由其它
                              服务函数调用
                msk：是用于清除TCB状态字节的掩码，比如：
                             OSSemPost() 将传递TAT_SEM, OSMboxPost() 将传递OS_STAT_MBOX

这个函数看得不是很懂
*********************************************************************************************************
*/
#if OS_EVENT_EN > 0  
//能使队列代码产生&&申请队列控制块最大数不为零||能使邮箱代码产生||
//能使信号量代码产生||能使互斥量代码产生 

INT8U  OS_EventTaskRdy (OS_EVENT *pevent, void *msg, INT8U msk)
{
    OS_TCB *ptcb;
    INT8U   x;
    INT8U   y;
    INT8U   bitx;
    INT8U   bity;
    INT8U   prio;


    y    = OSUnMapTbl[pevent->OSEventGrp];            /* Find highest prio. task waiting for message   */
    bity = OSMapTbl[y];//寻找等待消息的最高优先级任务
    x    = OSUnMapTbl[pevent->OSEventTbl[y]];
    bitx = OSMapTbl[x];
    prio = (INT8U)((y << 3) + x);                     /* Find priority of task getting the msg         */
    if ((pevent->OSEventTbl[y] &= ~bitx) == 0x00) {   /* Remove this task from the waiting list        */
		//将此任务从等待列表中移除。
        pevent->OSEventGrp &= ~bity;                  /* Clr group bit if this was only task pending   */
		//如果是唯一挂起的任务，那么清除组位，
    }
    ptcb                 =  OSTCBPrioTbl[prio];       /* Point to this task's OS_TCB                   */
	//指针指向当前任务的OS_TCB 
    ptcb->OSTCBDly       =  0;                        /* Prevent OSTimeTick() from readying task       */
	//防止时钟节拍使任务就绪？这样就行？
    ptcb->OSTCBEventPtr  = (OS_EVENT *)0;             /* Unlink ECB from this task                     */
	//从这样任务上断开ECB
#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0)
    ptcb->OSTCBMsg       = msg;                       /* Send message directly to waiting task         */
//将消息直接发送到等待的任务
#else
    msg                  = msg;                       /* Prevent compiler warning if not used          */
//防止编译器警告
#endif
    ptcb->OSTCBStat     &= ~msk;                      /* Clear bit associated with event type          */
//事件状态位清除
    if (ptcb->OSTCBStat == OS_STAT_RDY) {             /* See if task is ready (could be susp'd)        */
		//如果任务就绪，（不能被挂起）
        OSRdyGrp        |=  bity;                     /* Put task in the ready to run list             */
        OSRdyTbl[y]     |=  bitx;
    }
    return (prio);
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                   MAKE TASK WAIT FOR EVENT TO OCCUR
*
* Description: This function is called by other uC/OS-II services to suspend a task because an event has
*              not occurred.
*
* Arguments  : pevent   is a pointer to the event control block for which the task will be waiting for.
*
* Returns    : none
*
* Note       : This function is INTERNAL to uC/OS-II and your application should not call it.
                                  使任务等待事件发生
描述：由ucosII服务程序调用去挂起一个任务因为一个事件还没有发生
参数：pevent  指向将要等待的任务的ECB的指针
返回：无
备注：ucos内部调用，其它应用程序不能调用它
*********************************************************************************************************
*/

#if OS_EVENT_EN > 0
#define  OS_EVENT_EN       (((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0))
//能使队列代码产生&&申请队列控制块最大数不为零||能使邮箱代码产生||
//能使信号量代码产生||能使互斥量代码产生 

void  OS_EventTaskWait (OS_EVENT *pevent)
{
    OSTCBCur->OSTCBEventPtr = pevent;            /* Store pointer to event control block in TCB        */
//保存ECB指针到TCB
    if ((OSRdyTbl[OSTCBCur->OSTCBY] &= ~OSTCBCur->OSTCBBitX) == 0x00) {   /* Task no longer ready      */
		//如果任务没有就绪（那就挂起吗？）
        OSRdyGrp &= ~OSTCBCur->OSTCBBitY;        /* Clear event grp bit if this was only task pending  */
		//如果是没有一个任务挂起的话就清除事件群位。
    }
    pevent->OSEventTbl[OSTCBCur->OSTCBY] |= OSTCBCur->OSTCBBitX;          /* Put task in waiting list  */
    pevent->OSEventGrp                   |= OSTCBCur->OSTCBBitY;
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                              MAKE TASK READY TO RUN BASED ON EVENT TIMEOUT
*
* Description: This function is called by other uC/OS-II services to make a task ready to run because a
*              timeout occurred.
*
* Arguments  : pevent   is a pointer to the event control block which is readying a task.
*
* Returns    : none
*
* Note       : This function is INTERNAL to uC/OS-II and your application should not call it.
                                                             当事件超时，让任务准备运行
描述：由于超时发生，由ucos调用让个任务准备运行，
参数：pevent：就绪任务的ECB指针
返回：无
备注：ucos内部函数，其它应用函数不能调用。
*********************************************************************************************************
*/
#if OS_EVENT_EN > 0
void  OS_EventTO (OS_EVENT *pevent)
{
    if ((pevent->OSEventTbl[OSTCBCur->OSTCBY] &= ~OSTCBCur->OSTCBBitX) == 0x00) {
        pevent->OSEventGrp &= ~OSTCBCur->OSTCBBitY;
    }//如果一组中没有一个任务挂起就清除该组
    OSTCBCur->OSTCBStat     = OS_STAT_RDY;       /* Set status to ready         */
	//将当前任务设置为即将运行
    OSTCBCur->OSTCBEventPtr = (OS_EVENT *)0;     /* No longer waiting for event   */
	//运行就不再等待事物了
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                 INITIALIZE EVENT CONTROL BLOCK'S WAIT LIST
*
* Description: This function is called by other uC/OS-II services to initialize the event wait list.
*
* Arguments  : pevent    is a pointer to the event control block allocated to the event.
*
* Returns    : none
*
* Note       : This function is INTERNAL to uC/OS-II and your application should not call it.
                                                        初始化ECB等待列表
描述：由ucos调用初始化事件等待列表
参数：pevent：指向指定事件的ECB的指针
返回：无
备注：内部函数，应用函数不能调用
*********************************************************************************************************
*/
#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0)
void  OS_EventWaitListInit (OS_EVENT *pevent)
{
    INT8U  *ptbl;


    pevent->OSEventGrp = 0x00;                   /* No task waiting on event     */
	//初始化时事件中无等待的任务
    ptbl               = &pevent->OSEventTbl[0];//取地址

#if OS_EVENT_TBL_SIZE > 0//不采用循环的原因是这样运行速度更快
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 1
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 2
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 3
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 4
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 5
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 6
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 7
    *ptbl              = 0x00;
#endif
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                           INITIALIZE THE FREE LIST OF EVENT CONTROL BLOCKS
*
* Description: This function is called by OSInit() to initialize the free list of event control blocks.
*
* Arguments  : none
*
* Returns    : none
描述：初始化事件控制块空闲列表
参数：无
返回：无
*********************************************************************************************************
*/

static  void  OS_InitEventList (void)
{
#if (OS_EVENT_EN > 0) && (OS_MAX_EVENTS > 0)
#if (OS_MAX_EVENTS > 1)//如果任务数大于一
    INT16U     i;
    OS_EVENT  *pevent1;
    OS_EVENT  *pevent2;


    pevent1 = &OSEventTbl[0];//事件控制块方框表
    pevent2 = &OSEventTbl[1];
    for (i = 0; i < (OS_MAX_EVENTS - 1); i++) {                  /* Init. list of free EVENT control blocks  */
		//初始化空ECB链表
        pevent1->OSEventType = OS_EVENT_TYPE_UNUSED;//状态设置为挂起
        pevent1->OSEventPtr  = pevent2;//把它们链起来。接起来
        pevent1++;
        pevent2++;//加吧，加吧，加到OS_MAX_EVENTS - 1
    }
    pevent1->OSEventType = OS_EVENT_TYPE_UNUSED;
    pevent1->OSEventPtr  = (OS_EVENT *)0;//前趋指向零指针
    OSEventFreeList      = &OSEventTbl[0];//空表指向OSEventTbl第一个
#else//如果只有一个事件控制块
    OSEventFreeList              = &OSEventTbl[0];    /* Only have ONE event control block        */
    OSEventFreeList->OSEventType = OS_EVENT_TYPE_UNUSED;
    OSEventFreeList->OSEventPtr  = (OS_EVENT *)0;//前趋指向NULL
#endif
#endif
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                                    INITIALIZE MISCELLANEOUS VARIABLES
*
* Description: This function is called by OSInit() to initialize miscellaneous variables.
*
* Arguments  : none
*
* Returns    : none
描述：由OSInit()调用去初始化各种变量
参数：无
返回：无
*********************************************************************************************************
*/

static  void  OS_InitMisc (void)
{
#if OS_TIME_GET_SET_EN > 0   
    OSTime        = 0L;                                          /* Clear the 32-bit system clock            */
#endif//清除32位系统时钟

    OSIntNesting  = 0;   /* Clear the interrupt nesting counter      *///中断嵌套初始化为0
    OSLockNesting = 0;  /* Clear the scheduling lock counter        */
	//清除调度锁定计数器

    OSTaskCtr     = 0;                                           /* Clear the number of tasks                */
       //初始化任务计数器
    OSRunning     = FALSE;                                       /* Indicate that multitasking not started   */
    //表示多任务处理还没有开始
    OSCtxSwCtr    = 0;                                           /* Clear the context switch counter         */
	//任务转换计数器为0
    OSIdleCtr     = 0L;                                          /* Clear the 32-bit idle counter            */
    //清除32位空闲计数器
#if (OS_TASK_STAT_EN > 0) && (OS_TASK_CREATE_EXT_EN > 0)
    OSIdleCtrRun  = 0L;
    OSIdleCtrMax  = 0L;
    OSStatRdy     = FALSE;                                       /* Statistic task is not ready              */
	//统计任务还没有准备好呢！
#endif
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                                       INITIALIZE THE READY LIST
*
* Description: This function is called by OSInit() to initialize the Ready List.
*
* Arguments  : none
*
* Returns    : none
                                                 初始化就绪列表
描述：由OSInit()调用，初始化就绪列表
参数：无
返回：无
*********************************************************************************************************
*/

static  void  OS_InitRdyList (void)
{
    INT16U   i;
    INT8U   *prdytbl;


    OSRdyGrp      = 0x00;                    /* Clear the ready list                     */
	//等待列表清零
    prdytbl       = &OSRdyTbl[0];//得到就绪任务方框表首地址
    for (i = 0; i < OS_RDY_TBL_SIZE; i++) {
        *prdytbl++ = 0x00;//每一组都清零
    }

    OSPrioCur     = 0;//当前任务优先级
    OSPrioHighRdy = 0;//最高优先级任务的优先级

    OSTCBHighRdy  = (OS_TCB *)0; //指向最高优先级TCB指针                                
    OSTCBCur      = (OS_TCB *)0;//当前运行任务TCB指针
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                                         CREATING THE IDLE TASK
*
* Description: This function creates the Idle Task.
*
* Arguments  : none
*
* Returns    : none
                                                 建立空闲任务
描述：这个函数建立空闲任务
参数：无
返回：无
*********************************************************************************************************
*/

static  void  OS_InitTaskIdle (void)
{
#if OS_TASK_CREATE_EXT_EN > 0//包含创建任务代码OS_TASK_CREATE_EXT_EN 
    #if OS_STK_GROWTH == 1//堆栈从高到低
    (void)OSTaskCreateExt(OS_TaskIdle,//建立空闲任务
                          (void *)0,                                 /* No arguments passed to OS_TaskIdle() */
                          //pdata为零，没有参数传递表示没有参数传给OS_TaskIdle()
                          &OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1], /* Set Top-Of-Stack                     */
                          //设置栈顶地址
                          OS_IDLE_PRIO,                              /* Lowest priority level                */
                          //优先级
                          OS_TASK_IDLE_ID,//任务ID
                          &OSTaskIdleStk[0],                         /* Set Bottom-Of-Stack                  */
                          //设置栈底
                          OS_TASK_IDLE_STK_SIZE,//堆栈大小，即进口宽度
                          (void *)0,                                 /* No TCB extension                     */
                          //无TCB扩展
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);/* Enable stack checking + clear stack  */
	                   //允许堆栈检测/清除堆栈
    #else//堆栈从低到高
    (void)OSTaskCreateExt(OS_TaskIdle,
                          (void *)0,                                 /* No arguments passed to OS_TaskIdle() */
                          &OSTaskIdleStk[0],   /* Set Top-Of-Stack                     */
                          //与上面相反
                          OS_IDLE_PRIO,                              /* Lowest priority level                */
                          OS_TASK_IDLE_ID,
                          &OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1], /* Set Bottom-Of-Stack        */
                          //与上面相反
                          OS_TASK_IDLE_STK_SIZE,
                          (void *)0,                                 /* No TCB extension                     */
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);/* Enable stack checking + clear stack  */
    #endif
#else//不包含创建任务代码OS_TASK_CREATE_EXT_EN 
    #if OS_STK_GROWTH == 1//堆栈从高到低
    (void)OSTaskCreate(OS_TaskIdle,
                       (void *)0,
                       &OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1],
                       OS_IDLE_PRIO);
    #else//堆栈从低到高
    (void)OSTaskCreate(OS_TaskIdle,
                       (void *)0,
                       &OSTaskIdleStk[0],
                       OS_IDLE_PRIO);
    #endif
#endif
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                                      CREATING THE STATISTIC TASK
*
* Description: This function creates the Statistic Task.
*
* Arguments  : none
*
* Returns    : none
                                               建立统计任务
描述：建立统计任务
参数：无
返回：无
*********************************************************************************************************
*/
//这个函数同上面一样差不多
#if OS_TASK_STAT_EN > 0
static  void  OS_InitTaskStat (void)
{
#if OS_TASK_CREATE_EXT_EN > 0
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreateExt(OS_TaskStat,
                          (void *)0,                                   /* No args passed to OS_TaskStat()*/
                          &OSTaskStatStk[OS_TASK_STAT_STK_SIZE - 1],   /* Set Top-Of-Stack               */
                          OS_STAT_PRIO,                                /* One higher than the idle task  */
                          //比空闲任务优先级高一
                          OS_TASK_STAT_ID,
                          &OSTaskStatStk[0],                           /* Set Bottom-Of-Stack            */
                          OS_TASK_STAT_STK_SIZE,
                          (void *)0,                                   /* No TCB extension               */
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);  /* Enable stack checking + clear  */
    #else
    (void)OSTaskCreateExt(OS_TaskStat,
                          (void *)0,                                   /* No args passed to OS_TaskStat()*/
                          &OSTaskStatStk[0],                           /* Set Top-Of-Stack               */
                          OS_STAT_PRIO,                                /* One higher than the idle task  */
                          OS_TASK_STAT_ID,
                          &OSTaskStatStk[OS_TASK_STAT_STK_SIZE - 1],   /* Set Bottom-Of-Stack            */
                          OS_TASK_STAT_STK_SIZE,
                          (void *)0,                                   /* No TCB extension               */
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);  /* Enable stack checking + clear  */
    #endif
#else
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreate(OS_TaskStat,
                       (void *)0,                                      /* No args passed to OS_TaskStat()*/
                       &OSTaskStatStk[OS_TASK_STAT_STK_SIZE - 1],      /* Set Top-Of-Stack               */
                       OS_STAT_PRIO);                                  /* One higher than the idle task  */
    #else
    (void)OSTaskCreate(OS_TaskStat,
                       (void *)0,                                      /* No args passed to OS_TaskStat()*/
                       &OSTaskStatStk[0],                              /* Set Top-Of-Stack               */
                       OS_STAT_PRIO);                                  /* One higher than the idle task  */
    #endif
#endif
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                            INITIALIZE THE FREE LIST OF TASK CONTROL BLOCKS
*
* Description: This function is called by OSInit() to initialize the free list of OS_TCBs.
*
* Arguments  : none
*
* Returns    : none
                                                 初始化TCB链表
描述：由OSInit()调用，初始化OS_TCBs空闲链表
*********************************************************************************************************
*/

static  void  OS_InitTCBList (void)
{
    INT8U    i;
    OS_TCB  *ptcb1;
    OS_TCB  *ptcb2;


    OSTCBList     = (OS_TCB *)0;                                 /* TCB Initialization                       */
	//初始化双向TCB链表
    for (i = 0; i < (OS_LOWEST_PRIO + 1); i++) {                 /* Clear the priority table    */
        OSTCBPrioTbl[i] = (OS_TCB *)0;
    }//清除优先级方框表
    ptcb1 = &OSTCBTbl[0];
    ptcb2 = &OSTCBTbl[1];//取地址
    for (i = 0; i < (OS_MAX_TASKS + OS_N_SYS_TASKS - 1); i++) {  /* Init. list of free TCBs                  */
        ptcb1->OSTCBNext = ptcb2;
        ptcb1++;
        ptcb2++;//把它们链在一起
    }
    ptcb1->OSTCBNext = (OS_TCB *)0;  /* Last OS_TCB    *///最后一个指向空指针
    OSTCBFreeList    = &OSTCBTbl[0];//空表指向第一个地址
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                              SCHEDULER
*
* Description: This function is called by other uC/OS-II services to determine whether a new, high
*              priority task has been made ready to run.  This function is invoked by TASK level code
*              and is not used to reschedule tasks from ISRs (see OSIntExit() for ISR rescheduling).
*
* Arguments  : none
*
* Returns    : none
*
* Notes      : 1) This function is INTERNAL to uC/OS-II and your application should not call it.
*              2) Rescheduling is prevented when the scheduler is locked (see OS_SchedLock())
*********************************************************************************************************
*/
/*
************************************************************************************************
                                                                   调度程序
描述：这个函数由其它ucosII版本服务调用去检查是否一个新的，高优先级的任务将准备运行。
                 这个功能由任务等级代码唤醒，它不用于从中断服务程序中重新安排代码。
参数：无
返回：无
备注：这个函数是ucosII内部的，在应用程序中不能调用
                 当调度程序锁定的话，重新调度将被禁止。（见OS_SchedLock()）

**************************************************************************************************
*/

void  OS_Sched (void)
{
#if OS_CRITICAL_METHOD == 3                            /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr;//为CPU状态寄存器分配存储空间
#endif    
    INT8U      y;


    OS_ENTER_CRITICAL();//进入临界状态
    if ((OSIntNesting == 0) && (OSLockNesting == 0)) { /* Sched. only if all ISRs done & not locked    */
		//如果中断嵌套层为零，多任务处理锁定嵌套层为零
		//即只有在所以ISR完成，且没有锁定的请况下调度
        y             = OSUnMapTbl[OSRdyGrp];          /* Get pointer to HPT ready to run              */
		//得到高优先级就绪态的任务指针
        OSPrioHighRdy = (INT8U)((y << 3) + OSUnMapTbl[OSRdyTbl[y]]);
		//最高优先级任务的优先数算法，有时间要研究一下，现在看不懂
        if (OSPrioHighRdy != OSPrioCur) {              /* No Ctx Sw if current task is highest rdy     */
			//如果最高优先级任务不是当前任务
            OSTCBHighRdy = OSTCBPrioTbl[OSPrioHighRdy];//将最高优先级任务调到即将运行指针
            OSCtxSwCtr++;                              /* Increment context switch counter             */
			//上下文转换数加一
            OS_TASK_SW();                              /* Perform a context switch                     */
            //运行上下文转换
        }
    }
    OS_EXIT_CRITICAL();//退出临界状态
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                              IDLE TASK
*
* Description: This task is internal to uC/OS-II and executes whenever no other higher priority tasks
*              executes because they are ALL waiting for event(s) to occur.
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : 1) OSTaskIdleHook() is called after the critical section to ensure that interrupts will be
*                 enabled for at least a few instructions.  On some processors (ex. Philips XA), enabling
*                 and then disabling interrupts didn't allow the processor enough time to have interrupts
*                 enabled before they were disabled again.  uC/OS-II would thus never recognize
*                 interrupts.
*              2) This hook has been added to allow you to do such things as STOP the CPU to conserve 
*                 power.
                                           空闲任务：
描述：这个任务是ucos内部任务，由于其它任务都在等事件发生，
                没有高优先级任务运行的时候它就运行
参数：无
返回：无
备注：1、出临界后要调用OSTaskIdleHook()保证中断真正开启。
                2、这个能加扩展允许我们做一些事情，如：为了节能，让CPU
                停止工作
*********************************************************************************************************
*/

void  OS_TaskIdle (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    pdata = pdata;                               /* Prevent compiler warning for not using 'pdata'     */
    for (;;) {
        OS_ENTER_CRITICAL();
        OSIdleCtr++;//加一前后中断先关后开，是因为8位或者十六位处理器加一
        //需要多条指令，防止中断打入。
        OS_EXIT_CRITICAL();
        OSTaskIdleHook();                        /* Call user definable HOOK                           */
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                            STATISTICS TASK
*
* Description: This task is internal to uC/OS-II and is used to compute some statistics about the
*              multitasking environment.  Specifically, OS_TaskStat() computes the CPU usage.
*              CPU usage is determined by:
*
*                                                     OSIdleCtr
*                 OSCPUUsage = 100 * (1 - ------------)     (units are in %)
*                                                    OSIdleCtrMax
*
* Arguments  : pdata     this pointer is not used at this time.
*
* Returns    : none
*
* Notes      : 1) This task runs at a priority level higher than the idle task.  In fact, it runs at the
*                 next higher priority, OS_IDLE_PRIO-1.
*              2) You can disable this task by setting the configuration #define OS_TASK_STAT_EN to 0.
*              3) We delay for 5 seconds in the beginning to allow the system to reach steady state and
*                 have all other tasks created before we do statistics.  You MUST have at least a delay
*                 of 2 seconds to allow for the system to establish the maximum value for the idle
*                 counter.
                                                         统计任务：
描述：作多任务处理的一些统计，一般计算CPU使用率，公式如下：
*                                                      OSIdleCtr
*                 OSCPUUsage = 100 * (1 - ------------)     (units are in %)
*                                                     OSIdleCtrMax
参数：pdata:暂时没有用到
返回：无
备注：1、此任务优先级只比idle高，实际上，它运行在进一步高的优先
                       级上，OS_IDLE_PRIO-1
                2、通过设置OS_TASK_STAT_EN为零来禁止此任务
                3、我们延时5秒让系统稳定，统计前我们建立其它任务，
                      我们至少有两秒去让空闲任务建立最大值。

*********************************************************************************************************
*/

#if OS_TASK_STAT_EN > 0
void  OS_TaskStat (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    INT32U     run;
    INT32U     max;
    INT8S      usage;


    pdata = pdata;                               /* Prevent compiler warning for not using 'pdata'     */
    while (OSStatRdy == FALSE) {
        OSTimeDly(2 * OS_TICKS_PER_SEC);         /* Wait until statistic task is ready   */
		//延时两秒OSIdleCr不会像没有什么应用任务运行时那样有那么多计数，
		//它最大计数值是OSStatInit()在初始化时，保存在空闲计数器最大值OSIdleCtr中的
    }
    max = OSIdleCtrMax / 100L;
    for (;;) {
        OS_ENTER_CRITICAL();
        OSIdleCtrRun = OSIdleCtr;                /* Obtain the of the idle counter for the past second */
        run          = OSIdleCtr;
        OSIdleCtr    = 0L;                       /* Reset the idle counter for the next second         */
		//清除，用于下一次测量
        OS_EXIT_CRITICAL();
        if (max > 0L) {
            usage = (INT8S)(100L - run / max);
            if (usage >= 0) {                    /* Make sure we don't have a negative percentage      */
                OSCPUUsage = usage;
            } else {
                OSCPUUsage = 0;
            }
        } else {
            OSCPUUsage = 0;
            max        = OSIdleCtrMax / 100L;
        }
        OSTaskStatHook();                        /* Invoke user definable hook           */
		//一旦完成，就调用外界接入函数OSTaskStatHook()，能使统计任务得到扩展，
		//这样用户可以计算并显示所有任务总执行时间，每个任务执行的百分比等。
        OSTimeDly(OS_TICKS_PER_SEC); /* Accumulate OSIdleCtr for the next second           */
		//为下一秒作准备
    }
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                            INITIALIZE TCB
*
* Description: This function is internal to uC/OS-II and is used to initialize a Task Control Block when
*              a task is created (see OSTaskCreate() and OSTaskCreateExt()).
*
* Arguments  : prio          is the priority of the task being created
*
*              ptos          is a pointer to the task's top-of-stack assuming that the CPU registers
*                            have been placed on the stack.  Note that the top-of-stack corresponds to a
*                            'high' memory location is OS_STK_GROWTH is set to 1 and a 'low' memory
*                            location if OS_STK_GROWTH is set to 0.  Note that stack growth is CPU
*                            specific.
*
*              pbos          is a pointer to the bottom of stack.  A NULL pointer is passed if called by
*                            'OSTaskCreate()'.
*
*              id            is the task's ID (0..65535)
*
*              stk_size      is the size of the stack (in 'stack units').  If the stack units are INT8Us
*                            then, 'stk_size' contains the number of bytes for the stack.  If the stack
*                            units are INT32Us then, the stack contains '4 * stk_size' bytes.  The stack
*                            units are established by the #define constant OS_STK which is CPU
*                            specific.  'stk_size' is 0 if called by 'OSTaskCreate()'.
*
*              pext          is a pointer to a user supplied memory area that is used to extend the task
*                            control block.  This allows you to store the contents of floating-point
*                            registers, MMU registers or anything else you could find useful during a
*                            context switch.  You can even assign a name to each task and store this name
*                            in this TCB extension.  A NULL pointer is passed if called by OSTaskCreate().
*
*              opt           options as passed to 'OSTaskCreateExt()' or,
*                            0 if called from 'OSTaskCreate()'.
*
* Returns    : OS_NO_ERR         if the call was successful
*              OS_NO_MORE_TCB    if there are no more free TCBs to be allocated and thus, the task cannot
*                                be created.
*
* Note       : This function is INTERNAL to uC/OS-II and your application should not call it.
*********************************************************************************************************
*/
/*
*******************************************************************************
                                           初始化任务控制块
参数：prio：任务创建时的优先级
      ptos:假定CPU寄存器放置于堆栈中指向堆栈栈顶的指针。栈顶当OS_STK_GROWTH为1时是寄存器的  

         高位、当OS_STK_GROWTH为0时是寄存器的低位，堆栈增长是CPU的特权。
      pbos：栈底指针。由OSTaskCreate()调用时传入空指针。
      id： 任务的ID
      stk_size:堆栈大小。 当堆栈单位是int8us时，堆栈大小包含堆栈数量个字节，当堆栈单位是
               int32us时，堆栈大小包含“4*stk_size”个字节。堆栈单位由“#define constant
               OS_STK”建立，它是CPU特有。如果被OSTaskCreate()调用stk_size为0。
      pext：用户提供存储器空间的指针，用于任务控制块。允许存储浮点寄存器常量，MMU寄存器或
            者其它在内容转换时有用的东西。甚至在TCB扩展中为每个任务指定一个名字存到这个名 字里面。
            当被OSTaskCreate()调用的时候为空指针。
      opt ：传到OSTaskCreateExt()时可以选择，被OSTaskCreate()调用的时候为0。
返回：OS_NO_ERR ：如果调用成功。
      OS_NO_MORE_TCB：如果没有多余TCB安排，所以任务不能被创建。
备注：这个函数对uC/OS-II来说是内部的，自己的应用程序不能够调用它。

*************************************************************************
*/

INT8U  OS_TCBInit (INT8U prio, OS_STK *ptos, OS_STK *pbos, INT16U id, INT32U stk_size, void *pext, INT16U opt)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
//为CPU状态寄存器分配存储空间
    OS_CPU_SR  cpu_sr;//#define OS_CPU_SR    unsigned int 
#endif    
    OS_TCB    *ptcb;//定义任务控制块


    OS_ENTER_CRITICAL();//进入临界状态
    ptcb = OSTCBFreeList;                                  /* Get a free TCB from the free TCB list    */
	  //从空TCB列表中得到一块空TCB
    if (ptcb != (OS_TCB *)0) {//分配空TCB成功
        OSTCBFreeList        = ptcb->OSTCBNext;            /* Update pointer to free TCB list          */
		//更新空TCB列表，即减掉一块
        OS_EXIT_CRITICAL();//退出临界状态
        ptcb->OSTCBStkPtr    = ptos;                       /* Load Stack pointer in TCB                */
		//装载TCB中的堆栈指针
        ptcb->OSTCBPrio      = (INT8U)prio;                /* Load task priority into TCB              */
		//装载TCB中任务优先级
        ptcb->OSTCBStat      = OS_STAT_RDY;                /* Task is ready to run                     */
		//任务状态设为就绪
        ptcb->OSTCBDly       = 0;                          /* Task is not delayed                      */
		//任务不延时

#if OS_TASK_CREATE_EXT_EN > 0//如果能使
        ptcb->OSTCBExtPtr    = pext;                       /* Store pointer to TCB extension           */
        //存储TCB扩展指针
        ptcb->OSTCBStkSize   = stk_size;                   /* Store stack size */
		//存储堆栈大小
        ptcb->OSTCBStkBottom = pbos;                       /* Store pointer to bottom of stack         */
		//存储栈底指针
        ptcb->OSTCBOpt       = opt;                        /* Store task options                       */
		//存储任务选项
        ptcb->OSTCBId        = id;                         /* Store task ID                            */
		//保存任务ID
#else//如果不能使
        pext                 = pext;                       /* Prevent compiler warning if not used     */
        stk_size             = stk_size;
        pbos                 = pbos;
        opt                  = opt;
        id                   = id;//防止编译器警告，不要删除
#endif

#if OS_TASK_DEL_EN > 0//允许包含任务删除代码
        ptcb->OSTCBDelReq    = OS_NO_ERR;//是否允许自动删除
#endif

        ptcb->OSTCBY         = prio >> 3;                  /* Pre-compute X, Y, BitX and BitY          */
        ptcb->OSTCBBitY      = OSMapTbl[ptcb->OSTCBY];//提前计算X,Y,位X，位Y，
        ptcb->OSTCBX         = prio & 0x07;
        ptcb->OSTCBBitX      = OSMapTbl[ptcb->OSTCBX];//为什么要这样算？不知道

#if OS_EVENT_EN > 0
		//能使队列代码产生&&申请队列控制块最大数不为零||能使邮箱代码产生||
		//能使信号量代码产生||能使互斥量代码产生 

        ptcb->OSTCBEventPtr  = (OS_EVENT *)0;              /* Task is not pending on an event          */
//任务没有在事件中挂起
#endif

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0) && (OS_TASK_DEL_EN > 0)
//OS版本大于等于251&&能使事件标志代码产生&&最大标志数大于零&&
//允许包含任务删除函数
        ptcb->OSTCBFlagNode  = (OS_FLAG_NODE *)0;          /* Task is not pending on an event flag     */
//任务没有在事件标志中挂起
#endif

#if (OS_MBOX_EN > 0) || ((OS_Q_EN > 0) && (OS_MAX_QS > 0))
//允许邮箱代码产生||允许队列代码产生&&最大队列控制块大于零
        ptcb->OSTCBMsg       = (void *)0;                  /* No message received                      */
//没有接收到任务消息
#endif

#if OS_VERSION >= 204//ucos版本大于等于204
        OSTCBInitHook(ptcb);
#endif

        OSTaskCreateHook(ptcb);                            /* Call user defined hook                   */
//调用用户定义的hook
        
        OS_ENTER_CRITICAL();//进入临界状态
        OSTCBPrioTbl[prio] = ptcb;//任务控制块存入到已创建任务控制块列表当中
        ptcb->OSTCBNext    = OSTCBList;                    /* Link into TCB chain                      */
		//连入TCB链表OSTCBList为TCB双向链表指针
        ptcb->OSTCBPrev    = (OS_TCB *)0;//前驱为零
        if (OSTCBList != (OS_TCB *)0) {//如果链表为空
            OSTCBList->OSTCBPrev = ptcb;//前驱指向本身
        }
        OSTCBList               = ptcb;//本身为表头
        OSRdyGrp               |= ptcb->OSTCBBitY;         /* Make task ready to run                   */
		//位掩码存入就绪列表组，使任务就绪
        OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;
		//X位掩码存入Y组中，为什么？看不懂
        OS_EXIT_CRITICAL();//退出临界状态
        return (OS_NO_ERR);//返回成功
    }
    OS_EXIT_CRITICAL();//如果分配不成功，也退出临界状态
    return (OS_NO_MORE_TCB);//返回没有多余的TCB。
}
