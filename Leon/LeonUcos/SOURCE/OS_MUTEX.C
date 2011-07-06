/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                  MUTUAL EXCLUSION SEMAPHORE MANAGEMENT
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File : OS_MUTEX.C
* By   : Jean J. Labrosse

* 翻译： likee
                                            互斥型信号量管理
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

/*
*********************************************************************************************************
*                                            LOCAL CONSTANTS
*********************************************************************************************************
*/

#define  OS_MUTEX_KEEP_LOWER_8   0x00FF
#define  OS_MUTEX_KEEP_UPPER_8   0xFF00

#define  OS_MUTEX_AVAILABLE      0x00FF


#if OS_MUTEX_EN > 0
/*
*********************************************************************************************************
*                                   ACCEPT MUTUAL EXCLUSION SEMAPHORE
*
* Description: This  function checks the mutual exclusion semaphore to see if a resource is available.
*              Unlike OSMutexPend(), OSMutexAccept() does not suspend the calling task if the resource is
*              not available or the event did not occur.
*
* Arguments  : pevent     is a pointer to the event control block
*
*              err        is a pointer to an error code which will be returned to your application:
*                            OS_NO_ERR          if the call was successful.
*                            OS_ERR_EVENT_TYPE  if 'pevent' is not a pointer to a mutex
*                            OS_ERR_PEVENT_NULL 'pevent' is a NULL pointer
*                            OS_ERR_PEND_ISR     if you called this function from an ISR
*
* Returns    : == 1       if the resource is available, the mutual exclusion semaphore is acquired
*              == 0       a) if the resource is not available
*                         b) you didn't pass a pointer to a mutual exclusion semaphore
*                         c) you called this function from an ISR
*
* Warning(s) : This function CANNOT be called from an ISR because mutual exclusion semaphores are
*              intended to be used by tasks only.
*********************************************************************************************************
*/

#if OS_MUTEX_ACCEPT_EN > 0
INT8U  OSMutexAccept (OS_EVENT *pevent, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                            /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    if (OSIntNesting > 0) {                            /* Make sure it's not called from an ISR        */
        *err = OS_ERR_PEND_ISR;
        return (0);
    }
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                     /* Validate 'pevent'                            */
        *err = OS_ERR_PEVENT_NULL;
        return (0);
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) {  /* Validate event block type                    */
        *err = OS_ERR_EVENT_TYPE;
        return (0);
    }
#endif                                                     
    OS_ENTER_CRITICAL();							   /* Get value (0 or 1) of Mutex                  */
    if ((pevent->OSEventCnt & OS_MUTEX_KEEP_LOWER_8) == OS_MUTEX_AVAILABLE) {     
        pevent->OSEventCnt &= OS_MUTEX_KEEP_UPPER_8;   /*      Mask off LSByte (Acquire Mutex)         */
        pevent->OSEventCnt |= OSTCBCur->OSTCBPrio;     /*      Save current task priority in LSByte    */
        pevent->OSEventPtr  = (void *)OSTCBCur;        /*      Link TCB of task owning Mutex           */
        OS_EXIT_CRITICAL();
        *err = OS_NO_ERR;
        return (1);
    }
    OS_EXIT_CRITICAL();
    *err = OS_NO_ERR;
    return (0);
}
#endif                                                     

