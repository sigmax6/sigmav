/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                       MESSAGE MAILBOX MANAGEMENT
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File : OS_MBOX.C
* By   : Jean J. Labrosse
* 翻译： likee
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

#if OS_MBOX_EN > 0
/*
*********************************************************************************************************
*                                     ACCEPT MESSAGE FROM MAILBOX
*
* Description: This function checks the mailbox to see if a message is available.  Unlike OSMboxPend(),
*              OSMboxAccept() does not suspend the calling task if a message is not available.
*
* Arguments  : pevent        is a pointer to the event control block
*
* Returns    : != (void *)0  is the message in the mailbox if one is available.  The mailbox is cleared
*                            so the next time OSMboxAccept() is called, the mailbox will be empty.
*              == (void *)0  if the mailbox is empty or,
*                            if 'pevent' is a NULL pointer or,
*                            if you didn't pass the proper event pointer.
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                    无等待地从邮箱中得到一则消息
描述：此函数检查邮箱中是否有消息，不同于OSMboxPend()，如果没有可行消息，
                OSMboxAccept() 不会挂起调用此函数的任务
参数；：pevent：是指向事件控制块的指针
返回：!= (void *)0 如果邮箱有消息，它指向邮箱中的一则消息，邮箱被清空，
                     如果下一次调用，它将是空的。
                 == (void *)0 如果邮箱为空，或者pevent指向空指针，或者没有传递一个合理事件
                                指针
*********************************************************************************************************
*/


#if OS_MBOX_ACCEPT_EN > 0
void  *OSMboxAccept (OS_EVENT *pevent)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    void      *msg;


#if OS_ARG_CHK_EN > 0//允许参数检验
    if (pevent == (OS_EVENT *)0) {                        /* Validate 'pevent'                         */
        return ((void *)0);//如果事件无效，返回零
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_MBOX) {      /* Validate event block type                 */
		//检查pevent所指向的事件控制块是不是由OS_EVENT_TYPE_MBOX建立
        return ((void *)0);
    }
#endif
    OS_ENTER_CRITICAL();
    msg                = pevent->OSEventPtr;//函数得到邮箱的当前内容
    pevent->OSEventPtr = (void *)0;                       /* Clear the mailbox     *///清空邮箱
    OS_EXIT_CRITICAL();
    return (msg);   /* Return the message received (or NULL)     *///将邮箱中的内容返回，以供调用
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                        CREATE A MESSAGE MAILBOX
*
* Description: This function creates a message mailbox if free event control blocks are available.
*
* Arguments  : msg           is a pointer to a message that you wish to deposit in the mailbox.  If
*                            you set this value to the NULL pointer (i.e. (void *)0) then the mailbox
*                            will be considered empty.
*
* Returns    : != (OS_EVENT *)0  is a pointer to the event control clock (OS_EVENT) associated with the
*                                created mailbox
*              == (OS_EVENT *)0  if no event control blocks were available
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                            建立一个邮箱
描述：如果空余事件控制块允许，就建立一个消息邮箱
参数：msg:指向你想存入邮箱的消息指针，如果存入零，我们认为它为空
返回：!= (OS_EVENT *)0：指向所建立邮箱的事件控制块指针
                 == (OS_EVENT *)0：如果没有可行事件控制块
*********************************************************************************************************
*/



