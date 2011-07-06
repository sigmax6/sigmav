/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                        MESSAGE QUEUE MANAGEMENT
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File : OS_Q.C
* By   : Jean J. Labrosse
* 翻译： likee
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE//防止编译器包含一些代码
#include "includes.h"
#endif

#if (OS_Q_EN > 0) && (OS_MAX_QS > 0)
/*
*********************************************************************************************************
*                                      ACCEPT MESSAGE FROM QUEUE
*
* Description: This function checks the queue to see if a message is available.  Unlike OSQPend(),
*              OSQAccept() does not suspend the calling task if a message is not available.
*
* Arguments  : pevent        is a pointer to the event control block
*
* Returns    : != (void *)0  is the message in the queue if one is available.  The message is removed
*                            from the so the next time OSQAccept() is called, the queue will contain
*                            one less entry.
*              == (void *)0  if the queue is empty or,
*                            if 'pevent' is a NULL pointer or,
*                            if you passed an invalid event type

*********************************************************************************************************
*/

#if OS_Q_ACCEPT_EN > 0
void  *OSQAccept (OS_EVENT *pevent)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif
    void      *msg;
    OS_Q      *pq;


#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {               /* Validate 'pevent'                                  */
        return ((void *)0);
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) {/* Validate event block type                          */
        return ((void *)0);
    }
#endif
    OS_ENTER_CRITICAL();
    pq = (OS_Q *)pevent->OSEventPtr;             /* Point at queue control block                       */
    if (pq->OSQEntries > 0) {                    /* See if any messages in the queue                   */
        msg = *pq->OSQOut++;                     /* Yes, extract oldest message from the queue         */
        pq->OSQEntries--;                        /* Update the number of entries in the queue          */
        if (pq->OSQOut == pq->OSQEnd) {          /* Wrap OUT pointer if we are at the end of the queue */
            pq->OSQOut = pq->OSQStart;
        }
    } else {
        msg = (void *)0;                         /* Queue is empty                                     */
    }
    OS_EXIT_CRITICAL();
    return (msg);                                /* Return message received (or NULL)                  */
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                        CREATE A MESSAGE QUEUE
*
* Description: This function creates a message queue if free event control blocks are available.
*
* Arguments  : start         is a pointer to the base address of the message queue storage area.  The
*                            storage area MUST be declared as an array of pointers to 'void' as follows
*
*                            void *MessageStorage[size]
*
*              size          is the number of elements in the storage area
*
* Returns    : != (OS_EVENT *)0  is a pointer to the event control clock (OS_EVENT) associated with the
*                                created queue
*              == (OS_EVENT *)0  if no event control blocks were available or an error was detected
                                                      建立一个消息队列
描述：如果有空余事件控制块，就建立一个消息队列
参数：start：指向消息队列存储空间基地址。存储空间必须定义成void 型的
                            一系列指针，形式如右：void *MessageStorage[size]
                 size：存储空间内单元的数目
返回：!= (OS_EVENT *)0  是指向结合建立的队列的事件控制时钟(OS_EVENT)的指针
                 == (OS_EVENT *)0如果没有适合的事件控制块或者有错误
*********************************************************************************************************
*/