/*$PAGE*/
/*
*********************************************************************************************************
*                                  CREATE A MUTUAL EXCLUSION SEMAPHORE
*
* Description: This function creates a mutual exclusion semaphore.
*
* Arguments  : prio          is the priority to use when accessing the mutual exclusion semaphore.  In
*                            other words, when the semaphore is acquired and a higher priority task
*                            attempts to obtain the semaphore then the priority of the task owning the
*                            semaphore is raised to this priority.  It is assumed that you will specify
*                            a priority that is LOWER in value than ANY of the tasks competing for the
*                            mutex.
*
*              err           is a pointer to an error code which will be returned to your application:
*                               OS_NO_ERR           if the call was successful.
*                               OS_ERR_CREATE_ISR   if you attempted to create a MUTEX from an ISR
*                               OS_PRIO_EXIST       if a task at the priority inheritance priority
*                                                   already exist.
*                               OS_ERR_PEVENT_NULL  No more event control blocks available.
*                               OS_PRIO_INVALID     if the priority you specify is higher that the 
*                                                   maximum allowed (i.e. > OS_LOWEST_PRIO)
*
* Returns    : != (void *)0  is a pointer to the event control clock (OS_EVENT) associated with the
*                            created mutex.
*              == (void *)0  if an error is detected.
*
* Note(s)    : 1) The LEAST significant 8 bits of '.OSEventCnt' are used to hold the priority number
*                 of the task owning the mutex or 0xFF if no task owns the mutex.
*              2) The MOST  significant 8 bits of '.OSEventCnt' are used to hold the priority number
*                 to use to reduce priority inversion.
                                                     建立一个互斥型信号量
描述：建立一个互斥型信号量
参数：prio：当存取互斥型信号量时它的优先级。就是说，当任务需要信号量，
                     而另一优先级更高的任务想得到信号量，就改变当前任务的优先级，变为更高
                     假定你改变的优先级值小于任务竞争这个信号量的任务的值（即优先级更高）
                 err:应用时包含错误代码的指针 ：
*                               OS_NO_ERR          调用成功
*                               OS_ERR_CREATE_ISR   如果想从ISR中建立
*                               OS_PRIO_EXIST       如果优先级继承优先级的优先级已经存在
*                               OS_ERR_PEVENT_NULL  没有事件控制块可用
*                               OS_PRIO_INVALID    如果你指定的优先级大于最大值

*********************************************************************************************************
*/

OS_EVENT  *OSMutexCreate (INT8U prio, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    OS_EVENT  *pevent;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_CREATE_ISR;                          /* ... can't CREATE mutex from an ISR       */
        return ((OS_EVENT *)0);
    }//不能从ISR中建立，不允许在ISR中调用此函数
#if OS_ARG_CHK_EN > 0
    if (prio >= OS_LOWEST_PRIO) {                          /* Validate PIP                             */
        *err = OS_PRIO_INVALID;
        return ((OS_EVENT *)0);
    }//不合理的PIP
#endif
    OS_ENTER_CRITICAL();
    if (OSTCBPrioTbl[prio] != (OS_TCB *)0) {               /* Mutex priority must not already exist    */
		//确认PIP没有被任何任务占用。OSTCBPrioTbl[ ]中的一个指向NULL的空指针指示
		//PIP有效
        OS_EXIT_CRITICAL();                                /* Task already exist at priority ...       */
        *err = OS_PRIO_EXIST;                              /* ... inheritance priority                 */
		//如果优先级存在 ，则出错。
        return ((OS_EVENT *)0);                            
    }
    OSTCBPrioTbl[prio] = (OS_TCB *)1;                      /* Reserve the table entry                  */
	//置非空指针，将这个优先级保留下来。
    pevent             = OSEventFreeList;                  /* Get next free event control block        */
	//从空余ECB中得到一块空的ECB。
    if (pevent == (OS_EVENT *)0) {                         /* See if an ECB was available              */
		//看ECB是否可用
        OSTCBPrioTbl[prio] = (OS_TCB *)0;                  /* No, Release the table entry              */
		//如果不可用，释放此优先级表入口
        OS_EXIT_CRITICAL();
        *err               = OS_ERR_PEVENT_NULL;           /* No more event control blocks             */
        return (pevent);
    }
    OSEventFreeList     = (OS_EVENT *)OSEventFreeList->OSEventPtr;   /* Adjust the free list           */
	//如果可用，重新调整事件控制块的表头
    OS_EXIT_CRITICAL();
    pevent->OSEventType = OS_EVENT_TYPE_MUTEX;//将其标记为互斥型信号量
    pevent->OSEventCnt  = (prio << 8) | OS_MUTEX_AVAILABLE;/* Resource is available                    */
	//mutex为有效值，同时将PIP保存起来。值得注意的是，事件计数器.OSEventCnt
    //在此处的用法不同，高八位用于保存PIP的值，低侂位在资源无任务占用
    //时的值为0xff，有任务占用时为占用mutex任务的优先级。这个避免了增加额
    //外的空间，节约对RAM的占用量
    pevent->OSEventPtr  = (void *)0;                       /* No task owning the mutex       */
	//消息正在初始化，所以没有等待这个mutex的任务
    OS_EventWaitListInit(pevent);//初始化事件等待列表
    *err                = OS_NO_ERR;
    return (pevent);
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                          DELETE A MUTEX
*
* Description: This function deletes a mutual exclusion semaphore and readies all tasks pending on the it.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired mutex.
*
*              opt           determines delete options as follows:
*                            opt == OS_DEL_NO_PEND   Delete mutex ONLY if no task pending
*                            opt == OS_DEL_ALWAYS    Deletes the mutex even if tasks are waiting.
*                                                    In this case, all the tasks pending will be readied.
*
*              err           is a pointer to an error code that can contain one of the following values:
*                            OS_NO_ERR               The call was successful and the mutex was deleted
*                            OS_ERR_DEL_ISR          If you attempted to delete the MUTEX from an ISR
*                            OS_ERR_INVALID_OPT      An invalid option was specified
*                            OS_ERR_TASK_WAITING     One or more tasks were waiting on the mutex
*                            OS_ERR_EVENT_TYPE       If you didn't pass a pointer to a mutex
*                            OS_ERR_PEVENT_NULL      If 'pevent' is a NULL pointer.
*
* Returns    : pevent        upon error
*              (OS_EVENT *)0 if the mutex was successfully deleted.
*
* Note(s)    : 1) This function must be used with care.  Tasks that would normally expect the presence of
*                 the mutex MUST check the return code of OSMutexPend().
*              2) This call can potentially disable interrupts for a long time.  The interrupt disable
*                 time is directly proportional to the number of tasks waiting on the mutex.
*              3) Because ALL tasks pending on the mutex will be readied, you MUST be careful because the
*                 resource(s) will no longer be guarded by the mutex.
                                      删除一个互斥型信号量