OS_EVENT  *OSMboxCreate (void *msg)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    OS_EVENT  *pevent;


    if (OSIntNesting > 0) {                      /* See if called from ISR ...                         */
        return ((OS_EVENT *)0);                  /* ... can't CREATE from an ISR                       */
    }//此函数不能由中断服务程序调用
    OS_ENTER_CRITICAL();
    pevent = OSEventFreeList;                    /* Get next free event control block                  */
	//得到一块新的事件控制块
    if (OSEventFreeList != (OS_EVENT *)0) {      /* See if pool of free ECB pool was empty             */
        OSEventFreeList = (OS_EVENT *)OSEventFreeList->OSEventPtr;
    }//如果空余事件控制链表不为空，则进行适当调整，指向下一块空余事件控制块
    OS_EXIT_CRITICAL();
    if (pevent != (OS_EVENT *)0) {
        pevent->OSEventType = OS_EVENT_TYPE_MBOX;//如果ECB可用，则将其设置为邮箱型
        pevent->OSEventCnt  = 0;//信号量，邮箱中不使用它
        pevent->OSEventPtr  = msg;               /* Deposit message in event control block             */
		//保存消息在ECB中
        OS_EventWaitListInit(pevent);//对事件控制块的等待任务列表进行初始化
    }
    return (pevent);                             /* Return pointer to event control block              */
	//返回事件控制块的指针，对邮箱的操作都通过此指针完成。
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                         DELETE A MAIBOX
*
* Description: This function deletes a mailbox and readies all tasks pending on the mailbox.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired
*                            mailbox.
*
*              opt           determines delete options as follows:
*                            opt == OS_DEL_NO_PEND   Delete the mailbox ONLY if no task pending
*                            opt == OS_DEL_ALWAYS    Deletes the mailbox even if tasks are waiting.
*                                                    In this case, all the tasks pending will be readied.
*
*              err           is a pointer to an error code that can contain one of the following values:
*                            OS_NO_ERR               The call was successful and the mailbox was deleted
*                            OS_ERR_DEL_ISR          If you attempted to delete the mailbox from an ISR
*                            OS_ERR_INVALID_OPT      An invalid option was specified
*                            OS_ERR_TASK_WAITING     One or more tasks were waiting on the mailbox
*                            OS_ERR_EVENT_TYPE       If you didn't pass a pointer to a mailbox
*                            OS_ERR_PEVENT_NULL      If 'pevent' is a NULL pointer.
*
* Returns    : pevent        upon error
*              (OS_EVENT *)0 if the mailbox was successfully deleted.
*
* Note(s)    : 1) This function must be used with care.  Tasks that would normally expect the presence of
*                 the mailbox MUST check the return code of OSMboxPend().
*              2) OSMboxAccept() callers will not know that the intended mailbox has been deleted!
*              3) This call can potentially disable interrupts for a long time.  The interrupt disable
*                 time is directly proportional to the number of tasks waiting on the mailbox.
*              4) Because ALL tasks pending on the mailbox will be readied, you MUST be careful in
*                 applications where the mailbox is used for mutual exclusion because the resource(s)
*                 will no longer be guarded by the mailbox.
                                             删除一个邮箱
描述：此任务删除一个邮箱，准备好邮箱中挂起的所有任务
参数：pevent：指向想要邮箱的ECB的指针
                 opt：确定删除选项如下：
                 opt == OS_DEL_NO_PEND：如果没有任务挂起就删除邮箱
                 opt == OS_DEL_ALWAYS：即使任务等待也删除邮箱，这种情况下，所有挂起的任务将就绪
                 err：包含以下值的错误代码指针：
                 OS_NO_ERR：调用成功，删除邮箱
                 OS_ERR_DEL_ISR   ：如果从ISR中删除邮箱
*               OS_ERR_INVALID_OPT ：指定了不合理选项
*               OS_ERR_TASK_WAITING ：一个或者几个任务在邮箱中等待
*               OS_ERR_EVENT_TYPE ：没有传递指针给邮箱
*               OS_ERR_PEVENT_NULL :pevent是空指针，即邮箱的ECB为空。
返回：pevent：操作错误
                 (OS_EVENT *)0：邮箱成功删除
备注：1、此函数必须小心使用，任务希望在场的邮箱检查OSMboxPend()的返回代码
                2、OSMboxAccept()调用将不知道想要的邮箱已经被删除
                3、这个函数将潜在地关中断一长段时间，关中断时间与在邮箱中等待的任务
                       数成正比
                4、因为所有在邮箱中挂起的任务将就绪，你必须在邮箱相互排斥情况下小心使用
                       因为资源不再被邮箱保护
*********************************************************************************************************
*/


#if OS_MBOX_DEL_EN > 0
OS_EVENT  *OSMboxDel (OS_EVENT *pevent, INT8U opt, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    BOOLEAN    tasks_waiting;//typedef unsigned char BOOLEAN


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_DEL_ISR;                             /* ... can't DELETE from an ISR             */
        return (pevent);//如果在中断中调用，不能删除
    }
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        *err = OS_ERR_PEVENT_NULL;
        return (pevent);//如果pevent不合理
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_MBOX) {       /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return (pevent);//如果事件不是邮箱模式
    }
