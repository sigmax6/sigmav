/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                          SEMAPHORE MANAGEMENT
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File : OS_SEM.C
* By   : Jean J. Labrosse
* 翻译： likee
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

#if OS_SEM_EN > 0
/*
*********************************************************************************************************
*                                           ACCEPT SEMAPHORE接受信号量
*
* Description: This function checks the semaphore to see if a resource is available or, if an event
*              occurred.  Unlike OSSemPend(), OSSemAccept() does not suspend the calling task if the
*              resource is not available or the event did not occur.
*
* Arguments  : pevent     is a pointer to the event control block
*
* Returns    : >  0       if the resource is available or the event did not occur the semaphore is
*                         decremented to obtain the resource.
*              == 0       if the resource is not available or the event did not occur or,
*                         if 'pevent' is a NULL pointer or,
*                         if you didn't pass a pointer to a semaphore
                                                        
*********************************************************************************************************
*/

#if OS_SEM_ACCEPT_EN > 0
INT16U  OSSemAccept (OS_EVENT *pevent)
{
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr;
#endif    
    INT16U     cnt;


#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        return (0);
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_SEM) {   /* Validate event block type                     */
        return (0);
    }
#endif
    OS_ENTER_CRITICAL();
    cnt = pevent->OSEventCnt;
    if (cnt > 0) {                                    /* See if resource is available                  */
        pevent->OSEventCnt--;                         /* Yes, decrement semaphore and notify caller    */
    }
    OS_EXIT_CRITICAL();
    return (cnt);                                     /* Return semaphore count                        */
}
#endif    

/*$PAGE*/
/*
*********************************************************************************************************
*                                           CREATE A SEMAPHORE
*
* Description: This function creates a semaphore.
*
* Arguments  : cnt           is the initial value for the semaphore.  If the value is 0, no resource is
*                            available (or no event has occurred).  You initialize the semaphore to a
*                            non-zero value to specify how many resources are available (e.g. if you have
*                            10 resources, you would initialize the semaphore to 10).
*
* Returns    : != (void *)0  is a pointer to the event control clock (OS_EVENT) associated with the
*                            created semaphore
*              == (void *)0  if no event control blocks were available
                                                             建立一个信号量
描述：建立一个信号量
参数：cnt：信号量的初始化值，如果为零，则没有信号量可用（或者没有事件发生）
                          用非零值初始化信号量则显示它有多少可用资源，如果你有十个
                          资源，则初始化信号量为10
返回：!= (void *)0  指向事件控制块结合建立的信号量的指针
*              == (void *)0  如果没有可用事件控制块
*********************************************************************************************************
*/