描述： 删除一个互斥型信号量将挂起的任务就绪
参数：pevent：指向事件控制块结合目标mutex的指针
*              opt           决定删除选项
*                            opt == OS_DEL_NO_PEND   没有任务挂起时才删
*                            opt == OS_DEL_ALWAYS   即使有任务挂起也删，删除后所有等待的事件全部就绪
*              err          指向包含错误代码的指针
*                            OS_NO_ERR              调用成功 mutex 删除
*                            OS_ERR_DEL_ISR         如果在ISR中调用此函数
*                            OS_ERR_INVALID_OPT     设置了非法选项
*                            OS_ERR_TASK_WAITING    有任务在等待 mutex
*                            OS_ERR_EVENT_TYPE       如果没有传递指针到 mutex
*                            OS_ERR_PEVENT_NULL      如果 'pevent' 是一个空指针
备注：1) 小心使用，  Tasks that would normally expect the presence of
*                 the mutex MUST check the return code of OSMutexPend().
*              2) This call can potentially disable interrupts for a long time.  The interrupt disable
*                 time is directly proportional to the number of tasks waiting on the mutex.
*              3) Because ALL tasks pending on the mutex will be readied, you MUST be careful because the
*                 resource(s) will no longer be guarded by the mutex.
*********************************************************************************************************
*/

#if OS_MUTEX_DEL_EN
OS_EVENT  *OSMutexDel (OS_EVENT *pevent, INT8U opt, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    BOOLEAN    tasks_waiting;
    INT8U      pip;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_DEL_ISR;                             /* ... can't DELETE from an ISR             */
        return (pevent);
    }//不允许在ISR中调用此函数
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        *err = OS_ERR_PEVENT_NULL;
        return ((OS_EVENT *)0);
    }//非法的pevent
    if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) {      /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return (pevent);
    }//非法的事件控制块