OS_EVENT  *OSQCreate (void **start, INT16U size)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;//分配名为cpu_sr 的局部变量，用于支持OS_CRITICAL_METHOD方法3
#endif
    OS_EVENT  *pevent;
    OS_Q      *pq;


    if (OSIntNesting > 0) {                      /* See if called from ISR ...                         */
        return ((OS_EVENT *)0);                  /* ... can't CREATE from an ISR                       */
    }//不能在ISR中建立消息队列
    OS_ENTER_CRITICAL();
    pevent = OSEventFreeList;                    /* Get next free event control block                  */
	//从空余的ECB链表中取得一个事件控制块。对剩下的作相应调整
	//ECB是单向链表
    if (OSEventFreeList != (OS_EVENT *)0) {      /* See if pool of free ECB pool was empty             */
        OSEventFreeList = (OS_EVENT *)OSEventFreeList->OSEventPtr;
    }//对剩下的进行相应的调整
    OS_EXIT_CRITICAL();
    if (pevent != (OS_EVENT *)0) {               /* See if we have an event control block              */
		//如果我们得到了事件控制块
        OS_ENTER_CRITICAL();
        pq = OSQFreeList;                        /* Get a free queue control block                     */
		//得到一块新的队列控制块
        if (pq != (OS_Q *)0) {                   /* Were we able to get a queue control block ?        */
			//如果得到的不为空，即真正得到了。
            OSQFreeList         = OSQFreeList->OSQPtr;    /* Yes, Adjust free list pointer to next free*/
			//调整表头
            OS_EXIT_CRITICAL();
			//初始化新得到的队列控制块
            pq->OSQStart        = start;                  /*      Initialize the queue                 */
            pq->OSQEnd          = &start[size];
            pq->OSQIn           = start;
            pq->OSQOut          = start;
            pq->OSQSize         = size;
            pq->OSQEntries      = 0;//初始化消息数为零
            pevent->OSEventType = OS_EVENT_TYPE_Q;//设置事件控制块类型
            pevent->OSEventCnt  = 0;//信号量
            pevent->OSEventPtr  = pq;//将此块指向消息队列结构指针
            OS_EventWaitListInit(pevent);   /*      Initalize the wait list  *///初始化等待列表
        } else {//如果没有得到队列控制块
            pevent->OSEventPtr = (void *)OSEventFreeList; /* No,  Return event control block on error  */
			//以错误形式返回队列控制块
            OSEventFreeList    = pevent;//退回事件控制块，不用了。
            OS_EXIT_CRITICAL();
            pevent = (OS_EVENT *)0;//清零，给下一次使用。
        }
    }
    return (pevent);//如果消息队列建立成功，则会返回一个指针，否则，就返回空指针。
    //消息队列成功后返回的这个指针用于以后对消息队列的操作因此，该指针
    //可以看作是相应消息队列的句柄。
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                        DELETE A MESSAGE QUEUE
*
* Description: This function deletes a message queue and readies all tasks pending on the queue.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired
*                            queue.
*
*              opt           determines delete options as follows:
*                            opt == OS_DEL_NO_PEND   Delete the queue ONLY if no task pending
*                            opt == OS_DEL_ALWAYS    Deletes the queue even if tasks are waiting.
*                                                    In this case, all the tasks pending will be readied.
*
*              err           is a pointer to an error code that can contain one of the following values:
*                            OS_NO_ERR               The call was successful and the queue was deleted
*                            OS_ERR_DEL_ISR          If you tried to delete the queue from an ISR
*                            OS_ERR_INVALID_OPT      An invalid option was specified
*                            OS_ERR_TASK_WAITING     One or more tasks were waiting on the queue
*                            OS_ERR_EVENT_TYPE       If you didn't pass a pointer to a queue
*                            OS_ERR_PEVENT_NULL      If 'pevent' is a NULL pointer.
*
* Returns    : pevent        upon error
*              (OS_EVENT *)0 if the queue was successfully deleted.
*
* Note(s)    : 1) This function must be used with care.  Tasks that would normally expect the presence of
*                 the queue MUST check the return code of OSQPend().
*              2) OSQAccept() callers will not know that the intended queue has been deleted unless
*                 they check 'pevent' to see that it's a NULL pointer.
*              3) This call can potentially disable interrupts for a long time.  The interrupt disable
*                 time is directly proportional to the number of tasks waiting on the queue.
*              4) Because ALL tasks pending on the queue will be readied, you MUST be careful in
*                 applications where the queue is used for mutual exclusion because the resource(s)
*                 will no longer be guarded by the queue.
*              5) If the storage for the message queue was allocated dynamically (i.e. using a malloc()
*                 type call) then your application MUST release the memory storage by call the counterpart
*                 call of the dynamic allocation scheme used.  If the queue storage was created statically
*                 then, the storage can be reused.
                                                  删除一个消息队列
描述：删除一个队列，使在队列上挂起的任务全部就绪
参数：pevent ：指向事件控制块和目标队列的指针
                 opt:决定删除选项，如下：
*                            opt == OS_DEL_NO_PEND   没有任务挂起才删除
*                            opt == OS_DEL_ALWAYS    有任务挂起也删，挂起的任务全部就绪
*              err          指向包含如下错误信息的指针
*                            OS_NO_ERR              调用成功，队列删除
*                            OS_ERR_DEL_ISR         如果想从ISR中删除
*                            OS_ERR_INVALID_OPT      指定了非法选项
*                            OS_ERR_TASK_WAITING     有任务在队列中等待
*                            OS_ERR_EVENT_TYPE      如果你没有传递消息给队列
*                            OS_ERR_PEVENT_NULL     如果pevent是一个空指针
返回：pevent；有错
                 (OS_EVENT *)0如果队列成功删除