OS_EVENT  *OSSemCreate (INT16U cnt)
{
#if OS_CRITICAL_METHOD == 3                       /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    OS_EVENT  *pevent;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        return ((OS_EVENT *)0);                            /* ... can't CREATE from an ISR             */
    }//中断服务子程序不能调用此函数，所有建立信号弹量的工作必须在任务级
    //代码中或者多任务启动前完成
    OS_ENTER_CRITICAL();
    pevent = OSEventFreeList;                              /* Get next free event control block        */
	//从空余事件控制块链表中获得一个事件控制块ECB
    if (OSEventFreeList != (OS_EVENT *)0) {                /* See if pool of free ECB pool was empty   */
//如果成功获得，则调整空余事件控制块链表。使之指向下一个空余的事件控制块
        OSEventFreeList = (OS_EVENT *)OSEventFreeList->OSEventPtr;
    }
    OS_EXIT_CRITICAL();
    if (pevent != (OS_EVENT *)0) {                         /* Get an event control block               */
		//如果得到的事件控制块可用，就将其的事件类型设置成信号量，
        pevent->OSEventType = OS_EVENT_TYPE_SEM;
        pevent->OSEventCnt  = cnt;                         /* Set semaphore value                      */
		//将信号量的初始值存入事件控制块ECB中。
        pevent->OSEventPtr  = (void *)0;                   /* Unlink from ECB free list                */
		//将.OSEventPtr初始化为NULL，因为它不于属于空余事件控制块链表了。
        OS_EventWaitListInit(pevent);                      /* Initialize to 'nobody waiting' on sem.   */
		//对等待任务列表进行初始化，因为信号量正在初始化，这时没有任何
		//任务等待该信号量，所以函数将.OSEventGrp()和.OSEventTbl[]清零
    }
    return (pevent);//返回调用函数一个指向事件控制块ECB的指针
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                         DELETE A SEMAPHORE
*
* Description: This function deletes a semaphore and readies all tasks pending on the semaphore.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired
*                            semaphore.
*
*              opt           determines delete options as follows:
*                            opt == OS_DEL_NO_PEND   Delete semaphore ONLY if no task pending
*                            opt == OS_DEL_ALWAYS    Deletes the semaphore even if tasks are waiting.
*                                                    In this case, all the tasks pending will be readied.
*
*              err           is a pointer to an error code that can contain one of the following values:
*                            OS_NO_ERR               The call was successful and the semaphore was deleted
*                            OS_ERR_DEL_ISR          If you attempted to delete the semaphore from an ISR
*                            OS_ERR_INVALID_OPT      An invalid option was specified
*                            OS_ERR_TASK_WAITING     One or more tasks were waiting on the semaphore
*                            OS_ERR_EVENT_TYPE       If you didn't pass a pointer to a semaphore
*                            OS_ERR_PEVENT_NULL      If 'pevent' is a NULL pointer.
*
* Returns    : pevent        upon error
*              (OS_EVENT *)0 if the semaphore was successfully deleted.
*
* Note(s)    : 1) This function must be used with care.  Tasks that would normally expect the presence of
*                 the semaphore MUST check the return code of OSSemPend().
*              2) OSSemAccept() callers will not know that the intended semaphore has been deleted unless
*                 they check 'pevent' to see that it's a NULL pointer.
*              3) This call can potentially disable interrupts for a long time.  The interrupt disable
*                 time is directly proportional to the number of tasks waiting on the semaphore.
*              4) Because ALL tasks pending on the semaphore will be readied, you MUST be careful in
*                 applications where the semaphore is used for mutual exclusion because the resource(s)
*                 will no longer be guarded by the semaphore.
                                              删除一个信号量
描述：删除一个信号量
参数：pevent：指向事件控制块结合目标信号量的指针
                opt:决定删除选项：
 *                            opt == OS_DEL_NO_PEND   没有任务挂起的时候才删除。
*                            opt == OS_DEL_ALWAYS    即使有任务等待也删除信号量，这种情况下，
                                   所有挂起的任务将就绪
 *              err           可能包含指下信息的错误代码的指针
*                            OS_NO_ERR               调用成功，信号量删除
*                            OS_ERR_DEL_ISR          如果想从中断服务子程序中删除信号量
*                            OS_ERR_INVALID_OPT      指定了不合理的选项
*                            OS_ERR_TASK_WAITING    一个或者多个任务在信号量中等待
*                            OS_ERR_EVENT_TYPE      没有传递指针到信号量
*                            OS_ERR_PEVENT_NULL      如果 'pevent'是一个空指针************
备注：删除信号量之前，必须首先删除操作该信号量的所有任务
*/

#if OS_SEM_DEL_EN > 0
OS_EVENT  *OSSemDel (OS_EVENT *pevent, INT8U opt, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    BOOLEAN    tasks_waiting;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_DEL_ISR;                             /* ... can't DELETE from an ISR             */
        return (pevent);
    }//是不是在ISR中调用
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        *err = OS_ERR_PEVENT_NULL;
        return (pevent);//非法的pevent
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_SEM) {        /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return (pevent);//不是事件块类型
    }