#endif
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0x00) {                      /* See if any tasks waiting on mutex        */
		//是不是有任务在等待
        tasks_waiting = TRUE;                              /* Yes                                      */
    } else {
        tasks_waiting = FALSE;                             /* No                                       */
    }
    switch (opt) {//删除选项
        case OS_DEL_NO_PEND:                               /* Delete mutex only if no task waiting     */
			//无任务等待才删除
             if (tasks_waiting == FALSE) {
                 pip                 = (INT8U)(pevent->OSEventCnt >> 8);
                 OSTCBPrioTbl[pip]   = (OS_TCB *)0;        /* Free up the PIP                          */
				 //释放PIP，这两行程序书上没有
                 pevent->OSEventType = OS_EVENT_TYPE_UNUSED;//标记为没有使用
                 pevent->OSEventPtr  = OSEventFreeList;    /* Return Event Control Block to free list  */
                 OSEventFreeList     = pevent;//返回ECB到空闲列表
                 OS_EXIT_CRITICAL();
                 *err = OS_NO_ERR;
                 return ((OS_EVENT *)0);                   /* Mutex has been deleted                   */
             } else {//如果有任务在等待
                 OS_EXIT_CRITICAL();
                 *err = OS_ERR_TASK_WAITING;
                 return (pevent);
             }

        case OS_DEL_ALWAYS:                                /* Always delete the mutex                  */
			//如果强制删除
             while (pevent->OSEventGrp != 0x00) {          /* Ready ALL tasks waiting for mutex        */
                 OS_EventTaskRdy(pevent, (void *)0, OS_STAT_MUTEX);
             }//就绪所有等待任务
             pip                 = (INT8U)(pevent->OSEventCnt >> 8);
             OSTCBPrioTbl[pip]   = (OS_TCB *)0;            /* Free up the PIP                          */
			 //释放PIP
             pevent->OSEventType = OS_EVENT_TYPE_UNUSED;//标记为未用
             pevent->OSEventPtr  = OSEventFreeList;        /* Return Event Control Block to free list  */
             OSEventFreeList     = pevent;                 /* Get next free event control block        */
			 //将ECB标明为没有被占用，并被送回到空余事件控制块
             OS_EXIT_CRITICAL();
             if (tasks_waiting == TRUE) {                  /* Reschedule only if task(s) were waiting  */
                 OS_Sched();                               /* Find highest priority task ready to run  */
             }//如果有任务等待，因为全部就绪，就进行任务调度。
             *err = OS_NO_ERR;
             return ((OS_EVENT *)0);                       /* Mutex has been deleted                   */

        default://其它异常情况
             OS_EXIT_CRITICAL();
             *err = OS_ERR_INVALID_OPT;
             return (pevent);
    }
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                  PEND ON MUTUAL EXCLUSION SEMAPHORE
*
* Description: This function waits for a mutual exclusion semaphore.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired
*                            mutex.
*
*              timeout       is an optional timeout period (in clock ticks).  If non-zero, your task will
*                            wait for the resource up to the amount of time specified by this argument.
*                            If you specify 0, however, your task will wait forever at the specified
*                            mutex or, until the resource becomes available.
*
*              err           is a pointer to where an error message will be deposited.  Possible error
*                            messages are:
*                               OS_NO_ERR          The call was successful and your task owns the mutex
*                               OS_TIMEOUT         The mutex was not available within the specified time.
*                               OS_ERR_EVENT_TYPE  If you didn't pass a pointer to a mutex
*                               OS_ERR_PEVENT_NULL 'pevent' is a NULL pointer
*                               OS_ERR_PEND_ISR    If you called this function from an ISR and the result
*                                                  would lead to a suspension.
*
* Returns    : none
*
* Note(s)    : 1) The task that owns the Mutex MUST NOT pend on any other event while it owns the mutex.
*              2) You MUST NOT change the priority of the task that owns the mutex
                                                     等待一个互斥型信号量
描述： 等待一个互斥型信号量
参数：pevent：指向事件控制块结合目标mutex的指针
                timeout：超时时间选项（按时钟节拍来），如果非零，任务将按照此
                             参数的定时在队列中等待消息到来，如果设置为零，任务将在目标
                             队列中永远等待，直到消息到来。
                err：指向可能的错误消息的指针，可能为：
*                            OS_NO_ERR        调用成功，任务接收到mutex
*                            OS_TIMEOUT         定时时间内mutex没有来
*                            OS_ERR_EVENT_TYPE   你没有传递指针到队列
*                            OS_ERR_PEVENT_NULL  如果 'pevent' 是一个空指针
*                            OS_ERR_PEND_ISR    如果从ISR中调用，结果将出现异常      
返回：无
备注：拥有mutex的任务不能在拥有的同时在其它事件中挂起
                 不能改变拥有mutex的信号量
*********************************************************************************************************
*/
void  OSMutexPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    INT8U      pip;                                        /* Priority Inheritance Priority (PIP)      */
//优先级继承优先级
    INT8U      mprio;                                      /* Mutex owner priority                     */
//nutex拥有的优先级
    BOOLEAN    rdy;                                        /* Flag indicating task was ready           */