备注：1、此函数要小心使用，任务希望现场队列检查OSQPend()的返回代码，什么意思，不知道
                2、OSQAccept()的调用者不知道目标队列是否删除了，除非检查pevent'是否为空指针
                3、此调用将潜在关中断一段时间，时间长短与队列中任务多少成正比
                4、因为所有队列中挂起的任务就绪，在多任务的时候要小心，因为这些资源
                       队列不再看管（因为删除了）。
                5、如果消息存储用动态分配（比如用malloc()），那么应用程序必须通过
                        调用相应的动态配置去释放内存空间，如果队列存储是静态建立，则可以再次
                利用
*********************************************************************************************************
*/

#if OS_Q_DEL_EN > 0
OS_EVENT  *OSQDel (OS_EVENT *pevent, INT8U opt, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    BOOLEAN    tasks_waiting;
    OS_Q      *pq;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_DEL_ISR;                             /* ... can't DELETE from an ISR             */
        return ((OS_EVENT *)0);
    }//不能在中断服务程序中删除
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        *err = OS_ERR_PEVENT_NULL;
        return (pevent);//pevent不合理
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) {          /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return (pevent);//非法的事件块模式
    }
#endif
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0x00) {                      /* See if any tasks waiting on queue        */
		//如果有任务在队列中等待
        tasks_waiting = TRUE;                              /* Yes                                      */
    } else {
        tasks_waiting = FALSE;                             /* No                                       */
    }
    switch (opt) {
        case OS_DEL_NO_PEND:                               /* Delete queue only if no task waiting     */
			//如果只在无任务等待的情况下删除
             if (tasks_waiting == FALSE) {//无任务等待
                 pq                  = (OS_Q *)pevent->OSEventPtr;  /* Return OS_Q to free list        */
                 pq->OSQPtr          = OSQFreeList;
                 OSQFreeList         = pq;//将它加入了空闲链表
                 pevent->OSEventType = OS_EVENT_TYPE_UNUSED;//标记未用
                 pevent->OSEventPtr  = OSEventFreeList;    /* Return Event Control Block to free list  */
                 OSEventFreeList     = pevent; /* Get next free event control block        *///将事件控制块放入空闲链表中
                 OS_EXIT_CRITICAL();
                 *err = OS_NO_ERR;//无错
                 return ((OS_EVENT *)0);                   /* Queue has been deleted                   */
             } else {//有任务等待
                 OS_EXIT_CRITICAL();
                 *err = OS_ERR_TASK_WAITING;
                 return (pevent);
             }

        case OS_DEL_ALWAYS:                                /* Always delete the queue                  */
			//始终要删除队列
             while (pevent->OSEventGrp != 0x00) {          /* Ready ALL tasks waiting for queue        */
			 	//真有任务等待
                 OS_EventTaskRdy(pevent, (void *)0, OS_STAT_Q);//将挂起的任务就绪
             }
             pq                  = (OS_Q *)pevent->OSEventPtr;      /* Return OS_Q to free list        */
             pq->OSQPtr          = OSQFreeList;
             OSQFreeList         = pq;//同上，将队列控制块放入空闲链表
             pevent->OSEventType = OS_EVENT_TYPE_UNUSED;//标记为未用
             pevent->OSEventPtr  = OSEventFreeList;        /* Return Event Control Block to free list  */
             OSEventFreeList     = pevent;                 /* Get next free event control block        */
			 //将事件控制块放入空闲链表
             OS_EXIT_CRITICAL();
             if (tasks_waiting == TRUE) {                  /* Reschedule only if task(s) were waiting  */
                 OS_Sched();                               /* Find highest priority task ready to run  */
				 //因为新任务就绪，所以要重新调度
             }
             *err = OS_NO_ERR;
             return ((OS_EVENT *)0);                       /* Queue has been deleted                   */

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
*                                           FLUSH QUEUE
*
* Description : This function is used to flush the contents of the message queue.
*
* Arguments   : none
*
* Returns     : OS_NO_ERR           upon success
*               OS_ERR_EVENT_TYPE   If you didn't pass a pointer to a queue
*               OS_ERR_PEVENT_NULL  If 'pevent' is a NULL pointer
                                                   清空消息队列
描述：清空消息队列中内容
参数：无
返回：
                 OS_NO_ERR          成功
*               OS_ERR_EVENT_TYPE   没有传递指针给队列
*               OS_ERR_PEVENT_NULL  如果'pevent' 是一个空指针

*********************************************************************************************************
*/

#if OS_Q_FLUSH_EN > 0
INT8U  OSQFlush (OS_EVENT *pevent)
{
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr;
#endif
    OS_Q      *pq;


#if OS_ARG_CHK_EN > 0//允许参数检验
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        return (OS_ERR_PEVENT_NULL);
    }//不合理的参数pevent
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) {     /* Validate event block type                     */
        return (OS_ERR_EVENT_TYPE);
    }//不是事件块类型