#endif
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0x00) {                      /* See if any tasks waiting on mailbox      */
		//看邮箱中有没有任务在等待
        tasks_waiting = TRUE;                              /* Yes                                      */
    } else {
        tasks_waiting = FALSE;                             /* No                                       */
    }
    switch (opt) {
        case OS_DEL_NO_PEND:                               /* Delete mailbox only if no task waiting   */
			//没有任务等待的情况下才删除邮箱
             if (tasks_waiting == FALSE) {//如果没有任务等待
                 pevent->OSEventType = OS_EVENT_TYPE_UNUSED;//将事件标志为没有用的
                 pevent->OSEventPtr  = OSEventFreeList;    /* Return Event Control Block to free list  */
				 //将ECB返回给空闲列表
                 OSEventFreeList     = pevent;             /* Get next free event control block        */
				 //它指向新的空闲列表
                 OS_EXIT_CRITICAL();
                 *err = OS_NO_ERR;//成功删除
                 return ((OS_EVENT *)0);                   /* Mailbox has been deleted                 */
             } else {
                 OS_EXIT_CRITICAL();
                 *err = OS_ERR_TASK_WAITING;//如果有任务在等待
                 return (pevent);
             }

        case OS_DEL_ALWAYS:                                /* Always delete the mailbox                */
             while (pevent->OSEventGrp != 0x00) {          /* Ready ALL tasks waiting for mailbox      */
                 OS_EventTaskRdy(pevent, (void *)0, OS_STAT_MBOX);
             }//所以等待该信号的任务都进入就绪态，每个任务都以为自己得到了
             //NULL指针，每个任务必须检查返回的指针，看是不是非NULL，另外，
             //当每个任务都进入就绪态，中断是关的，也延长了中断响应时间。
             pevent->OSEventType = OS_EVENT_TYPE_UNUSED;
             pevent->OSEventPtr  = OSEventFreeList;        /* Return Event Control Block to free list  */
             OSEventFreeList     = pevent;                 /* Get next free event control block        */
			 //同上面一样，将ECB返回给空闲列表，并指向新的表头
             OS_EXIT_CRITICAL();
             if (tasks_waiting == TRUE) {                  /* Reschedule only if task(s) were waiting  */
			 	//只有有任务等待的时候才需要任务调度
                 OS_Sched();                               /* Find highest priority task ready to run  */
             }
             *err = OS_NO_ERR;
             return ((OS_EVENT *)0);                       /* Mailbox has been deleted                 */

        default:
             OS_EXIT_CRITICAL();
             *err = OS_ERR_INVALID_OPT;//如果有其它不合理的选项。
             return (pevent);
    }
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                      PEND ON MAILBOX FOR A MESSAGE
*
* Description: This function waits for a message to be sent to a mailbox
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired mailbox
*
*              timeout       is an optional timeout period (in clock ticks).  If non-zero, your task will
*                            wait for a message to arrive at the mailbox up to the amount of time
*                            specified by this argument.  If you specify 0, however, your task will wait
*                            forever at the specified mailbox or, until a message arrives.
*
*              err           is a pointer to where an error message will be deposited.  Possible error
*                            messages are:
*
*                            OS_NO_ERR           The call was successful and your task received a
*                                                message.
*                            OS_TIMEOUT          A message was not received within the specified timeout
*                            OS_ERR_EVENT_TYPE   Invalid event type
*                            OS_ERR_PEND_ISR     If you called this function from an ISR and the result
*                                                would lead to a suspension.
*                            OS_ERR_PEVENT_NULL  If 'pevent' is a NULL pointer
*
* Returns    : != (void *)0  is a pointer to the message received
*              == (void *)0  if no message was received or,
*                            if 'pevent' is a NULL pointer or,
*                            if you didn't pass the proper pointer to the event control block.
                                                 等待邮箱中的消息
描述：等待将要送到邮箱中的消息
参数：pevent：指向目标邮箱ECB的指针
                 timeout：超时阶段的选项，如果非零，事件将在邮箱中等待消息的到来
                 直到这个值的时间到；如果是零，消息将永远等待，直到消息到来。
                 err：指向将要传递的错误消息指针。可能如下：
                            OS_NO_ERR          操作成功，任务得到消息