#endif
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0x00) {                      /* See if any tasks waiting on semaphore    */
		//如果有任务在等待
        tasks_waiting = TRUE;                              /* Yes                                      */
    } else {
        tasks_waiting = FALSE;                             /* No                                       */
    }
    switch (opt) {//删除选项
        case OS_DEL_NO_PEND:                               /* Delete semaphore only if no task waiting */
//如果只有没有任务等待的时候才删除
             if (tasks_waiting == FALSE) {//真没有任务等待
                 pevent->OSEventType = OS_EVENT_TYPE_UNUSED;//将事件控制块标记为未用，并退回到
                 //空余ECB中，此操作允许该事件控制块用于建立另一信号量
                 pevent->OSEventPtr  = OSEventFreeList;    /* Return Event Control Block to free list  */
				 //原空闲表头变成老二了。
                 OSEventFreeList     = pevent;             /* Get next free event control block        */
				 //pevent指向新的空闲表头
                 OS_EXIT_CRITICAL();
                 *err = OS_NO_ERR;//返回无错
                 return ((OS_EVENT *)0);                   /* Semaphore has been deleted               */
             } else {//如果有任务等待
                 OS_EXIT_CRITICAL();
                 *err = OS_ERR_TASK_WAITING;
                 return (pevent);
             }

        case OS_DEL_ALWAYS:                                /* Always delete the semaphore              */
			//无论如何都要删除
             while (pevent->OSEventGrp != 0x00) {          /* Ready ALL tasks waiting for semaphore    */
                 OS_EventTaskRdy(pevent, (void *)0, OS_STAT_SEM);
             }//使所有等待的任务就绪
             pevent->OSEventType = OS_EVENT_TYPE_UNUSED;//标记为未用
             pevent->OSEventPtr  = OSEventFreeList;        /* Return Event Control Block to free list  */
			 //原空闲表头变成老二了。
             OSEventFreeList     = pevent;                 /* Get next free event control block        */
             //pevent指向新的空闲表头
             OS_EXIT_CRITICAL();
             if (tasks_waiting == TRUE) {                  /* Reschedule only if task(s) were waiting  */
                 OS_Sched();                               /* Find highest priority task ready to run  */
				 //有任务等待的话，因为有任务就绪，所以要任务调度
             }
             *err = OS_NO_ERR;
             return ((OS_EVENT *)0);                       /* Semaphore has been deleted               */

        default://其它非法选项
             OS_EXIT_CRITICAL();
             *err = OS_ERR_INVALID_OPT;
             return (pevent);
    }
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                           PEND ON SEMAPHORE
*
* Description: This function waits for a semaphore.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired
*                            semaphore.
*
*              timeout       is an optional timeout period (in clock ticks).  If non-zero, your task will
*                            wait for the resource up to the amount of time specified by this argument.
*                            If you specify 0, however, your task will wait forever at the specified
*                            semaphore or, until the resource becomes available (or the event occurs).
*
*              err           is a pointer to where an error message will be deposited.  Possible error
*                            messages are:
*
*                            OS_NO_ERR           The call was successful and your task owns the resource
*                                                or, the event you are waiting for occurred.
*                            OS_TIMEOUT          The semaphore was not received within the specified
*                                                timeout.
*                            OS_ERR_EVENT_TYPE   If you didn't pass a pointer to a semaphore.
*                            OS_ERR_PEND_ISR     If you called this function from an ISR and the result
*                                                would lead to a suspension.
*                            OS_ERR_PEVENT_NULL  If 'pevent' is a NULL pointer.
*
* Returns    : none
                                                            等待一个信号量
描述：等待一个信号量
参数：pevent：指向事件控制块结合目标信号量的指针
                timeout：定时超时选项（以时钟节拍为单位），如果非零，你的任务将等待
                             //资源的时间值就是这个参数，如果为零，将永远等待直到资源变成可用（或者其它事件发生）
		  err			指向错误代码的消息指针
			   *							消息为：
			   *
			   *							OS_NO_ERR			调用成功，任务拥有资源或者目标事件发生
			   *							OS_TIMEOUT			规定时间内信号量没有发生
			   *							OS_ERR_EVENT_TYPE	如果没有传递指针到信号量
			   *							OS_ERR_PEND_ISR 	如果ISR调用此函数，结果将异常
			   *							OS_ERR_PEVENT_NULL	如果 'pevent' 是一个空指针