#endif
    OS_ENTER_CRITICAL();
    pq             = (OS_Q *)pevent->OSEventPtr;      /* Point to queue storage structure              */
	//保存结构指针
    pq->OSQIn      = pq->OSQStart;
    pq->OSQOut     = pq->OSQStart;//将队列的插入指针IN和取出指针OUT复位
    pq->OSQEntries = 0;//初始化入口为零
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                     PEND ON A QUEUE FOR A MESSAGE
*
* Description: This function waits for a message to be sent to a queue
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired queue
*
*              timeout       is an optional timeout period (in clock ticks).  If non-zero, your task will
*                            wait for a message to arrive at the queue up to the amount of time
*                            specified by this argument.  If you specify 0, however, your task will wait
*                            forever at the specified queue or, until a message arrives.
*
*              err           is a pointer to where an error message will be deposited.  Possible error
*                            messages are:
*
*                            OS_NO_ERR           The call was successful and your task received a
*                                                message.
*                            OS_TIMEOUT          A message was not received within the specified timeout
*                            OS_ERR_EVENT_TYPE   You didn't pass a pointer to a queue
*                            OS_ERR_PEVENT_NULL  If 'pevent' is a NULL pointer
*                            OS_ERR_PEND_ISR     If you called this function from an ISR and the result
*                                                would lead to a suspension.
*
* Returns    : != (void *)0  is a pointer to the message received
*              == (void *)0  if no message was received or,
*                            if 'pevent' is a NULL pointer or,
*                            if you didn't pass a pointer to a queue.
                                                 等待消息队列中的消息
描述：等待消息中的队列
参数：pevent：指向事件控制块结合目标队列的指针
                timeout：超时时间选项（按时钟节拍来），如果非零，任务将按照此
                             参数的定时在队列中等待消息到来，如果设置为零，任务将在目标
                             队列中永远等待，直到消息到来。
                err：指向可能的错误消息的指针，可能为：
*                            OS_NO_ERR        调用成功，任务接收到消息
*                            OS_TIMEOUT         定时时间内消息没有来
*                            OS_ERR_EVENT_TYPE   你没有传递指针到队列
*                            OS_ERR_PEVENT_NULL  如果 'pevent' 是一个空指针
*                            OS_ERR_PEND_ISR    如果从ISR中调用，结果将出现异常              
*********************************************************************************************************
*/

void  *OSQPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif
    void      *msg;
    OS_Q      *pq;


    if (OSIntNesting > 0) {                      /* See if called from ISR ...                         */
        *err = OS_ERR_PEND_ISR;                  /* ... can't PEND from an ISR                         */
        return ((void *)0);//不能在ISR中挂起
    }
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {               /* Validate 'pevent'                                  */
        *err = OS_ERR_PEVENT_NULL;
        return ((void *)0);//不合理的pevent
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) {/* Validate event block type                          */
        *err = OS_ERR_EVENT_TYPE;
        return ((void *)0);//不是事件块类型
    }
#endif
    OS_ENTER_CRITICAL();
    pq = (OS_Q *)pevent->OSEventPtr;             /* Point at queue control block                       */
	//取出队列控制块指针
    if (pq->OSQEntries > 0) {                    /* See if any messages in the queue                   */
//大于零，表示有消息可以用。此时，OSQPend得到事件控制块的.OSQOut域所指
//向的消息，将指向消息的指针复制到msg变量中，并让.OSQOut指针指向
//队列中的下个单元
        msg = *pq->OSQOut++;                     /* Yes, extract oldest message from the queue         */
        pq->OSQEntries--;                        /* Update the number of entries in the queue          */
// 队列中有效消息减一
        if (pq->OSQOut == pq->OSQEnd) {          /* Wrap OUT pointer if we are at the end of the queue */
            pq->OSQOut = pq->OSQStart;
        }//消息队列是一个循环缓冲区，如果超出了队列中最末一个单元，发生
        //这种越界时，就要将.OSQOut重新调整到指向队列的起始单元
        OS_EXIT_CRITICAL();
        *err = OS_NO_ERR;
        return (msg);  /* Return message received      *///返回接收到的消息
    }
    OSTCBCur->OSTCBStat |= OS_STAT_Q;            /* Task will have to pend for a message to be posted  */
	//设置任务的TCB状态标志，以表明等待消息队列消息的任务被挂起
    OSTCBCur->OSTCBDly   = timeout;              /* Load timeout into TCB                              */
	//装载定时器到TCB
    OS_EventTaskWait(pevent);                    /* Suspend task until event or timeout occurs         */
	//挂起任务直到消息到来或者超时。在用户进程中，调用此函数的任务
	//并不知道消息没有到来之前自己被挂起，队列接收到一则消息或者超时
	//时，此函数就会调用调度函数恢复运行。
    OS_EXIT_CRITICAL();
    OS_Sched();                                  /* Find next highest priority task ready to run       */
	//挂起后要进行任务调度。
    OS_ENTER_CRITICAL();
    msg = OSTCBCur->OSTCBMsg;//任务调度后，任务会检查此函数是不是将消息放到了
    //任务的TCB中。
    if (msg != (void *)0) {                      /* Did we get a message?                              */
		//如果消息确实存在。
        OSTCBCur->OSTCBMsg      = (void *)0;     /* Extract message from TCB (Put there by QPost)      */
		//清除此消息？
        OSTCBCur->OSTCBStat     = OS_STAT_RDY;//就绪
        OSTCBCur->OSTCBEventPtr = (OS_EVENT *)0; /* No longer waiting for event                        */
		//不再等待事件了
        OS_EXIT_CRITICAL();
        *err                    = OS_NO_ERR;
        return (msg);                            /* Return message received                            */
    }
    OS_EventTO(pevent);                          /* Timed out                                          */
	//超时了。
    OS_EXIT_CRITICAL();
    *err = OS_TIMEOUT;                           /* Indicate a timeout occured       */
    return ((void *)0);                          /* No message received      */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                        POST MESSAGE TO A QUEUE