*                            OS_TIMEOUT          规定时间内没有收到消息
*                            OS_ERR_EVENT_TYPE  非法事件类型
*                            OS_ERR_PEND_ISR     如果在中断中调用，结果将中止
*                            OS_ERR_PEVENT_NULL  如果pevent 是空指针
*********************************************************************************************************
*/

void  *OSMboxPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    void      *msg;


    if (OSIntNesting > 0) {                           /* See if called from ISR ...                    */
        *err = OS_ERR_PEND_ISR;                       /* ... can't PEND from an ISR                    */
        return ((void *)0);
    }//中断服务子程序不能等待。
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        *err = OS_ERR_PEVENT_NULL;
        return ((void *)0);//目标邮箱ECB指针为零，不合理
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_MBOX) {  /* Validate event block type                     */
		//事件类型不是邮箱类型
        *err = OS_ERR_EVENT_TYPE;
        return ((void *)0);
    }
#endif
    OS_ENTER_CRITICAL();
    msg = pevent->OSEventPtr;
    if (msg != (void *)0) {                           /* See if there is already a message */
//如果邮箱中有消息，则从中取出消息，返回给调用函数，并将NULL存入邮箱，
//同时返回无错代码给它的调用函数，这个返回结果表示，已由另一个任务或者中断
//服务子程序将信息发送到邮箱中。如果为空的话，调用此函数的任务将进入睡眠状态
//等待另一个任务（可中断服务程序）通过邮箱发送消息，OSMboxPend（）允许定义一个
//最长等待时间作为它的参数，防止任务无期限等待
        pevent->OSEventPtr = (void *)0;               /* Clear the mailbox                             */
        OS_EXIT_CRITICAL();
        *err = OS_NO_ERR;
        return (msg);                                 /* Return the message received (or NULL)         */
		//返回得到的消息
    }
    OSTCBCur->OSTCBStat |= OS_STAT_MBOX;              /* Message not available, task will pend         */
	//消息无效，挂起任务，进入睡眠，等待其它邮箱消息唤醒
    OSTCBCur->OSTCBDly   = timeout;                   /* Load timeout in TCB                           */
	//等待时间也放入任务控制块中，该址在OSTimeTick()中被逐次减一
    OS_EventTaskWait(pevent);                         /* Suspend task until event or timeout occurs    */
	//真正将任务进入睡眠，
    OS_EXIT_CRITICAL();
    OS_Sched();                                       /* Find next highest priority task ready to run  */
	//任务调度，下一个优先级的任务运行。
    OS_ENTER_CRITICAL();
    msg = OSTCBCur->OSTCBMsg;//取出消息
    if (msg != (void *)0) {                           /* See if we were given the message              */
        OSTCBCur->OSTCBMsg      = (void *)0;          /* Yes, clear message received                   */
        OSTCBCur->OSTCBStat     = OS_STAT_RDY;
        OSTCBCur->OSTCBEventPtr = (OS_EVENT *)0;      /* No longer waiting for event                   */
        OS_EXIT_CRITICAL();
		//是否真正接收到消息，如果是，进入就绪态，不再等待事件，
        *err                    = OS_NO_ERR;
        return (msg);                                 /* Return the message received                   */
		//将收到的消息返回
    }
    OS_EventTO(pevent);                               /* Timed out, Make task ready                    */
	//如果超时，让任务就绪
    OS_EXIT_CRITICAL();
    *err = OS_TIMEOUT;                                /* Indicate that a timeout occured               */
    return ((void *)0);                               /* Return a NULL message                         */
	//返回空指针
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                       POST MESSAGE TO A MAILBOX
*
* Description: This function sends a message to a mailbox
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired mailbox
*
*              msg           is a pointer to the message to send.  You MUST NOT send a NULL pointer.
*
* Returns    : OS_NO_ERR            The call was successful and the message was sent
*              OS_MBOX_FULL         If the mailbox already contains a message.  You can can only send one
*                                   message at a time and thus, the message MUST be consumed before you
*                                   are allowed to send another one.
*              OS_ERR_EVENT_TYPE    If you are attempting to post to a non mailbox.
*              OS_ERR_PEVENT_NULL   If 'pevent' is a NULL pointer
*              OS_ERR_POST_NULL_PTR If you are attempting to post a NULL pointer
                                                       向邮箱发送一则消息