//显示任务就绪的标志
    OS_TCB    *ptcb;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_PEND_ISR;                            /* ... can't PEND from an ISR               */
        return;
    }//不允许在ISR中调用此函数
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        *err = OS_ERR_PEVENT_NULL;
        return;
    }//非法的pevent
    if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) {      /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return;
    }//非法的事件控制块
#endif
    OS_ENTER_CRITICAL();								   /* Is Mutex available?                      */
    if ((INT8U)(pevent->OSEventCnt & OS_MUTEX_KEEP_LOWER_8) == OS_MUTEX_AVAILABLE) {//确保Mutex有效
        pevent->OSEventCnt &= OS_MUTEX_KEEP_UPPER_8;       /* Yes, Acquire the resource                */
		//如果OSEventCnt低八位是0xFF，这个mutex有效，此函数将mutex赋给任务，且
		//此函数置OSEventCnt低八位是调用此函数的优先级
        pevent->OSEventCnt |= OSTCBCur->OSTCBPrio;         /*      Save priority of owning task        */
        pevent->OSEventPtr  = (void *)OSTCBCur;            /*      Point to owning task's OS_TCB       */
		//OSEventPtr指向调用该函数任务的任务控制块。
        OS_EXIT_CRITICAL();
        *err  = OS_NO_ERR;
        return;
    }
	//如果mutex不可用
    pip   = (INT8U)(pevent->OSEventCnt >> 8);                     /* No, Get PIP from mutex            */
	//在调用让任务进入休眠状态程序前，先提取mutex中的PIP，占用mutex任务的
	//优先级和占用mutex的任务控制块指针
    mprio = (INT8U)(pevent->OSEventCnt & OS_MUTEX_KEEP_LOWER_8);  /*     Get priority of mutex owner   */
    ptcb  = (OS_TCB *)(pevent->OSEventPtr);                       /*     Point to TCB of mutex owner   */
//如果占用mutex的任务优先级比调用此函数的任务优先级值大（低），则要将
//占用此mutex的任务优先级提升到PIP。
    if (ptcb->OSTCBPrio != pip && mprio > OSTCBCur->OSTCBPrio) {  /*     Need to promote prio of owner?*/
        if ((OSRdyTbl[ptcb->OSTCBY] & ptcb->OSTCBBitX) != 0x00) { /*     See if mutex owner is ready   */
                                                                  /*     Yes, Remove owner from Rdy ...*/
//如果任务处于就绪态，那么这个任务就已不是处在它原来优先级上的就绪态，
//是以置rdy标志。表明占用mutex的任务进入就绪态。可以运行。
                                                                  /*          ... list at current prio */
            if ((OSRdyTbl[ptcb->OSTCBY] &= ~ptcb->OSTCBBitX) == 0x00) {
                OSRdyGrp &= ~ptcb->OSTCBBitY;
            }
            rdy = TRUE;
        } else {
            rdy = FALSE;                                          /* No                                */
        }//如果任务没进入就绪态，rdy置位与否要看情况。置位是为了表明需要让这个
        //任务在新的，较高的优先级PIP上进入就绪态
        ptcb->OSTCBPrio         = pip;                     /* Change owner task prio to PIP            */
        ptcb->OSTCBY            = ptcb->OSTCBPrio >> 3;//在优先级继续优先级上处理任务控制块。
        ptcb->OSTCBBitY         = OSMapTbl[ptcb->OSTCBY];
        ptcb->OSTCBX            = ptcb->OSTCBPrio & 0x07;
        ptcb->OSTCBBitX         = OSMapTbl[ptcb->OSTCBX];
        if (rdy == TRUE) {                                 /* If task was ready at owner's priority ...*/
			//确定占用mutex的任务是否应在PIP优先级上进入就绪态
            OSRdyGrp               |= ptcb->OSTCBBitY;     /* ... make it ready at new priority.       */
            OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;//保证在新优先级上就绪
        }
        OSTCBPrioTbl[pip]       = (OS_TCB *)ptcb;
    }
    OSTCBCur->OSTCBStat |= OS_STAT_MUTEX;             /* Mutex not available, pend current task        */
	//为了让调用本函数的任务进入休眠态，就将任务的TCB置位，表明等待的任务已经挂起
    OSTCBCur->OSTCBDly   = timeout;                   /* Store timeout in current task's TCB           */
	//保存超时时间
    OS_EventTaskWait(pevent);                         /* Suspend task until event or timeout occurs    */
	//挂起任务直到超时或者事件发生
    OS_EXIT_CRITICAL();
    OS_Sched();                                        /* Find next highest priority task ready         */
	//找另一优先级最高任务运行
    OS_ENTER_CRITICAL();
    if (OSTCBCur->OSTCBStat & OS_STAT_MUTEX) {        /* Must have timed out if still waiting for event*/
		//看是否还在等待mutex，如果还在等待，说明一定是没有从OSMutexPost()
      //中得到mutex，而是由于OSTimeTick（）函数进入就绪态，出现等待超时
        OS_EventTO(pevent);//让等待超时的任务运行
        OS_EXIT_CRITICAL();
        *err = OS_TIMEOUT;                            /* Indicate that we didn't get mutex within TO   */
        return;
    }
    OSTCBCur->OSTCBEventPtr = (OS_EVENT *)0;//释放事件控制块指针
    OS_EXIT_CRITICAL();
    *err = OS_NO_ERR;
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                  POST TO A MUTUAL EXCLUSION SEMAPHORE
*
* Description: This function signals a mutual exclusion semaphore
*
* Arguments  : pevent              is a pointer to the event control block associated with the desired
*                                  mutex.
*
* Returns    : OS_NO_ERR               The call was successful and the mutex was signaled.
*              OS_ERR_EVENT_TYPE       If you didn't pass a pointer to a mutex
*              OS_ERR_PEVENT_NULL      'pevent' is a NULL pointer
*              OS_ERR_POST_ISR         Attempted to post from an ISR (not valid for MUTEXes)
*              OS_ERR_NOT_MUTEX_OWNER  The task that did the post is NOT the owner of the MUTEX.
                                          释放一个互斥型信号量