*
* Description: This function sends a message to a queue
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired queue
*
*              msg           is a pointer to the message to send.  You MUST NOT send a NULL pointer.
*
* Returns    : OS_NO_ERR             The call was successful and the message was sent
*              OS_Q_FULL             If the queue cannot accept any more messages because it is full.
*              OS_ERR_EVENT_TYPE     If you didn't pass a pointer to a queue.
*              OS_ERR_PEVENT_NULL    If 'pevent' is a NULL pointer
*              OS_ERR_POST_NULL_PTR  If you are attempting to post a NULL pointer
                                             向消息队列发送一则消息（FIFO）
描述：发送一则消息到队列
参数：pevent：指向事件控制块联合目标队列的指针
                msg：指向要发送的消息。不能发送NULL
返回：OS_NO_ERR            消息成功发送
*              OS_Q_FULL             如果队列满了，不能接收消息了
*              OS_ERR_EVENT_TYPE     如果没有发送消息到队列
*              OS_ERR_PEVENT_NULL    如果 'pevent' 是空指针
*              OS_ERR_POST_NULL_PTR  如果你发送空消息
*********************************************************************************************************
*/

#if OS_Q_POST_EN > 0
INT8U  OSQPost (OS_EVENT *pevent, void *msg)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif
    OS_Q      *pq;


#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        return (OS_ERR_PEVENT_NULL);
    }//不合理的pevent
    if (msg == (void *)0) {                           /* Make sure we are not posting a NULL pointer   */
        return (OS_ERR_POST_NULL_PTR);
    }//保证没有发送空指针
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) {     /* Validate event block type                     */
        return (OS_ERR_EVENT_TYPE);
    }//非法的事件块类型
#endif
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0x00) {                 /* See if any task pending on queue              */
//如果有任务在等
        OS_EventTaskRdy(pevent, msg, OS_STAT_Q);      /* Ready highest priority task waiting on event  */
//使在事件中等待的最高优先级任务就绪
        OS_EXIT_CRITICAL();
        OS_Sched();                                   /* Find highest priority task ready to run       */
		//任务调度
        return (OS_NO_ERR);
    }
    pq = (OS_Q *)pevent->OSEventPtr;                  /* Point to queue control block                  */
    if (pq->OSQEntries >= pq->OSQSize) {              /* Make sure queue is not full                   */
        OS_EXIT_CRITICAL();
        return (OS_Q_FULL);
    }
    *pq->OSQIn++ = msg;                               /* Insert message into queue                     */
	//将消息插入
    pq->OSQEntries++;                                 /* Update the nbr of entries in the queue        */
	//更新总消息数
    if (pq->OSQIn == pq->OSQEnd) {                    /* Wrap IN ptr if we are at end of queue         */
        pq->OSQIn = pq->OSQStart;//调节消息队列循环缓冲区，当越界时，重新指向新的开头
    }
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                   POST MESSAGE TO THE FRONT OF A QUEUE
*
* Description: This function sends a message to a queue but unlike OSQPost(), the message is posted at
*              the front instead of the end of the queue.  Using OSQPostFront() allows you to send
*              'priority' messages.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired queue
*
*              msg           is a pointer to the message to send.  You MUST NOT send a NULL pointer.
*
* Returns    : OS_NO_ERR             The call was successful and the message was sent
*              OS_Q_FULL             If the queue cannot accept any more messages because it is full.
*              OS_ERR_EVENT_TYPE     If you didn't pass a pointer to a queue.
*              OS_ERR_PEVENT_NULL    If 'pevent' is a NULL pointer
*              OS_ERR_POST_NULL_PTR  If you are attempting to post to a non queue.
                             向消息队列发送一则消息（LIFO）
