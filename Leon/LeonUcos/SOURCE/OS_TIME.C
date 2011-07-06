/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                             TIME MANAGEMENT
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File : OS_TIME.C
* By   : Jean J. Labrosse
* 翻译： likee
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

/*
*********************************************************************************************************
*                                DELAY TASK 'n' TICKS   (n from 0 to 65535)
*
* Description: This function is called to delay execution of the currently running task until the
*              specified number of system ticks expires.  This, of course, directly equates to delaying
*              the current task for some time to expire.  No delay will result If the specified delay is
*              0.  If the specified delay is greater than 0 then, a context switch will result.
*
* Arguments  : ticks     is the time delay that the task will be suspended in number of clock 'ticks'.
*                        Note that by specifying 0, the task will not be delayed.
*
* Returns    : none
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                           延迟任务N个节拍（N从0到65535）
 描述：此函数调用去延迟正在运行的任务直到规定时间期满，如果延迟0
                         就等于没有延迟，如果大于零，将会发生任务转换
 参数：ticks：延迟任务将被挂起的时钟节拍数，是零的话不延迟
 返回：无                     
*********************************************************************************************************
*/



void  OSTimeDly (INT16U ticks)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    


    if (ticks > 0) {      /* 0 means no delay!         *///如果需要延迟
        OS_ENTER_CRITICAL();
        if ((OSRdyTbl[OSTCBCur->OSTCBY] &= ~OSTCBCur->OSTCBBitX) == 0) {  /* Delay current task        */
            OSRdyGrp &= ~OSTCBCur->OSTCBBitY;//从就绪表中移除这个任务，即删除当前任务
        }
        OSTCBCur->OSTCBDly = ticks;              /* Load ticks in TCB         *///保存延迟节拍数到OS_TCB中
        OS_EXIT_CRITICAL();
        OS_Sched();                /* Find next task to run!    *///任务调度，找另一个任务运行
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                     DELAY TASK FOR SPECIFIED TIME
*
* Description: This function is called to delay execution of the currently running task until some time
*              expires.  This call allows you to specify the delay time in HOURS, MINUTES, SECONDS and
*              MILLISECONDS instead of ticks.
*
* Arguments  : hours     specifies the number of hours that the task will be delayed (max. is 255)
*              minutes   specifies the number of minutes (max. 59)
*              seconds   specifies the number of seconds (max. 59)
*              milli     specifies the number of milliseconds (max. 999)
*
* Returns    : OS_NO_ERR
*              OS_TIME_INVALID_MINUTES
*              OS_TIME_INVALID_SECONDS
*              OS_TIME_INVALID_MS
*              OS_TIME_ZERO_DLY
*
* Note(s)    : The resolution on the milliseconds depends on the tick rate.  For example, you can't do
*              a 10 mS delay if the ticker interrupts every 100 mS.  In this case, the delay would be
*              set to 0.  The actual delay is rounded to the nearest tick.
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                         按时分秒延迟函数
描述：使任务延迟直到一定时间期满，它可以按照小时，分，秒和毫秒
                 来延时，而非节拍
参数：hours:延迟小时数（0到255）
                minutes：指定分钟数：0到59
                seconds：指定秒钟数：0到59
                milli：毫秒数：0到999
返回：  OS_NO_ERR
*              OS_TIME_INVALID_MINUTES
*              OS_TIME_INVALID_SECONDS
*              OS_TIME_INVALID_MS
*              OS_TIME_ZERO_DLY  
备注：延迟毫秒数取决于节拍频率，比如：如果节拍是100ms，那么你不能够
延迟10ms，所以，它将设为0，实际的延迟与最近的节拍相匹配
*********************************************************************************************************
*/



#if OS_TIME_DLY_HMSM_EN > 0
INT8U  OSTimeDlyHMSM (INT8U hours, INT8U minutes, INT8U seconds, INT16U milli)
{
    INT32U ticks;
    INT16U loops;


    if (hours > 0 || minutes > 0 || seconds > 0 || milli > 0) {//确实需要延迟
        if (minutes > 59) {
            return (OS_TIME_INVALID_MINUTES);    /* Validate arguments to be within range              */
        }
        if (seconds > 59) {
            return (OS_TIME_INVALID_SECONDS);
        }
        if (milli > 999) {
            return (OS_TIME_INVALID_MILLI);
        }//分，秒，毫秒出错
                                                 /* Compute the total number of clock ticks required.. */
                                                 /* .. (rounded to the nearest tick)                   */
        ticks = ((INT32U)hours * 3600L + (INT32U)minutes * 60L + (INT32U)seconds) * OS_TICKS_PER_SEC
              + OS_TICKS_PER_SEC * ((INT32U)milli + 500L / OS_TICKS_PER_SEC) / 1000L;
	//计算要延迟的时间里的节拍数，采用了四舍五入
        loops = (INT16U)(ticks / 65536L);        /* Compute the integral number of 65536 tick delays   */
	//因为ucosII只能延迟65535个节拍，所以如果多于它的话，要分几次延
        ticks = ticks % 65536L;                  /* Obtain  the fractional number of ticks             */
	//余下不足65535的，作一次延迟
        OSTimeDly((INT16U)ticks);//先延不足65535的
        while (loops > 0) {//再延loop个65535年节拍。
            OSTimeDly(32768);
            OSTimeDly(32768);
            loops--;
        }
        return (OS_NO_ERR);
    }
    return (OS_TIME_ZERO_DLY);//如果没有延迟
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                         RESUME A DELAYED TASK
*
* Description: This function is used resume a task that has been delayed through a call to either
*              OSTimeDly() or OSTimeDlyHMSM().  Note that you MUST NOT call this function to resume a
*              task that is waiting for an event with timeout.  This situation would make the task look
*              like a timeout occurred (unless you desire this effect).  Also, you cannot resume a task
*              that has called OSTimeDlyHMSM() with a combined time that exceeds 65535 clock ticks.  In
*              other words, if the clock tick runs at 100 Hz then, you will not be able to resume a
*              delayed task that called OSTimeDlyHMSM(0, 10, 55, 350) or higher.
*
*                  (10 Minutes * 60 + 55 Seconds + 0.35) * 100 ticks/second.
*
* Arguments  : prio      specifies the priority of the task to resume
*
* Returns    : OS_NO_ERR                 Task has been resumed
*              OS_PRIO_INVALID           if the priority you specify is higher that the maximum allowed
*                                        (i.e. >= OS_LOWEST_PRIO)
*              OS_TIME_NOT_DLY           Task is not waiting for time to expire
*              OS_TASK_NOT_EXIST         The desired task has not been created
********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                                恢复一个延迟任务
描述：这个函数通过调用OSTimeDly() 或者 OSTimeDlyHMSM()去恢复一个延时的任务，它不能唤醒
               等待超时的任务。这个情况任务将把它看成等待超时，除非你指定这种效果，
               同时，不能恢复调用OSTimeDlyHMSM() 延时超过65535个时钟节拍的任务，就是说，如果
               时钟节拍是100Hz，你将不能够任务OSTimeDlyHMSM(0, 10, 55, 350) 或者更大延迟数
 参数：prio：要恢复任务的优先级
返回	  : OS_NO_ERR				  成功恢复
 *				OS_PRIO_INVALID 		  优先级值大于最大值
 *										  (i.e. >= OS_LOWEST_PRIO)
 *				OS_TIME_NOT_DLY 		  任务没有等待时间期满
 *				OS_TASK_NOT_EXIST		  要恢复的任务没有创建

********************************************************************************************************
*/



#if OS_TIME_DLY_RESUME_EN > 0
INT8U  OSTimeDlyResume (INT8U prio)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    OS_TCB    *ptcb;


    if (prio >= OS_LOWEST_PRIO) {//要保证优先级有效
        return (OS_PRIO_INVALID);
    }
    OS_ENTER_CRITICAL();
    ptcb = (OS_TCB *)OSTCBPrioTbl[prio];                   /* Make sure that task exist                */
	//将任务的TCB提取出来
    if (ptcb != (OS_TCB *)0) {//要保证不为空
        if (ptcb->OSTCBDly != 0) {                         /* See if task is delayed                   */
			//如果任务被延迟
            ptcb->OSTCBDly  = 0;                           /* Clear the time delay                     */
			//取消延迟
            if ((ptcb->OSTCBStat & OS_STAT_SUSPEND) == OS_STAT_RDY) {  /* See if task is ready to run  */
				//看任务是否就绪，如果没有，准备就绪
                OSRdyGrp               |= ptcb->OSTCBBitY;             /* Make task ready to run       */
                OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;
                OS_EXIT_CRITICAL();
                OS_Sched();                                /* See if this is new highest priority      */
				//任务调度
            } else {//如果任务就绪，就不用管了。
                OS_EXIT_CRITICAL();                        /* Task may be suspended                    */
            }
            return (OS_NO_ERR);
        } else {
            OS_EXIT_CRITICAL();
            return (OS_TIME_NOT_DLY);   /* Indicate that task was not delayed       */
             //如果为零，显示任务没有延时，
        }
    }
    OS_EXIT_CRITICAL();
    return (OS_TASK_NOT_EXIST);      /* The task does not exist      *///如果任务不存在
}
#endif    
/*$PAGE*/
/*
*********************************************************************************************************
*                                         GET CURRENT SYSTEM TIME
*
* Description: This function is used by your application to obtain the current value of the 32-bit
*              counter which keeps track of the number of clock ticks.
*
* Arguments  : none
*
* Returns    : The current value of OSTime
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                                    获取当前时间
 描述：获得32位跟踪时钟节拍数的计数器，获得当前值
 参数：无
 返回：OStime的当前值
 备注：在访问OStime的时候，中断是关的，因为大多数八位处理器上增加或者拷贝
一定32位数，要数条指令，中途不能被打断
*********************************************************************************************************
*/



#if OS_TIME_GET_SET_EN > 0
INT32U  OSTimeGet (void)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    INT32U     ticks;


    OS_ENTER_CRITICAL();
    ticks = OSTime;//获取当前值
    OS_EXIT_CRITICAL();
    return (ticks);//返回当前值
}
#endif    

/*
*********************************************************************************************************
*                                            SET SYSTEM CLOCK
*
* Description: This function sets the 32-bit counter which keeps track of the number of clock ticks.
*
* Arguments  : ticks      specifies the new value that OSTime needs to take.
*
* Returns    : none
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                       设置系统时间
 描述：此函数设置32位跟踪系统时钟节拍的计数器
 参数：ticks：OSTime的新值
 返回：无
*********************************************************************************************************
*/



#if OS_TIME_GET_SET_EN > 0
void  OSTimeSet (INT32U ticks)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    


    OS_ENTER_CRITICAL();
    OSTime = ticks;//设置好，
    OS_EXIT_CRITICAL();
}
#endif    