描述：释放一个互斥型信号量
参数：pevent：指向事件控制块结合目标mutex的指针
返回：OS_NO_ERR               调用成功，mutex成功释放
*              OS_ERR_EVENT_TYPE       如果没有传递指针到mutex
*              OS_ERR_PEVENT_NULL      'pevent' 是空指针
*              OS_ERR_POST_ISR        想从ISR中释放
*              OS_ERR_NOT_MUTEX_OWNER  想释放的任务并没有占有此mutex
*********************************************************************************************************
*/

INT8U  OSMutexPost (OS_EVENT *pevent)
{
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr;
#endif    
    INT8U      pip;                                   /* Priority inheritance priority                 */
//优先级继承优先级
    INT8U      prio;


    if (OSIntNesting > 0) {                           /* See if called from ISR ...                    */
        return (OS_ERR_POST_ISR);                     /* ... can't POST mutex from an ISR              */
    }
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        return (OS_ERR_PEVENT_NULL);
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) { /* Validate event block type                     */
        return (OS_ERR_EVENT_TYPE);
    }                                                 
#endif
    OS_ENTER_CRITICAL();
    pip  = (INT8U)(pevent->OSEventCnt >> 8);          /* Get priority inheritance priority of mutex    */
	//得到PIP的优先级值
    prio = (INT8U)(pevent->OSEventCnt & OS_MUTEX_KEEP_LOWER_8);  /* Get owner's original priority      */
	//得到占有任务的原始优先级
    if (OSTCBCur->OSTCBPrio != pip && 
        OSTCBCur->OSTCBPrio != prio) {                /* See if posting task owns the MUTEX            */
        //看看释放的任务是否占有此mutex
        OS_EXIT_CRITICAL();
        return (OS_ERR_NOT_MUTEX_OWNER);
    }
    if (OSTCBCur->OSTCBPrio == pip) {                 /* Did we have to raise current task's priority? */
                                                      /* Yes, Return to original priority              */
                                                      /*      Remove owner from ready list at 'pip'    */
        if ((OSRdyTbl[OSTCBCur->OSTCBY] &= ~OSTCBCur->OSTCBBitX) == 0) {
//将调用本函数的任务从任务就绪表中PIP位置删去，放回任务就绪表原来的优先
//级位置上。原来任务优先级的TCB将被重新处置
            OSRdyGrp &= ~OSTCBCur->OSTCBBitY;
        }
        OSTCBCur->OSTCBPrio         = prio;
        OSTCBCur->OSTCBY            = prio >> 3;
        OSTCBCur->OSTCBBitY         = OSMapTbl[OSTCBCur->OSTCBY];
        OSTCBCur->OSTCBX            = prio & 0x07;
        OSTCBCur->OSTCBBitX         = OSMapTbl[OSTCBCur->OSTCBX];
        OSRdyGrp                   |= OSTCBCur->OSTCBBitY;
        OSRdyTbl[OSTCBCur->OSTCBY] |= OSTCBCur->OSTCBBitX;
        OSTCBPrioTbl[prio]          = (OS_TCB *)OSTCBCur;
    }
    OSTCBPrioTbl[pip] = (OS_TCB *)1;                  /* Reserve table entry                           */
	//保留入口
    if (pevent->OSEventGrp != 0x00) {                 /* Any task waiting for the mutex?               */
                                                      /* Yes, Make HPT waiting for mutex ready         */
	//是的，让等待的高优先级任务就绪
        prio                = OS_EventTaskRdy(pevent, (void *)0, OS_STAT_MUTEX);
        pevent->OSEventCnt &= OS_MUTEX_KEEP_UPPER_8;  /*      Save priority of mutex's new owner       */
		//保存新占有者的优先级
        pevent->OSEventCnt |= prio;//新的占有mutex任务的优先级保存在mutex的ECB中
        pevent->OSEventPtr  = OSTCBPrioTbl[prio];     /*      Link to mutex owner's OS_TCB             */
		//加到单向链表中去
        OS_EXIT_CRITICAL();
        OS_Sched();                                   /*      Find highest priority task ready to run  */
		//任务调度
        return (OS_NO_ERR);
    }
    pevent->OSEventCnt |= OS_MUTEX_AVAILABLE;         /* No,  Mutex is now available                   */
    pevent->OSEventPtr  = (void *)0;
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                     QUERY A MUTUAL EXCLUSION SEMAPHORE
*
* Description: This function obtains information about a mutex
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired mutex
*
*              pdata         is a pointer to a structure that will contain information about the mutex
*
* Returns    : OS_NO_ERR            The call was successful and the message was sent
*              OS_ERR_QUERY_ISR     If you called this function from an ISR
*              OS_ERR_PEVENT_NULL   'pevent' is a NULL pointer
*              OS_ERR_EVENT_TYPE    If you are attempting to obtain data from a non mutex.
                                                获取互斥型信号量的当前状态