描述：发送一则消息，但与OSQPost()不同，消息是指向前端而不是队列的后端，
                //用此函数允许你发送优先级消息。
参数：pevent：指向事件控制块联合目标队列的指针
                smg:指向你要发送的消息，不能发送空指针
返回：OS_NO_ERR             调用成功，消息成功发送
*              OS_Q_FULL             如果队列满了，不能接收消息了
*              OS_ERR_EVENT_TYPE     如果没有发送消息到队列
*              OS_ERR_PEVENT_NULL   如果 'pevent' 是空指针
*              OS_ERR_POST_NULL_PTR  如果你想发送到的不是队列
*********************************************************************************************************
*/

#if OS_Q_POST_FRONT_EN > 0
INT8U  OSQPostFront (OS_EVENT *pevent, void *msg)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif
    OS_Q      *pq;


#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        return (OS_ERR_PEVENT_NULL);
    }}//不合理的pevent
    if (msg == (void *)0) {                           /* Make sure we are not posting a NULL pointer   */
        return (OS_ERR_POST_NULL_PTR);
    }//保证没有发送空指针
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) {     /* Validate event block type                     */
        return (OS_ERR_EVENT_TYPE);
    }//非法的事件块类型
#endif
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0x00) {                 /* See if any task pending on queue              */
        OS_EventTaskRdy(pevent, msg, OS_STAT_Q);      /* Ready highest priority task waiting on event  */
        OS_EXIT_CRITICAL();
        OS_Sched();                                   /* Find highest priority task ready to run       */
        return (OS_NO_ERR);//解释见上个函数
    }
    pq = (OS_Q *)pevent->OSEventPtr;                  /* Point to queue control block                  */
	//装载队列控制块指针
    if (pq->OSQEntries >= pq->OSQSize) {  /* Make sure queue is not full                   */
		//保证队列还没有满
        OS_EXIT_CRITICAL();
        return (OS_Q_FULL);
    }//OSQOut指针指向的是已经插入了消息指针的单元，所以在插入新的消息指针前，
    //要将OSQOut指针在消息队列中前移一个单元，
    if (pq->OSQOut == pq->OSQStart) {                 /* Wrap OUT ptr if we are at the 1st queue entry */
        pq->OSQOut = pq->OSQEnd;//更新循环缓冲链。如果pq->OSQOut指向的当前单无是队
        //列中的第一个单元，再向前移的话就意味着指向了队列的末尾
    }
    pq->OSQOut--;//OSQEnd指向的是消息队列中最后一个单元的下一个单元，所以，
    //要求将其调整到指向队列的有效范围内。因为OSQPend（）函数取出的消息
    //是由此函数刚刚插入的，这样，就实现了后进先出。
    *pq->OSQOut = msg;                                /* Insert message into queue                     */
    pq->OSQEntries++;                                 /* Update the nbr of entries in the queue        */
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                        POST MESSAGE TO A QUEUE
*
* Description: This function sends a message to a queue.  This call has been added to reduce code size
*              since it can replace both OSQPost() and OSQPostFront().  Also, this function adds the
*              capability to broadcast a message to ALL tasks waiting on the message queue.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired queue
*
*              msg           is a pointer to the message to send.  You MUST NOT send a NULL pointer.
*
*              opt           determines the type of POST performed:
*                            OS_POST_OPT_NONE         POST to a single waiting task
*                                                     (Identical to OSQPost())
*                            OS_POST_OPT_BROADCAST    POST to ALL tasks that are waiting on the queue
*                            OS_POST_OPT_FRONT        POST as LIFO (Simulates OSQPostFront())
*
*                            Below is a list of ALL the possible combination of these flags:
*
*                                 1) OS_POST_OPT_NONE
*                                    identical to OSQPost()
*
*                                 2) OS_POST_OPT_FRONT
*                                    identical to OSQPostFront()
*
*                                 3) OS_POST_OPT_BROADCAST
*                                    identical to OSQPost() but will broadcast 'msg' to ALL waiting tasks
*
*                                 4) OS_POST_OPT_FRONT + OS_POST_OPT_BROADCAST  is identical to
*                                    OSQPostFront() except that will broadcast 'msg' to ALL waiting tasks
*
* Returns    : OS_NO_ERR             The call was successful and the message was sent
*              OS_Q_FULL             If the queue cannot accept any more messages because it is full.
*              OS_ERR_EVENT_TYPE     If you didn't pass a pointer to a queue.
*              OS_ERR_PEVENT_NULL    If 'pevent' is a NULL pointer
*              OS_ERR_POST_NULL_PTR  If you are attempting to post a NULL pointer
*
* Warning    : Interrupts can be disabled for a long time if you do a 'broadcast'.  In fact, the
*              interrupt disable time is proportional to the number of tasks waiting on the queue.
                              向消息队列发送一则消息（LIFO或者FIFO）   