描述：向邮箱发送一则消息
参数：pevent：指向目标邮箱的ECB的指针
                 msg:将要发送的消息指针，不能发送空指针
 返回：OS_NO_ERR：消息发送成功
                OS_MBOX_FULL ：如果邮箱中已经有消息了，一次只能发送一条消息，
                在你允许发送另一条前，消息必须用掉
                OS_ERR_EVENT_TYPE：如果你要发送到的不是邮箱
                 OS_ERR_PEVENT_NULL：如果目标ECB是空指针
                 OS_ERR_POST_NULL_PTR：如果你想发空指针
*********************************************************************************************************
*/

#if OS_MBOX_POST_EN > 0
INT8U  OSMboxPost (OS_EVENT *pevent, void *msg)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        return (OS_ERR_PEVENT_NULL);
    }//合理的目标ECB
    if (msg == (void *)0) {                           /* Make sure we are not posting a NULL pointer   */
        return (OS_ERR_POST_NULL_PTR);
    }//发送的不是空指针
    if (pevent->OSEventType != OS_EVENT_TYPE_MBOX) {  /* Validate event block type                     */
        return (OS_ERR_EVENT_TYPE);
    }
#endif
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0x00) {                 /* See if any task pending on mailbox            */
		//是不是有任务在等待该邮箱中的消息，如果OSEventGrp非零，则表明有任务
        OS_EventTaskRdy(pevent, msg, OS_STAT_MBOX);   /* Ready highest priority task waiting on event  */
		//将最高优先级的任务从等待列表中删除
        OS_EXIT_CRITICAL();
        OS_Sched();                                   /* Find highest priority task ready to run       */
		//任务调度，检查该任务是否系统中就绪任务优先级最高，如果是，任务切换，
		//该任务得以执行，如果不是，则OS_Sched（）返回，OSMboxPost的调用函数继续。
        return (OS_NO_ERR);
    }
    if (pevent->OSEventPtr != (void *)0) {            /* Make sure mailbox doesn't already have a msg  */
        OS_EXIT_CRITICAL();
        return (OS_MBOX_FULL);//如果邮箱中有消息，表明邮箱满了
    }
    pevent->OSEventPtr = msg;                         /* Place message in mailbox                      */
	//将消息放到邮箱中
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                       POST MESSAGE TO A MAILBOX
*
* Description: This function sends a message to a mailbox
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired mailbox
*
*              msg           is a pointer to the message to send.  You MUST NOT send a NULL pointer.
*
*              opt           determines the type of POST performed:
*                            OS_POST_OPT_NONE         POST to a single waiting task 
*                                                     (Identical to OSMboxPost())
*                            OS_POST_OPT_BROADCAST    POST to ALL tasks that are waiting on the mailbox
*
* Returns    : OS_NO_ERR            The call was successful and the message was sent
*              OS_MBOX_FULL         If the mailbox already contains a message.  You can can only send one
*                                   message at a time and thus, the message MUST be consumed before you
*                                   are allowed to send another one.
*              OS_ERR_EVENT_TYPE    If you are attempting to post to a non mailbox.
*              OS_ERR_PEVENT_NULL   If 'pevent' is a NULL pointer
*              OS_ERR_POST_NULL_PTR If you are attempting to post a NULL pointer
*
* Warning    : Interrupts can be disabled for a long time if you do a 'broadcast'.  In fact, the 
*              interrupt disable time is proportional to the number of tasks waiting on the mailbox.
                                              功能更强的向邮箱中发送一则消息
描述：发送一个消息到邮箱
参数：pevent：指向目标邮箱的ECB的指针
                 msg:将要发送的消息指针，不能发送空指针
                 opt：决定如下发送模式：
                         OS_POST_OPT_NONE ：发给单片等待任务，这个和OSMboxPost()等同。
                         OS_POST_OPT_BROADCAST：向在邮箱中等待的所有任务发送。
返回：  OS_NO_ERR：消息发送成功
                OS_MBOX_FULL ：如果邮箱中已经有消息了，一次只能发送一条消息，
                在你允许发送另一条前，消息必须用掉
                OS_ERR_EVENT_TYPE：如果你要发送到的不是邮箱
                 OS_ERR_PEVENT_NULL：如果目标ECB是空指针
                 OS_ERR_POST_NULL_PTR：如果你想发空指针                     