*********************************************************************************************************
*/

void  OSSemPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr;
#endif    


    if (OSIntNesting > 0) {                           /* See if called from ISR ...                    */
        *err = OS_ERR_PEND_ISR;                       /* ... can't PEND from an ISR                    */
        return;
    }//是不是在ISR中调用
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        *err = OS_ERR_PEVENT_NULL;
        return;
    }//非法的pevent
    if (pevent->OSEventType != OS_EVENT_TYPE_SEM) {   /* Validate event block type                     */
        *err = OS_ERR_EVENT_TYPE;
        return;
    }//不是事件块类型
#endif
    OS_ENTER_CRITICAL();
    if (pevent->OSEventCnt > 0) {                     /* If sem. is positive, resource available ...   */
        pevent->OSEventCnt--;                         /* ... decrement semaphore only if positive.     */
        OS_EXIT_CRITICAL();
        *err = OS_NO_ERR;
        return;
    }
//如果信号量有效，则信号量计数值递减，返回无错给调用它的函数，如果此
//函数用于等待一共享资源的信号量，那么通过返回此值，知道代码是否正确运行。
//如果信号量的计数值为0，则调用此函数的任务将进入睡眠状态，等待另一个任务
//或者ISR发出信号量。
                                                      /* Otherwise, must wait until event occurs       */
    OSTCBCur->OSTCBStat |= OS_STAT_SEM;               /* Resource not available, pend on semaphore     */
//资源不可用，挂起任务
    OSTCBCur->OSTCBDly   = timeout;                   /* Store pend timeout in TCB                     */
//保存挂起定时到TCB
    OS_EventTaskWait(pevent);                         /* Suspend task until event or timeout occurs    */
//使任务等待事件发生（将任务挂起）
    OS_EXIT_CRITICAL();
    OS_Sched();                                       /* Find next highest priority task ready         */
	//由于得不到信号量，当前任务不再处于就绪状态，任务调度，让下一个
	//优先级最高的任务运行。这样，调用此函数的任务被挂起，直到信号量出现，
	//才能继续运行。
    OS_ENTER_CRITICAL();
    if (OSTCBCur->OSTCBStat & OS_STAT_SEM) {          /* Must have timed out if still waiting for event*/
//再次检查任务控制块中的状态标志，是否仍处于等待信号量的状态，如果是，
//则说明该任务并没有被此函数发出的信号量唤醒；而实际上该任务是因为等待
//超时，而被TimeTick()函数置为就绪态。
        OS_EventTO(pevent);//等待事件超时，将任务从等待列表中删除，让任务继续运行。
        OS_EXIT_CRITICAL();
        *err = OS_TIMEOUT;     /* Indicate that didn't get event within TO      *///返回超时错误代码
        return;//超时了
    }
    OSTCBCur->OSTCBEventPtr = (OS_EVENT *)0;//将指向信号量ECB的指针从该任务的任务控制块中删除
    OS_EXIT_CRITICAL();
    *err = OS_NO_ERR;
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                         POST TO A SEMAPHORE
*
* Description: This function signals a semaphore
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired
*                            semaphore.
*
* Returns    : OS_NO_ERR           The call was successful and the semaphore was signaled.
*              OS_SEM_OVF          If the semaphore count exceeded its limit.  In other words, you have
*                                  signalled the semaphore more often than you waited on it with either
*                                  OSSemAccept() or OSSemPend().
*              OS_ERR_EVENT_TYPE   If you didn't pass a pointer to a semaphore
*              OS_ERR_PEVENT_NULL  If 'pevent' is a NULL pointer.
                                                      发出一个信号量