描述：以可以替代的，更灵活的方式给消息队列发消息，可以代替LIFO或者FIFO
               允许发送消息给所有消息队列中等待消息的任务（广播方式）
参数：pevent：指向事件控制块联合目标队列的指针
                smg:指向你要发送的消息，不能发送空指针
                opt:确定发送方式：
*                            OS_POST_OPT_NONE        发送给单个任务，相当于OSQPost()
*                            OS_POST_OPT_BROADCAST   发送给队列中所有等待的任务
*                            OS_POST_OPT_FRONT        以LIFO 发送(与 OSQPostFront()相似)
                 下面是所有标志可能的结合情况：  
*                                 1) OS_POST_OPT_NONE
*                                    相当于 OSQPost()
*
*                                 2) OS_POST_OPT_FRONT
*                                    相当于 OSQPostFront()
*
*                                 3) OS_POST_OPT_BROADCAST
*                                    相当于 OSQPost() 但可以以广播方式发送 to ALL waiting tasks
*
*                                 4) OS_POST_OPT_FRONT + OS_POST_OPT_BROADCAST  相当于
*                                    OSQPostFront() 可以以广播方式发送
*********************************************************************************************************
*/

#if OS_Q_POST_OPT_EN > 0
INT8U  OSQPostOpt (OS_EVENT *pevent, void *msg, INT8U opt)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif
    OS_Q      *pq;


#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        return (OS_ERR_PEVENT_NULL);
    }//非法pevent 
    if (msg == (void *)0) {                           /* Make sure we are not posting a NULL pointer   */
        return (OS_ERR_POST_NULL_PTR);
    }//保证没有送空指针
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) {     /* Validate event block type                     */
        return (OS_ERR_EVENT_TYPE);
    }//不是事件块类型
#endif
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0x00) {                 /* See if any task pending on queue              */
		//如果有任务在队列中挂起
        if ((opt & OS_POST_OPT_BROADCAST) != 0x00) {  /* Do we need to post msg to ALL waiting tasks ? */
			//向所有任务发送？
            while (pevent->OSEventGrp != 0x00) {      /* Yes, Post to ALL tasks waiting on queue       */
                OS_EventTaskRdy(pevent, msg, OS_STAT_Q);
            }//是的。如果将opt参数中的OS_POST_OPT_BROADCAST位置为1，则所有正在等待消息
            //的任务都能接收到这则消息，并且被OS_EventTaskRdy从等待列表中删除。
        } else {//不要向所有的等待任务发消息
            OS_EventTaskRdy(pevent, msg, OS_STAT_Q);  /* No,  Post to HPT waiting on queue             */
			//那就只发优先级最高的。
        }
        OS_EXIT_CRITICAL();
        OS_Sched();                                   /* Find highest priority task ready to run       */
		//要任务调度
        return (OS_NO_ERR);
    }
    pq = (OS_Q *)pevent->OSEventPtr;                  /* Point to queue control block                  */
	//指向队列控制块
    if (pq->OSQEntries >= pq->OSQSize) {              /* Make sure queue is not full                   */
        OS_EXIT_CRITICAL();//如果满了，返回错误代码
        return (OS_Q_FULL);
    }
    if ((opt & OS_POST_OPT_FRONT) != 0x00) {          /* Do we post to the FRONT of the queue?         */
		//FIFO还是LIFO，如果是LIFO
        if (pq->OSQOut == pq->OSQStart) {             /* Yes, Post as LIFO, Wrap OUT pointer if we ... */
            pq->OSQOut = pq->OSQEnd;                  /*      ... are at the 1st queue entry           */
        }//如果是后进先出，则相当于OSQPostFront 。如果我人在队列第一个，则调整
        pq->OSQOut--;
        *pq->OSQOut = msg;                            /*      Insert message into queue                */
		//插入消息
    } else {                                          /* No,  Post as FIFO                             */
    //如果是FIFO
        *pq->OSQIn++ = msg;                           /*      Insert message into queue                */
        if (pq->OSQIn == pq->OSQEnd) {                /*      Wrap IN ptr if we are at end of queue    */
            pq->OSQIn = pq->OSQStart;//如果在最末尾，则调整
        }
    }
    pq->OSQEntries++;                                 /* Update the nbr of entries in the queue        */
	//更新消息条数
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                        QUERY A MESSAGE QUEUE
*
* Description: This function obtains information about a message queue.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired queue
*
*              pdata         is a pointer to a structure that will contain information about the message
*                            queue.
*
* Returns    : OS_NO_ERR           The call was successful and the message was sent
*              OS_ERR_EVENT_TYPE   If you are attempting to obtain data from a non queue.
*              OS_ERR_PEVENT_NULL  If 'pevent' is a NULL pointer
                                       获取消息队列状态