描述： 获取互斥型信号量的当前状态
参数：pevent：指向事件控制块结合目标mutex的指针
                 pdata：指向包含mutex信息的结构指针
*********************************************************************************************************
*/

#if OS_MUTEX_QUERY_EN > 0
INT8U  OSMutexQuery (OS_EVENT *pevent, OS_MUTEX_DATA *pdata)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    INT8U     *psrc;
    INT8U     *pdest;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        return (OS_ERR_QUERY_ISR);                         /* ... can't QUERY mutex from an ISR        */
    }//不能从ISR中调用
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        return (OS_ERR_PEVENT_NULL);
    }//非法的pevent
    if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) {      /* Validate event block type                */
        return (OS_ERR_EVENT_TYPE);
    }//非法的事件控制块类型
#endif
    OS_ENTER_CRITICAL();
    pdata->OSMutexPIP  = (INT8U)(pevent->OSEventCnt >> 8);//将相应的参数传给OS_MUTE_DATA,mutex的PIP
//由OSEnentCnt的高八位得到。
    pdata->OSOwnerPrio = (INT8U)(pevent->OSEventCnt & OS_MUTEX_KEEP_LOWER_8);
    if (pdata->OSOwnerPrio == 0xFF) {
        pdata->OSValue = 1;//从.OSEventCnt的低八位得到mutex的值，如果mutex有效（低八位是0XFF）.
       //将mutex的值设为一
    } else {//否则设置为零
        pdata->OSValue = 0;
    }
    pdata->OSEventGrp  = pevent->OSEventGrp;           //  复制链表
    psrc               = &pevent->OSEventTbl[0];
    pdest              = &pdata->OSEventTbl[0];
#if OS_EVENT_TBL_SIZE > 0
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 1
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 2
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 3
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 4
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 5
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 6
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 7
    *pdest             = *psrc;
#endif
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif                                                     /* OS_MUTEX_QUERY_EN                        */
#endif                                                     /* OS_MUTEX_EN                              */