描述：发出一个信号量
参数：pevent ：指向事件控制块结合目标信号量的指针
返回：OS_NO_ERR           调用成功，信号量发出
*              OS_SEM_OVF       信号量数目超出范围. 也就是说你发送的信号量多于在either
*                                  OSSemAccept() or OSSemPend()中等待的 
*              OS_ERR_EVENT_TYPE   没有传递指针给信号量
*              OS_ERR_PEVENT_NULL  如果 'pevent' 是空指针
*********************************************************************************************************
*/

INT8U  OSSemPost (OS_EVENT *pevent)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;                               
#endif    


#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        return (OS_ERR_PEVENT_NULL);
    }//非法的pevent
    if (pevent->OSEventType != OS_EVENT_TYPE_SEM) {        /* Validate event block type                */
        return (OS_ERR_EVENT_TYPE);
    }//不是事件块类型
#endif
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0x00) {                      /* See if any task waiting for semaphore    */
		//是不是有任务等待
        OS_EventTaskRdy(pevent, (void *)0, OS_STAT_SEM);   /* Ready highest prio task waiting on event */
		//就绪最高优先级的等待任务
        OS_EXIT_CRITICAL();
        OS_Sched(); /* Find highest priority task ready to run  *///任务调度，找最高优先级的任务运行                                       
        return (OS_NO_ERR);
    }
    if (pevent->OSEventCnt < 65535) {                 /* Make sure semaphore will not overflow         */
		//保证信号量没有溢出
        pevent->OSEventCnt++;                         /* Increment semaphore count to register event   */
		//更新信号量数目
        OS_EXIT_CRITICAL();
        return (OS_NO_ERR);
    }
    OS_EXIT_CRITICAL();    /* Semaphore value has reached its maximum   *///信号量满了。
    return (OS_SEM_OVF);
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                          QUERY A SEMAPHORE
*
* Description: This function obtains information about a semaphore
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired
*                            semaphore
*
*              pdata         is a pointer to a structure that will contain information about the
*                            semaphore.
*
* Returns    : OS_NO_ERR           The call was successful and the message was sent
*              OS_ERR_EVENT_TYPE   If you are attempting to obtain data from a non semaphore.
*              OS_ERR_PEVENT_NULL  If 'pevent' is a NULL pointer.
                                                        查询一个信号量的当前状态
描述：获得一个信号量的信息
参数：pevent：指向事件控制块结合目标信号量的指针
                pdata：指向包含信号量信息的结构指针
返回：OS_NO_ERR           调用成功，消息发送
*              OS_ERR_EVENT_TYPE   如果想从非信号量获得数据
*              OS_ERR_PEVENT_NULL  如果 'pevent' 是空指针
*********************************************************************************************************
*/

#if OS_SEM_QUERY_EN > 0
INT8U  OSSemQuery (OS_EVENT *pevent, OS_SEM_DATA *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    INT8U     *psrc;
    INT8U     *pdest;


#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        return (OS_ERR_PEVENT_NULL);
    }//非法的pevent
    if (pevent->OSEventType != OS_EVENT_TYPE_SEM) {        /* Validate event block type                */
        return (OS_ERR_EVENT_TYPE);
    }//不是事件块类型
#endif
    OS_ENTER_CRITICAL();
    pdata->OSEventGrp = pevent->OSEventGrp;                /* Copy message mailbox wait list           */
	//将OS_EVENT结构中的等待任务列表复制到OS_SEM_DATA数据结构中，
	//此处使用条件编译不不用循环语句，目的是使程序运行得更愉
    psrc              = &pevent->OSEventTbl[0];
    pdest             = &pdata->OSEventTbl[0];//指向表头
#if OS_EVENT_TBL_SIZE > 0
    *pdest++          = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 1
    *pdest++          = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 2
    *pdest++          = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 3
    *pdest++          = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 4
    *pdest++          = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 5
    *pdest++          = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 6
    *pdest++          = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 7
    *pdest            = *psrc;
#endif
    pdata->OSCnt      = pevent->OSEventCnt; /* Get semaphore count       */
//获得信号量数。
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif                                                     /* OS_SEM_QUERY_EN                          */
#endif                                                     /* OS_SEM_EN                                */