*********************************************************************************************************
*/

#if OS_MBOX_POST_OPT_EN > 0
INT8U  OSMboxPostOpt (OS_EVENT *pevent, void *msg, INT8U opt)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        return (OS_ERR_PEVENT_NULL);
    }
    if (msg == (void *)0) {                           /* Make sure we are not posting a NULL pointer   */
        return (OS_ERR_POST_NULL_PTR);
    }//保证传递的不是空指针
    if (pevent->OSEventType != OS_EVENT_TYPE_MBOX) {  /* Validate event block type                     */
        return (OS_ERR_EVENT_TYPE);
    }//保证是邮箱类型
#endif
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0x00) {                 /* See if any task pending on mailbox            */
		//是否有任务在邮箱中挂起
        if ((opt & OS_POST_OPT_BROADCAST) != 0x00) {  /* Do we need to post msg to ALL waiting tasks ? */
			//是不是向所有任务发消息？
            while (pevent->OSEventGrp != 0x00) {      /* Yes, Post to ALL tasks waiting on mailbox     */   
				//如果是，向所有等待任务发，将所有任务从等待列表中删除
                OS_EventTaskRdy(pevent, msg, OS_STAT_MBOX);    
            }
        } else {
            OS_EventTaskRdy(pevent, msg, OS_STAT_MBOX);    /* No,  Post to HPT waiting on mbox         */
			//如果没有广播，则只有最高优先级进入就绪态，准备运行，
			//OS_EventTaskRdy函数只将最高优先级任务从等待列表中删除
        }
        OS_EXIT_CRITICAL();
        OS_Sched();                                        /* Find highest priority task ready to run  */
		//任务调度
        return (OS_NO_ERR);
    }
    if (pevent->OSEventPtr != (void *)0) {            /* Make sure mailbox doesn't already have a msg  */
        OS_EXIT_CRITICAL();
        return (OS_MBOX_FULL);//如果邮箱中有邮件，返回已经满了。
    }
    pevent->OSEventPtr = msg;                         /* Place message in mailbox                      */
	//保存邮箱中的消息
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                        QUERY A MESSAGE MAILBOX
*
* Description: This function obtains information about a message mailbox.
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired mailbox
*
*              pdata         is a pointer to a structure that will contain information about the message
*                            mailbox.
*
* Returns    : OS_NO_ERR           The call was successful and the message was sent
*              OS_ERR_EVENT_TYPE   If you are attempting to obtain data from a non mailbox.
*              OS_ERR_PEVENT_NULL  If 'pevent' is a NULL pointer
                                                               查询一个邮箱的状态
描述：包含消息邮箱的信息
参数：pevent：指向目标邮箱ECB的指针
                 pdata：包含消息邮箱信息的结构指针
返回：OS_NO_ERR           调用成功，消息发送成功
*              OS_ERR_EVENT_TYPE   你想从非邮箱中得到数据
*              OS_ERR_PEVENT_NULL  如果pevent是NULL
*********************************************************************************************************
*/

#if OS_MBOX_QUERY_EN > 0
INT8U  OSMboxQuery (OS_EVENT *pevent, OS_MBOX_DATA *pdata)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    INT8U     *psrc;
    INT8U     *pdest;


#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        return (OS_ERR_PEVENT_NULL);
    }//不合理的pevent
    if (pevent->OSEventType != OS_EVENT_TYPE_MBOX) {       /* Validate event block type                */
        return (OS_ERR_EVENT_TYPE);
    }//如果不是邮箱类型
#endif
    OS_ENTER_CRITICAL();
    pdata->OSEventGrp = pevent->OSEventGrp;                /* Copy message mailbox wait list           */
	//拷贝消息邮箱等待列表
    psrc              = &pevent->OSEventTbl[0];
    pdest             = &pdata->OSEventTbl[0];//邮箱中等待事件发生链表
//复制等待任务列表，，之所以使用条件编译产生的内嵌代码，不用循环语句，
//是因为这样代码运行速度更快
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
    pdata->OSMsg = pevent->OSEventPtr;                     /* Get message from mailbox                 */
//从邮箱中获得消息
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif                                                     /* OS_MBOX_QUERY_EN                         */
#endif                                                     /* OS_MBOX_EN                               */