描述：获取消息队列的信息
参数：pevent：指向事件控制块结合目标队列的指针
                pdata：指向消息队列包含信息的结构指针
返回：OS_NO_ERR           调用成功，消息成功发送
*              OS_ERR_EVENT_TYPE   你想获取非队列的信息
*              OS_ERR_PEVENT_NULL  如果 'pevent' 是一个NULL指针
*********************************************************************************************************
*/

#if OS_Q_QUERY_EN > 0
INT8U  OSQQuery (OS_EVENT *pevent, OS_Q_DATA *pdata)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif
    OS_Q      *pq;
    INT8U     *psrc;
    INT8U     *pdest;


#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'     */
        return (OS_ERR_PEVENT_NULL);
    }//非法pevent 
    if (pevent->OSEventType != OS_EVENT_TYPE_Q) {          /* Validate event block type   */
        return (OS_ERR_EVENT_TYPE);
    }//保证没有送空指针
#endif
    OS_ENTER_CRITICAL();
//复制等待任务列表
    pdata->OSEventGrp = pevent->OSEventGrp;           /* Copy message queue wait list  */
    psrc              = &pevent->OSEventTbl[0];
    pdest             = &pdata->OSEventTbl[0];
#if OS_EVENT_TBL_SIZE > 0//从0开始，慢慢复制吧。
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
    pq = (OS_Q *)pevent->OSEventPtr;
    if (pq->OSQEntries > 0) {//如果有消息等待，则提取出（并没有删除）队列中最早进入
    //队列的消息，并将其复制到OSMsg中，OSQQuery并没有改动.OSQOut的指针。
        pdata->OSMsg = *pq->OSQOut;                        /* Get next message to return if available  */
    } else {//如果没有任务等待，则返回空指针
        pdata->OSMsg = (void *)0;
    }
    pdata->OSNMsgs = pq->OSQEntries;//队列中消息数
    pdata->OSQSize = pq->OSQSize;//队列容易大小
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif                                                     /* OS_Q_QUERY_EN                            */

/*$PAGE*/
/*
*********************************************************************************************************
*                                      QUEUE MODULE INITIALIZATION
*
* Description : This function is called by uC/OS-II to initialize the message queue module.  Your
*               application MUST NOT call this function.
*
* Arguments   :  none
*
* Returns     : none
*
* Note(s)    : This function is INTERNAL to uC/OS-II and your application should not call it.
                                                   初始化消息队列
描述：由ucos调用初始化消息队列模型，应用程序不能调用
参数：无
返回：无
备注：由ucos调用初始化消息队列模型，应用程序不能调用
*********************************************************************************************************
*/

void  OS_QInit (void)
{
#if OS_MAX_QS == 1//如果只有一个队列
    OSQFreeList         = &OSQTbl[0];            /* Only ONE queue!     */
    OSQFreeList->OSQPtr = (OS_Q *)0;//前趋为零指针
#endif

#if OS_MAX_QS >= 2
    INT16U  i;
    OS_Q   *pq1;
    OS_Q   *pq2;


    pq1 = &OSQTbl[0];
    pq2 = &OSQTbl[1];
    for (i = 0; i < (OS_MAX_QS - 1); i++) {      /* Init. list of free QUEUE control blocks            */
		//初始化空闲队列控制块，组成一个单向链表
        pq1->OSQPtr = pq2;
        pq1++;
        pq2++;
    }
    pq1->OSQPtr = (OS_Q *)0;//后趋附指向NULL指针
    OSQFreeList = &OSQTbl[0];//空链表指针指向首地址。
#endif
}
#endif                                                     /* OS_Q_EN                                  */
