/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                         EVENT FLAG  MANAGEMENT
*
*                          (c) Copyright 2001-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File : OS_FLAG.C
* By   : Jean J. Labrosse
* 翻译： likee
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include "INCLUDES.H"
#endif

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
/*
*********************************************************************************************************
*                                            LOCAL PROTOTYPES
                                                    局部定义
*********************************************************************************************************
*/

static  void     OS_FlagBlock(OS_FLAG_GRP *pgrp, OS_FLAG_NODE *pnode, OS_FLAGS flags, INT8U wait_type, INT16U timeout);
static  BOOLEAN  OS_FlagTaskRdy(OS_FLAG_NODE *pnode, OS_FLAGS flags_rdy);

/*$PAGE*/
/*
*********************************************************************************************************
*                              CHECK THE STATUS OF FLAGS IN AN EVENT FLAG GROUP
*
* Description: This function is called to check the status of a combination of bits to be set or cleared
*              in an event flag group.  Your application can check for ANY bit to be set/cleared or ALL
*              bits to be set/cleared.
*
*              This call does not block if the desired flags are not present.
*
* Arguments  : pgrp          is a pointer to the desired event flag group.
*
*              flags         Is a bit pattern indicating which bit(s) (i.e. flags) you wish to check.
*                            The bits you want are specified by setting the corresponding bits in
*                            'flags'.  e.g. if your application wants to wait for bits 0 and 1 then
*                            'flags' would contain 0x03.
*
*              wait_type     specifies whether you want ALL bits to be set/cleared or ANY of the bits
*                            to be set/cleared.
*                            You can specify the following argument:
*
*                            OS_FLAG_WAIT_CLR_ALL   You will check ALL bits in 'flags' to be clear (0)
*                            OS_FLAG_WAIT_CLR_ANY   You will check ANY bit  in 'flags' to be clear (0)
*                            OS_FLAG_WAIT_SET_ALL   You will check ALL bits in 'flags' to be set   (1)
*                            OS_FLAG_WAIT_SET_ANY   You will check ANY bit  in 'flags' to be set   (1)
*
*                            NOTE: Add OS_FLAG_CONSUME if you want the event flag to be 'consumed' by
*                                  the call.  Example, to wait for any flag in a group AND then clear
*                                  the flags that are present, set 'wait_type' to:
*
*                                  OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME
*
*              err           is a pointer to an error code and can be:
*                            OS_NO_ERR              No error
*                            OS_ERR_EVENT_TYPE      You are not pointing to an event flag group
*                            OS_FLAG_ERR_WAIT_TYPE  You didn't specify a proper 'wait_type' argument.
*                            OS_FLAG_INVALID_PGRP   You passed a NULL pointer instead of the event flag
*                                                   group handle.
*                            OS_FLAG_ERR_NOT_RDY    The desired flags you are waiting for are not
*                                                   available.
*
* Returns    : The state of the flags in the event flag group.
*
* Called from: Task or ISR
                                    无等待地获得事件标志组中的事件标志
描述：去检查事件标志组中结合位的状态是置位还是被清除，你能
                检查任何将被置位或者清除的位或者全部位
                如果等待事件不发生，调用事件并不挂起。与OSFlagPend()唯一不同点
参数：pgrp：指向目标事件标志组的指针
                flags：是一个位的模式显示要检查的位，比如：你要检测0位和1位，那么你将它
                           设置为0x03
                wait_type ：标记你想全部检测还是只想检测其实一部分。你能标记如下参数：
*                            OS_FLAG_WAIT_CLR_ALL   你将检测flags中全部的清零位
*                            OS_FLAG_WAIT_CLR_ANY   你将检测flags中任何清零位
*                            OS_FLAG_WAIT_SET_ALL   你将检测flags中全部的置一位
*                            OS_FLAG_WAIT_SET_ANY   你将检测flags中任何置一位
               如果想事件标志被调用函数清除的话，要加上OS_FLAG_CONSUME，比如：如果
               想要组与后清除，那么将wait_type设置成OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME
               err 指向错误代码的指针，可以为：
*                            OS_NO_ERR             无误
*                            OS_ERR_EVENT_TYPE      没有指向任务事件标志组
*                            OS_FLAG_ERR_WAIT_TYPE  你没有设置正确的 'wait_type' 参数
*                            OS_FLAG_INVALID_PGRP   你传送了零指针而不是事件标志组操作
*                            OS_FLAG_ERR_NOT_RDY    你等待的目标标志不合理
返回：事件标志组的标志状态
从任务和ISR中调用
*********************************************************************************************************
*/

#if OS_FLAG_ACCEPT_EN > 0
OS_FLAGS  OSFlagAccept (OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR     cpu_sr;
#endif
    OS_FLAGS      flags_cur;
    OS_FLAGS      flags_rdy;
    BOOLEAN       consume;


#if OS_ARG_CHK_EN > 0
    if (pgrp == (OS_FLAG_GRP *)0) {                        /* Validate 'pgrp'                          */
        *err = OS_FLAG_INVALID_PGRP;
        return ((OS_FLAGS)0);//非空的pgrp'
    }
    if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {          /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return ((OS_FLAGS)0);//合理的事件控制块类型
    }
#endif
    if (wait_type & OS_FLAG_CONSUME) {                     /* See if we need to consume the flags      */
        wait_type &= ~OS_FLAG_CONSUME;
        consume    = TRUE;
    } else {
        consume    = FALSE;
    }//是否需要清除位
/*$PAGE*/
    *err = OS_NO_ERR;                                      /* Assume NO error until proven otherwise.  */
//初始化无错
    OS_ENTER_CRITICAL();
    switch (wait_type) {
        case OS_FLAG_WAIT_SET_ALL:                         /* See if all required flags are set        */
             flags_rdy = pgrp->OSFlagFlags & flags;        /* Extract only the bits we want            */
             if (flags_rdy == flags) {                     /* Must match ALL the bits that we want     */
                 //如果刚好是我们要的
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags &= ~flags_rdy;      /* Clear ONLY the flags that we wanted      */
                 }//是否要清除
             } else {
                 *err  = OS_FLAG_ERR_NOT_RDY;//没有我们想要的，返回不合理
             }
             flags_cur = pgrp->OSFlagFlags;                /* Will return the state of the group       */
			 //返回标志
             OS_EXIT_CRITICAL();
             break;

        case OS_FLAG_WAIT_SET_ANY:
             flags_rdy = pgrp->OSFlagFlags & flags;        /* Extract only the bits we want            */
             if (flags_rdy != (OS_FLAGS)0) {               /* See if any flag set                      */
			 	//是否有标志置位
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags &= ~flags_rdy;      /* Clear ONLY the flags that we got         */
                 }//是否需要清除位
             } else {//没有标志置位
                 *err  = OS_FLAG_ERR_NOT_RDY;
             }
             flags_cur = pgrp->OSFlagFlags;                /* Will return the state of the group       */
			 //返回组状态，
             OS_EXIT_CRITICAL();
             break;

#if OS_FLAG_WAIT_CLR_EN > 0
        case OS_FLAG_WAIT_CLR_ALL:                         /* See if all required flags are cleared    */
             flags_rdy = ~pgrp->OSFlagFlags & flags;       /* Extract only the bits we want            */
			 //设置位
             if (flags_rdy == flags) {                     /* Must match ALL the bits that we want     */
			 	//必须所有的位都匹配
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags |= flags_rdy;       /* Set ONLY the flags that we wanted        */
                 }//是否要设置位
             } else {
                 *err  = OS_FLAG_ERR_NOT_RDY;//不是所有位都匹配
             }
             flags_cur = pgrp->OSFlagFlags;                /* Will return the state of the group       */
			 //返回标志组状态
             OS_EXIT_CRITICAL();
             break;

        case OS_FLAG_WAIT_CLR_ANY:
             flags_rdy = ~pgrp->OSFlagFlags & flags;       /* Extract only the bits we want            */
             if (flags_rdy != (OS_FLAGS)0) {               /* See if any flag cleared                  */
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags |= flags_rdy;       /* Set ONLY the flags that we got           */
                 }
             } else {
                 *err  = OS_FLAG_ERR_NOT_RDY;
             }
             flags_cur = pgrp->OSFlagFlags;                /* Will return the state of the group       */
             OS_EXIT_CRITICAL();
             break;
#endif

        default://其它异常情况
             OS_EXIT_CRITICAL();
             flags_cur = (OS_FLAGS)0;
             *err      = OS_FLAG_ERR_WAIT_TYPE;
             break;
    }
    return (flags_cur);//返回状态
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                           CREATE AN EVENT FLAG
*
* Description: This function is called to create an event flag group.
*
* Arguments  : flags         Contains the initial value to store in the event flag group.
*
*              err           is a pointer to an error code which will be returned to your application:
*                               OS_NO_ERR                if the call was successful.
*                               OS_ERR_CREATE_ISR        if you attempted to create an Event Flag from an
*                                                        ISR.
*                               OS_FLAG_GRP_DEPLETED     if there are no more event flag groups
*
* Returns    : A pointer to an event flag group or a NULL pointer if no more groups are available.
*
* Called from: Task ONLY
                                                     建立一个事件标志组
描述：建立一个事件标志组
参数：flags：包含存储在事件标志组中的初始值
                err：将返回到你应用程序的错误信息
 *                               OS_NO_ERR                如果成功
*                               OS_ERR_CREATE_ISR       如果你想从ISR中建立
*                               OS_FLAG_GRP_DEPLETED    如果没有多余的事件标志组了               
*********************************************************************************************************
*/

OS_FLAG_GRP  *OSFlagCreate (OS_FLAGS flags, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                         /* Allocate storage for CPU status register        */
    OS_CPU_SR    cpu_sr;
#endif
    OS_FLAG_GRP *pgrp;


    if (OSIntNesting > 0) {                         /* See if called from ISR ...                      */
        *err = OS_ERR_CREATE_ISR;                   /* ... can't CREATE from an ISR                    */
        return ((OS_FLAG_GRP *)0);//不能从ISR中建立
    }
    OS_ENTER_CRITICAL();
    pgrp = OSFlagFreeList;                          /* Get next free event flag                        */
	//指向空链表的空地址 ，取得一个空闲事件标志
    if (pgrp != (OS_FLAG_GRP *)0) {                 /* See if we have event flag groups available      */
                  /* Adjust free list       *///如果为0，表明没有空闲的事件标志组了
        OSFlagFreeList       = (OS_FLAG_GRP *)OSFlagFreeList->OSFlagWaitList;
		//调整系统的空闲事件标志组链表指针，使之指向新的表头
        pgrp->OSFlagType     = OS_EVENT_TYPE_FLAG;  /* Set to event flag group type                    */
		//分配它是事件标志组，确保能系统正常运行
        pgrp->OSFlagFlags    = flags;               /* Set to desired initial value                    */
		//将初始化值传入这个事件标志组
        pgrp->OSFlagWaitList = (void *)0;           /* Clear list of tasks waiting on flags            */
		//因为是刚刚建立的事件标志组，没有任何任务等待这个事件标志组，
		//所以等待任务链表指针初始化为0
        OS_EXIT_CRITICAL();
        *err                 = OS_NO_ERR;
    } else {//如果没有空闲的事件标志组了
        OS_EXIT_CRITICAL();
        *err                 = OS_FLAG_GRP_DEPLETED;
    }
    return (pgrp);                                  /* Return pointer to event flag group     */
	//返回刚刚建立的事件标志组指针，如果没有空闲的事件标志组，
	//将返回NULL指针
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                     DELETE AN EVENT FLAG GROUP
*
* Description: This function deletes an event flag group and readies all tasks pending on the event flag
*              group.
* Arguments  : pgrp          is a pointer to the desired event flag group.
*
*              opt           determines delete options as follows:
*                            opt == OS_DEL_NO_PEND   Deletes the event flag group ONLY if no task pending
*                            opt == OS_DEL_ALWAYS    Deletes the event flag group even if tasks are
*                                                    waiting.  In this case, all the tasks pending will be
*                                                    readied.
*
*              err           is a pointer to an error code that can contain one of the following values:
*                            OS_NO_ERR               The call was successful and the event flag group was
*                                                    deleted
*                            OS_ERR_DEL_ISR          If you attempted to delete the event flag group from
*                                                    an ISR
*                            OS_FLAG_INVALID_PGRP    If 'pgrp' is a NULL pointer.
*                            OS_ERR_EVENT_TYPE       If you didn't pass a pointer to an event flag group
*                            OS_ERR_INVALID_OPT      An invalid option was specified
*                            OS_ERR_TASK_WAITING     One or more tasks were waiting on the event flag
*                                                    group.
* Returns    : pevent        upon error
*              (OS_EVENT *)0 if the semaphore was successfully deleted.
*
* Note(s)    : 1) This function must be used with care.  Tasks that would normally expect the presence of
*                 the event flag group MUST check the return code of OSFlagAccept() and OSFlagPend().
*              2) This call can potentially disable interrupts for a long time.  The interrupt disable
*                 time is directly proportional to the number of tasks waiting on the event flag group.
                                                                删除一个事件标志组
描述：删除一个事件标志组，将事件标志组中所有挂起的任务就绪。
参数：pgrp：指向目标事件标志组的指针
                opt：决定以下删除选项：
*                            opt == OS_DEL_NO_PEND   没有任务挂起的时候才删事件标志组
*                            opt == OS_DEL_ALWAYS    即使有任务等待也删除，这个，所有挂起的
                                               任务将就绪。
 *              err           包含以下错误信息之一的指针
*                            OS_NO_ERR              调用成功，事件标志组成功删除
*                            OS_ERR_DEL_ISR     如果想从ISR中删除事件标志组
*                            OS_FLAG_INVALID_PGRP    如果pgrp是一个空指针
*                            OS_ERR_EVENT_TYPE      如果没有传递指针到事件标志组
*                            OS_ERR_INVALID_OPT      有非法选项
*                            OS_ERR_TASK_WAITING     一个或多个任务在事件标志组中等待
*返回    : pevent        有错
*              (OS_EVENT *)0 如果成功删除

                                               
*********************************************************************************************************
*/

#if OS_FLAG_DEL_EN > 0
OS_FLAG_GRP  *OSFlagDel (OS_FLAG_GRP *pgrp, INT8U opt, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR     cpu_sr;
#endif
    BOOLEAN       tasks_waiting;
    OS_FLAG_NODE *pnode;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_DEL_ISR;                             /* ... can't DELETE from an ISR             */
        return (pgrp);//不允许从ISR中删除
    }
#if OS_ARG_CHK_EN > 0
    if (pgrp == (OS_FLAG_GRP *)0) {                        /* Validate 'pgrp'                          */
        *err = OS_FLAG_INVALID_PGRP;
        return (pgrp);//pgrp必须有效。不能为零，并指向事件标志组
    }
    if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {          /* Validate event group type                */
        *err = OS_ERR_EVENT_TYPE;
        return (pgrp);//如果不是事件组类型
    }
#endif
    OS_ENTER_CRITICAL();
    if (pgrp->OSFlagWaitList != (void *)0) {     /* See if any tasks waiting on event flags  */
		//是否有任务在此等待？
        tasks_waiting = TRUE;                              /* Yes                                      */
    } else {//
        tasks_waiting = FALSE;                             /* No                                       */
    }
    switch (opt) {
        case OS_DEL_NO_PEND:    /* Delete group if no task waiting    */
             //如果没有任务等待才删除
             if (tasks_waiting == FALSE) {//没有任务等待
                 pgrp->OSFlagType     = OS_EVENT_TYPE_UNUSED;//将它标记为未使用
                 pgrp->OSFlagWaitList = (void *)OSFlagFreeList; /* Return group to free list           */
				 //将其返回到空闲链表中
                 OSFlagFreeList       = pgrp;
                 OS_EXIT_CRITICAL();
                 *err                 = OS_NO_ERR;//无错
                 return ((OS_FLAG_GRP *)0);                /* Event Flag Group has been deleted        */
             } else {//有任务等待
                 OS_EXIT_CRITICAL();
                 *err                 = OS_ERR_TASK_WAITING;
                 return (pgrp);//返回事件标志组指针
             }

        case OS_DEL_ALWAYS:                                /* Always delete the event flag group       */
			//有任务等待也删除
             pnode = (OS_FLAG_NODE *)pgrp->OSFlagWaitList;//指向事件标志中等待任务结点的指针
             while (pnode != (OS_FLAG_NODE *)0) {          /* Ready ALL tasks waiting for flags        */
			 	//就绪所有的任务
                 OS_FlagTaskRdy(pnode, (OS_FLAGS)0);//使任务就绪，事件发生
                 pnode = (OS_FLAG_NODE *)pnode->OSFlagNodeNext;//只要不为零，就继续指向下一个结点。继续就绪
             }
             pgrp->OSFlagType     = OS_EVENT_TYPE_UNUSED;//标记为未用
             pgrp->OSFlagWaitList = (void *)OSFlagFreeList;/* Return group to free list                */
             OSFlagFreeList       = pgrp;//将其返回到空闲链表
             OS_EXIT_CRITICAL();
             if (tasks_waiting == TRUE) {                  /* Reschedule only if task(s) were waiting  */
			 	//有任务等待的情况下，要重新任务调度。因为挂起的任务就绪了。
                 OS_Sched();                               /* Find highest priority task ready to run  */
             }
             *err = OS_NO_ERR;//无错
             return ((OS_FLAG_GRP *)0);                    /* Event Flag Group has been deleted        */

        default://其它异常情况
             OS_EXIT_CRITICAL();
             *err = OS_ERR_INVALID_OPT;
             return (pgrp);
    }
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                        WAIT ON AN EVENT FLAG GROUP
*
* Description: This function is called to wait for a combination of bits to be set in an event flag
*              group.  Your application can wait for ANY bit to be set or ALL bits to be set.
*
* Arguments  : pgrp          is a pointer to the desired event flag group.
*
*              flags         Is a bit pattern indicating which bit(s) (i.e. flags) you wish to wait for.
*                            The bits you want are specified by setting the corresponding bits in
*                            'flags'.  e.g. if your application wants to wait for bits 0 and 1 then
*                            'flags' would contain 0x03.
*
*              wait_type     specifies whether you want ALL bits to be set or ANY of the bits to be set.
*                            You can specify the following argument:
*
*                            OS_FLAG_WAIT_CLR_ALL   You will wait for ALL bits in 'mask' to be clear (0)
*                            OS_FLAG_WAIT_SET_ALL   You will wait for ALL bits in 'mask' to be set   (1)
*                            OS_FLAG_WAIT_CLR_ANY   You will wait for ANY bit  in 'mask' to be clear (0)
*                            OS_FLAG_WAIT_SET_ANY   You will wait for ANY bit  in 'mask' to be set   (1)
*
*                            NOTE: Add OS_FLAG_CONSUME if you want the event flag to be 'consumed' by
*                                  the call.  Example, to wait for any flag in a group AND then clear
*                                  the flags that are present, set 'wait_type' to:
*
*                                  OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME
*
*              timeout       is an optional timeout (in clock ticks) that your task will wait for the
*                            desired bit combination.  If you specify 0, however, your task will wait
*                            forever at the specified event flag group or, until a message arrives.
*
*              err           is a pointer to an error code and can be:
*                            OS_NO_ERR              The desired bits have been set within the specified
*                                                   'timeout'.
*                            OS_ERR_PEND_ISR        If you tried to PEND from an ISR
*                            OS_FLAG_INVALID_PGRP   If 'pgrp' is a NULL pointer.
*                            OS_ERR_EVENT_TYPE      You are not pointing to an event flag group
*                            OS_TIMEOUT             The bit(s) have not been set in the specified
*                                                   'timeout'.
*                            OS_FLAG_ERR_WAIT_TYPE  You didn't specify a proper 'wait_type' argument.
*
* Returns    : The new state of the flags in the event flag group when the task is resumed or,
*              0 if a timeout or an error occurred.
*
* Called from: Task ONLY
                                                         等待事件标志组的事件标志位
描述：去检查事件标志组中结合位的状态是置位还是被清除，你能
                检查任何将被置位或者清除的位或者全部位 
参数：pgrp：指向目标事件标志组的指针
                flags：是一个位的模式显示要检查的位，比如：你要检测0位和1位，那么你将它
                           设置为0x03
                wait_type ：标记你想全部检测还是只想检测其实一部分。你能标记如下参数：
*                            OS_FLAG_WAIT_CLR_ALL   等待所有指定事件标志组中的事件标志位清0
*                            OS_FLAG_WAIT_CLR_ANY   等待任意一个指定事件标志组中的事件标志位置1
*                            OS_FLAG_WAIT_SET_ALL   等待所有指定事件标志组中的事件标志位置1
*                            OS_FLAG_WAIT_SET_ANY  等待任意一个指定事件标志组中的事件标志位置1
               如果想事件标志被调用函数清除的话，要加上OS_FLAG_CONSUME，比如：如果
               想要组与后清除，那么将wait_type设置成OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME
               timeout:你的任务等待目标结合位超时，如果你标志零，你的任务将在指定
                         事件标志组永久等待直到一条消息到来
               err 指向错误代码的指针，可以为：
*                            OS_NO_ERR             无误
*                            OS_ERR_PEND_ISR       如果想从ISR中挂起
*                            OS_FLAG_INVALID_PGRP   如果 'pgrp' 是NULL指针
*                            OS_ERR_EVENT_TYPE      你没有指向任务事件控制组
*                            OS_TIMEOUT             在指定时间内位没有被置位
*                            OS_FLAG_ERR_WAIT_TYPE  你没有指定合适的 'wait_type' 参数

返回：当任务继续时，标志任务新状态，错误或者超时返回零
只能从任务中调用

*********************************************************************************************************
*/

OS_FLAGS  OSFlagPend (OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT16U timeout, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR     cpu_sr;
#endif
    OS_FLAG_NODE  node;
    OS_FLAGS      flags_cur;
    OS_FLAGS      flags_rdy;
    BOOLEAN       consume;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_PEND_ISR;                            /* ... can't PEND from an ISR               */
        return ((OS_FLAGS)0);//不允许在中断嵌套中调用本函数
    }
#if OS_ARG_CHK_EN > 0//允许检查函数参数
    if (pgrp == (OS_FLAG_GRP *)0) {                        /* Validate 'pgrp'                          */
        *err = OS_FLAG_INVALID_PGRP;
        return ((OS_FLAGS)0);//看pgrp是不是一个NULL指针
    }
    if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {          /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return ((OS_FLAGS)0);//看是否指向事件标志组数据结构。
    }
#endif
    if (wait_type & OS_FLAG_CONSUME) {                     /* See if we need to consume the flags      */
//看是否需要局部布尔变量，OSFlagPend 允许指定在任务等待事件发生后，重新
//置位或清除相应的事件标志位，方法是在调用此函数时，将一个常量与参数
//wait_type相加
        wait_type &= ~OS_FLAG_CONSUME;
        consume    = TRUE;
    } else {
        consume    = FALSE;
    }
/*$PAGE*/
    OS_ENTER_CRITICAL();
    switch (wait_type) {
        case OS_FLAG_WAIT_SET_ALL:                         /* See if all required flags are set        */
             flags_rdy = pgrp->OSFlagFlags & flags;        /* Extract only the bits we want            */
//如果wait_type为OS_FLAG_WAIT_SET_ALL或者OS_FLAG_WAIT_SET_AND时，函数取出由flags参数指定
//的事件标志位
             if (flags_rdy == flags) {                     /* Must match ALL the bits that we want     */
	//如果取出标志位状态恰好符合预期状态，说明任务需要等待的事件标志位都已经置位
	//此情况下，PEND函数立即返回调用函数
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags &= ~flags_rdy;      /* Clear ONLY the flags that we wanted      */
                 }//返回前检查是不是要进行清零操作
                 flags_cur = pgrp->OSFlagFlags;            /* Will return the state of the group       */
				 //返回组状态
                 OS_EXIT_CRITICAL();                       /* Yes, condition met, return to caller     */
				 //获取事件标志组的新事件标志状态值，并返回调用函数
                 *err      = OS_NO_ERR;
                 return (flags_cur);
             } else {                                      /* Block task until events occur or timeout */
             //如果指定的事件标志位没有完全置位，那么调用函数将挂起
             //直到超时或者事件来到
                 OS_FlagBlock(pgrp, &node, flags, wait_type, timeout);
                 OS_EXIT_CRITICAL();
             }
             break;

        case OS_FLAG_WAIT_SET_ANY:
             flags_rdy = pgrp->OSFlagFlags & flags;        /* Extract only the bits we want            */
			 //如果刚好是我们要的位，等待操作立即结束，返回调用函数
             if (flags_rdy != (OS_FLAGS)0) {               /* See if any flag set                      */
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags &= ~flags_rdy;      /* Clear ONLY the flags that we got         */
                 }//是否需要清除？
                 flags_cur = pgrp->OSFlagFlags;            /* Will return the state of the group       */
				 //获取事件标志组新的状态，并返回调用函数
                 OS_EXIT_CRITICAL();                       /* Yes, condition met, return to caller     */
                 *err      = OS_NO_ERR;
                 return (flags_cur);
             } else {                                      /* Block task until events occur or timeout */
                 OS_FlagBlock(pgrp, &node, flags, wait_type, timeout);
                 OS_EXIT_CRITICAL();
             }//如果所有都没有置位，则将调用函数挂起，直到事件来到或者超时
             break;

#if OS_FLAG_WAIT_CLR_EN > 0
        case OS_FLAG_WAIT_CLR_ALL:                         /* See if all required flags are cleared    */
             flags_rdy = ~pgrp->OSFlagFlags & flags;       /* Extract only the bits we want            */
             if (flags_rdy == flags) {                     /* Must match ALL the bits that we want     */
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags |= flags_rdy;       /* Set ONLY the flags that we wanted        */
                 }
                 flags_cur = pgrp->OSFlagFlags;            /* Will return the state of the group       */
                 OS_EXIT_CRITICAL();                       /* Yes, condition met, return to caller     */
                 *err      = OS_NO_ERR;
                 return (flags_cur);
             } else {                                      /* Block task until events occur or timeout */
                 OS_FlagBlock(pgrp, &node, flags, wait_type, timeout);
                 OS_EXIT_CRITICAL();
             }
             break;

        case OS_FLAG_WAIT_CLR_ANY:
             flags_rdy = ~pgrp->OSFlagFlags & flags;       /* Extract only the bits we want            */
             if (flags_rdy != (OS_FLAGS)0) {               /* See if any flag cleared                  */
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags |= flags_rdy;       /* Set ONLY the flags that we got           */
                 }
                 flags_cur = pgrp->OSFlagFlags;            /* Will return the state of the group       */
                 OS_EXIT_CRITICAL();                       /* Yes, condition met, return to caller     */
                 *err      = OS_NO_ERR;
                 return (flags_cur);
             } else {                                      /* Block task until events occur or timeout */
                 OS_FlagBlock(pgrp, &node, flags, wait_type, timeout);
                 OS_EXIT_CRITICAL();
             }
             break;
#endif

        default://其它情况
             OS_EXIT_CRITICAL();
             flags_cur = (OS_FLAGS)0;
             *err      = OS_FLAG_ERR_WAIT_TYPE;//错误的等待类型
             return (flags_cur);
    }
    OS_Sched();   /* Find next HPT ready to run  *///os_FlagBlock()返回时，会进行任务调度
    //是因为调用此任务的函数因为事件没有发生而不能继续运行
    OS_ENTER_CRITICAL();
    if (OSTCBCur->OSTCBStat & OS_STAT_FLAG) {              /* Have we timed-out?                       */
        OS_FlagUnlink(&node);//超时了吗？如果超时，就从双向等待列表中删除
        OSTCBCur->OSTCBStat = OS_STAT_RDY;/* Yes, make task ready-to-run   */
		//超时就使任务进入就绪状态
        OS_EXIT_CRITICAL();
        flags_cur           = (OS_FLAGS)0;
        *err                = OS_TIMEOUT;                  /* Indicate that we timed-out waiting       */
		//返回：我们超时啦！
    } else {//如果没有超时，一定是按照预期的发生了
        if (consume == TRUE) {         /* See if we need to consume the flags      */
			//是否要清除参数
            switch (wait_type) {
                case OS_FLAG_WAIT_SET_ALL:
                case OS_FLAG_WAIT_SET_ANY:                 /* Clear ONLY the flags we got              */
					//只清除我们得到的。
                     pgrp->OSFlagFlags &= ~OSTCBCur->OSTCBFlagsRdy;
                     break;

#if OS_FLAG_WAIT_CLR_EN > 0
                case OS_FLAG_WAIT_CLR_ALL:
                case OS_FLAG_WAIT_CLR_ANY:                 /* Set   ONLY the flags we got              */
					//只置位我们得到的。
                     pgrp->OSFlagFlags |= OSTCBCur->OSTCBFlagsRdy;
                     break;
#endif
            }
        }
        flags_cur = pgrp->OSFlagFlags;//此函数获取事件标志组的当前事件标志状态，
        //并返回给调用状态
        OS_EXIT_CRITICAL();
        *err      = OS_NO_ERR;  /* Event(s) must have occurred              */
    }
    return (flags_cur);
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                         POST EVENT FLAG BIT(S)
*
* Description: This function is called to set or clear some bits in an event flag group.  The bits to
*              set or clear are specified by a 'bit mask'.
*
* Arguments  : pgrp          is a pointer to the desired event flag group.
*
*              flags         If 'opt' (see below) is OS_FLAG_SET, each bit that is set in 'flags' will
*                            set the corresponding bit in the event flag group.  e.g. to set bits 0, 4
*                            and 5 you would set 'flags' to:
*
*                                0x31     (note, bit 0 is least significant bit)
*
*                            If 'opt' (see below) is OS_FLAG_CLR, each bit that is set in 'flags' will
*                            CLEAR the corresponding bit in the event flag group.  e.g. to clear bits 0,
*                            4 and 5 you would specify 'flags' as:
*
*                                0x31     (note, bit 0 is least significant bit)
*
*              opt           indicates whether the flags will be:
*                                set     (OS_FLAG_SET) or
*                                cleared (OS_FLAG_CLR)
*
*              err           is a pointer to an error code and can be:
*                            OS_NO_ERR              The call was successfull
*                            OS_FLAG_INVALID_PGRP   You passed a NULL pointer
*                            OS_ERR_EVENT_TYPE      You are not pointing to an event flag group
*                            OS_FLAG_INVALID_OPT    You specified an invalid option
*
* Returns    : the new value of the event flags bits that are still set.
*
* Called From: Task or ISR
*
* WARNING(s) : 1) The execution time of this function depends on the number of tasks waiting on the event
*                 flag group.
*              2) The amount of time interrupts are DISABLED depends on the number of tasks waiting on
*                 the event flag group.
                                                置位或清0事件标志组中的事件标志
描述：此函数设置或清除事件标志组中的一些位，通过设置掩码来完成。
参数：pgrp：目标事件标志组指针
                flags：如果opt是OS_FLAG_SET，flags中的每一位将设置事件标志组中的对应位。
                如：要设置0、4、5位，那么flags为0x31。如果'opt' (see below) 是 OS_FLAG_CLR，那么
                flags中的每一位置位将清除事件标志组中的对应位。
                如：清除0、4、5位，那么flags为0x31。
*              opt          显示flags是否为：
*                                set     (OS_FLAG_SET) 或者
*                                cleared (OS_FLAG_CLR)
*
*              err           is a pointer to an error code and can be:
*                            OS_NO_ERR              成功
*                            OS_FLAG_INVALID_PGRP   你传递了空指针
*                            OS_ERR_EVENT_TYPE     没有指向事件标志组
*                            OS_FLAG_INVALID_OPT    有错误选项
*********************************************************************************************************
*/
OS_FLAGS  OSFlagPost (OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U opt, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                          /* Allocate storage for CPU status register       */
    OS_CPU_SR     cpu_sr;
#endif
    OS_FLAG_NODE *pnode;
    BOOLEAN       sched;
    OS_FLAGS      flags_cur;
    OS_FLAGS      flags_rdy;


#if OS_ARG_CHK_EN > 0
    if (pgrp == (OS_FLAG_GRP *)0) {                  /* Validate 'pgrp'                                */
        *err = OS_FLAG_INVALID_PGRP;
        return ((OS_FLAGS)0);//pgrp是否合理
    }
    if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {    /* Make sure we are pointing to an event flag grp */
        *err = OS_ERR_EVENT_TYPE;
        return ((OS_FLAGS)0);//保证是指向事件标志组
    }
#endif
/*$PAGE*/
    OS_ENTER_CRITICAL();
    switch (opt) {
        case OS_FLAG_CLR://如果是负反馈，设置相反项
             pgrp->OSFlagFlags &= ~flags;            /* Clear the flags specified in the group         */
             break;

        case OS_FLAG_SET://如果是正反馈，设置相同项
             pgrp->OSFlagFlags |=  flags;            /* Set   the flags specified in the group         */
             break;

        default://有异常现象
             OS_EXIT_CRITICAL();                     /* INVALID option                                 */
             *err = OS_FLAG_INVALID_OPT;
             return ((OS_FLAGS)0);
    }
    sched = FALSE;                                   /* Indicate that we don't need rescheduling       */
	//初始化假设不会导致更高优先级任务进入就绪态，所以不需要调度
    pnode = (OS_FLAG_NODE *)pgrp->OSFlagWaitList;//取出等待列表中的第一个结点
    while (pnode != (OS_FLAG_NODE *)0) {             /* Go through all tasks waiting on event flag(s)  */
		//如果有任务等待，遍历事件标志中所有任务
        switch (pnode->OSFlagNodeWaitType) {//等待的类型是什么
            case OS_FLAG_WAIT_SET_ALL:               /* See if all req. flags are set for current node */
                 flags_rdy = pgrp->OSFlagFlags & pnode->OSFlagNodeFlags;
                 if (flags_rdy == pnode->OSFlagNodeFlags) {//如果完全和任务等待的事件标志位满足，
                     if (OS_FlagTaskRdy(pnode, flags_rdy) == TRUE) { /* Make task RTR, event(s) Rx'd   */
					 	//那么使任务进入就绪状态
                         sched = TRUE;                               /* When done we will reschedule   */
            //并不是每检查一个OSFLAG_NODE就进行一次任务调度，而是在遍历完全部等待任务后
            //进行一次总的任务调度。
                     }
                 }
                 break;

            case OS_FLAG_WAIT_SET_ANY:               /* See if any flag set                            */
				//任意一个指定 的事件标志位置位
                 flags_rdy = pgrp->OSFlagFlags & pnode->OSFlagNodeFlags;
                 if (flags_rdy != (OS_FLAGS)0) {//只要不为零（有位就行）就执行
                     if (OS_FlagTaskRdy(pnode, flags_rdy) == TRUE) { /* Make task RTR, event(s) Rx'd   */
                         sched = TRUE;                               /* When done we will reschedule   */
                     }
                 }
                 break;

#if OS_FLAG_WAIT_CLR_EN > 0
            case OS_FLAG_WAIT_CLR_ALL:               /* See if all req. flags are set for current node */
				//指定事件标志全部清零
                 flags_rdy = ~pgrp->OSFlagFlags & pnode->OSFlagNodeFlags;
                 if (flags_rdy == pnode->OSFlagNodeFlags) {
                     if (OS_FlagTaskRdy(pnode, flags_rdy) == TRUE) { /* Make task RTR, event(s) Rx'd   */
                         sched = TRUE;                               /* When done we will reschedule   */
                     }
                 }
                 break;

            case OS_FLAG_WAIT_CLR_ANY:               /* See if any flag set                            */
				//任意一个指定的事件标志清零
                 flags_rdy = ~pgrp->OSFlagFlags & pnode->OSFlagNodeFlags;
                 if (flags_rdy != (OS_FLAGS)0) {
                     if (OS_FlagTaskRdy(pnode, flags_rdy) == TRUE) { /* Make task RTR, event(s) Rx'd   */
                         sched = TRUE;                               /* When done we will reschedule   */
                     }
                 }
                 break;
#endif
        }
        pnode = (OS_FLAG_NODE *)pnode->OSFlagNodeNext; /* Point to next task waiting for event flag(s) */
		//通过双向链表得到下一个OSFlagNode

    }
    OS_EXIT_CRITICAL();
    if (sched == TRUE) {
		//如果布尔变量为真，进行任务调度
        OS_Sched();
    }
    OS_ENTER_CRITICAL();
    flags_cur = pgrp->OSFlagFlags;//返回当前事件标志组的事件标志状态
    OS_EXIT_CRITICAL();
    *err      = OS_NO_ERR;
    return (flags_cur);
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                           QUERY EVENT FLAG
*
* Description: This function is used to check the value of the event flag group.
*
* Arguments  : pgrp         is a pointer to the desired event flag group.
*
*              err           is a pointer to an error code returned to the called:
*                            OS_NO_ERR              The call was successfull
*                            OS_FLAG_INVALID_PGRP   You passed a NULL pointer
*                            OS_ERR_EVENT_TYPE      You are not pointing to an event flag group
*
* Returns    : The current value of the event flag group.
*
* Called From: Task or ISR
                                                         查询事件标志组的状态
描述：用于查询事件标志组的状态
参数： pgrp：指向目标事件标志组的状态
                  err：返回给调用函数的出错代码指针：
*                            OS_NO_ERR              调用成功
*                            OS_FLAG_INVALID_PGRP   你传递了空指针
*                            OS_ERR_EVENT_TYPE      你没有指向一个事件标志组                    
*********************************************************************************************************
*/

#if OS_FLAG_QUERY_EN > 0//包含 OSFlagQuery()代码
OS_FLAGS  OSFlagQuery (OS_FLAG_GRP *pgrp, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                       /* Allocate storage for CPU status register          */
    OS_CPU_SR  cpu_sr;
#endif
    OS_FLAGS   flags;


#if OS_ARG_CHK_EN > 0
    if (pgrp == (OS_FLAG_GRP *)0) {               /* Validate 'pgrp'                                   */
        *err = OS_FLAG_INVALID_PGRP;
        return ((OS_FLAGS)0);//不合理的pgrp
    }
    if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) { /* Validate event block type                         */
        *err = OS_ERR_EVENT_TYPE;
        return ((OS_FLAGS)0);//不是事件标志类型
    }
#endif
    OS_ENTER_CRITICAL();
    flags = pgrp->OSFlagFlags;//获取当前事件标志的状态
    OS_EXIT_CRITICAL();
    *err = OS_NO_ERR;
    return (flags);      /* Return the current value of the event flags       */
	//返回状态标志给调用函数
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                         SUSPEND TASK UNTIL EVENT FLAG(s) RECEIVED OR TIMEOUT OCCURS
*
* Description: This function is internal to uC/OS-II and is used to put a task to sleep until the desired
*              event flag bit(s) are set.
*
* Arguments  : pgrp          is a pointer to the desired event flag group.
*
*              pnode         is a pointer to a structure which contains data about the task waiting for
*                            event flag bit(s) to be set.
*
*              flags         Is a bit pattern indicating which bit(s) (i.e. flags) you wish to check.
*                            The bits you want are specified by setting the corresponding bits in
*                            'flags'.  e.g. if your application wants to wait for bits 0 and 1 then
*                            'flags' would contain 0x03.
*
*              wait_type     specifies whether you want ALL bits to be set/cleared or ANY of the bits
*                            to be set/cleared.
*                            You can specify the following argument:
*
*                            OS_FLAG_WAIT_CLR_ALL   You will check ALL bits in 'mask' to be clear (0)
*                            OS_FLAG_WAIT_CLR_ANY   You will check ANY bit  in 'mask' to be clear (0)
*                            OS_FLAG_WAIT_SET_ALL   You will check ALL bits in 'mask' to be set   (1)
*                            OS_FLAG_WAIT_SET_ANY   You will check ANY bit  in 'mask' to be set   (1)
*
*              timeout       is the desired amount of time that the task will wait for the event flag
*                            bit(s) to be set.
*
* Returns    : none
*
* Called by  : OSFlagPend()  OS_FLAG.C
*
* Note(s)    : This function is INTERNAL to uC/OS-II and your application should not call it.
                                           挂起任务直到事件标志来到或者超时
描述：它是一个ucos内部函数，将一个任务睡眠直到目标标志位置位
参数：pgrp：指向目标事件控制块的指针
                 pnode：指向等待事件标志位置位的任务的数据的结构指针
                 flags：是一个位模式，包含想检验的位，通过设置flags中相应位来设置你想要
                             的位。比如：如果应用中想等待0和1位，那么flags就为0x03。
                 wait_type：标志你想所有位设置/清零还是任何位设置/清零
                 你可以设成以下参数：
*                            OS_FLAG_WAIT_CLR_ALL   You will check ALL bits in 'mask' to be clear (0)
*                            OS_FLAG_WAIT_CLR_ANY   You will check ANY bit  in 'mask' to be clear (0)
*                            OS_FLAG_WAIT_SET_ALL   You will check ALL bits in 'mask' to be set   (1)
*                            OS_FLAG_WAIT_SET_ANY   You will check ANY bit  in 'mask' to be set   (1)
                 timeout：任务等待事件标志位的目标时间
*********************************************************************************************************
*/

static  void  OS_FlagBlock (OS_FLAG_GRP *pgrp, OS_FLAG_NODE *pnode, OS_FLAGS flags, INT8U wait_type, INT16U timeout)
{
    OS_FLAG_NODE  *pnode_next;


    OSTCBCur->OSTCBStat      |= OS_STAT_FLAG;
    OSTCBCur->OSTCBDly        = timeout;              /* Store timeout in task's TCB                   */
	//在TCB中保存超时时间
#if OS_TASK_DEL_EN > 0//允许包含任务删除代码
    OSTCBCur->OSTCBFlagNode   = pnode;                /* TCB to link to node                           */
//	将OSTCBFlagNode数据结构保存在TCB的一个链接中，前提是OS_TASK_DEL_EN为1，可以根据这个
//链接把要删除的任务从对应的事件标志组的等待列表中删除。删除工作由其它函数完成。
#endif
    pnode->OSFlagNodeFlags    = flags;                /* Save the flags that we need to wait for       */
//保存任务等待标志组中指定事件标志位和等待方式的信息，
    pnode->OSFlagNodeWaitType = wait_type;            /* Save the type of wait we are doing            */
//保存我们等待的方式 
    pnode->OSFlagNodeTCB      = (void *)OSTCBCur;     /* Link to task's TCB                            */
//将当前任务列入等待事件的TCB
    pnode->OSFlagNodeNext     = pgrp->OSFlagWaitList; /* Add node at beginning of event flag wait list */
//把新增的OS_FLAG_NODE的添加到双向链表的开始端
    pnode->OSFlagNodePrev     = (void *)0;
    pnode->OSFlagNodeFlagGrp  = (void *)pgrp;         /* Link to Event Flag Group                      */
	//将此事件标志组反向链接到OS_FLAG_NODE的事件标志组中，当删除一个任务时，
	//需要根据这个链接把被删除的任务从对应的事件标志组的等待任务列表中删除
    pnode_next                = (OS_FLAG_NODE *)pgrp->OSFlagWaitList;
	//把前面一个OS_FLAG_NODE指针链接到新添加的OS_FLAG_NODE
    if (pnode_next != (void *)0) {                    /* Is this the first NODE to insert?             */
        pnode_next->OSFlagNodePrev = pnode;           /* No, link in doubly linked list                */
    }
    pgrp->OSFlagWaitList = (void *)pnode;//第一个结点就变成pnode了
                                                      /* Suspend current task until flag(s) received   */
    if ((OSRdyTbl[OSTCBCur->OSTCBY] &= ~OSTCBCur->OSTCBBitX) == 0) {
        OSRdyGrp &= ~OSTCBCur->OSTCBBitY;//挂起当前任务直到收到事件标志
    }
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                    INITIALIZE THE EVENT FLAG MODULE
*
* Description: This function is called by uC/OS-II to initialize the event flag module.  Your application
*              MUST NOT call this function.  In other words, this function is internal to uC/OS-II.
*
* Arguments  : none
*
* Returns    : none
*
* WARNING    : You MUST NOT call this function from your code.  This is an INTERNAL function to uC/OS-II.
                                                    初始化事件标志模型
描述：由ucos调用初始化事件标志模块，应用程序不能调用
参数：无
返回：无
备注：是内部函数，应用程序不能调用
*********************************************************************************************************
*/

void  OS_FlagInit (void)
{
#if OS_MAX_FLAGS == 1//只有一个事件标志组
    OSFlagFreeList                 = (OS_FLAG_GRP *)&OSFlagTbl[0];  /* Only ONE event flag group!      */
//只有一个组
    OSFlagFreeList->OSFlagType     = OS_EVENT_TYPE_UNUSED;
//初始化设置成没有使用
    OSFlagFreeList->OSFlagWaitList = (void *)0;
//没有任务在等它
#endif

#if OS_MAX_FLAGS >= 2//多个事件标志组
    INT8U        i;
    OS_FLAG_GRP *pgrp1;
    OS_FLAG_GRP *pgrp2;


    pgrp1 = &OSFlagTbl[0];
    pgrp2 = &OSFlagTbl[1];
    for (i = 0; i < (OS_MAX_FLAGS - 1); i++) {                      /* Init. list of free EVENT FLAGS  */
		//初始化空闲的事件标志
        pgrp1->OSFlagType     = OS_EVENT_TYPE_UNUSED;//设置成没有使用
        pgrp1->OSFlagWaitList = (void *)pgrp2;//把它们链起来
        pgrp1++;
        pgrp2++;
    }
    pgrp1->OSFlagWaitList = (void *)0;//没有任务等它们
    OSFlagFreeList        = (OS_FLAG_GRP *)&OSFlagTbl[0];//表头
#endif
}

/*$PAGE*/
/*
*********************************************************************************************************
*                              MAKE TASK READY-TO-RUN, EVENT(s) OCCURRED
*
* Description: This function is internal to uC/OS-II and is used to make a task ready-to-run because the
*              desired event flag bits have been set.
*
* Arguments  : pnode         is a pointer to a structure which contains data about the task waiting for
*                            event flag bit(s) to be set.
*
*              flags_rdy     contains the bit pattern of the event flags that cause the task to become
*                            ready-to-run.
*
* Returns    : none
*
* Called by  : OSFlagsPost() OS_FLAG.C
*
* Note(s)    : 1) This function assumes that interrupts are disabled.
*              2) This function is INTERNAL to uC/OS-II and your application should not call it.
                                         //事件发生，使任务就绪
描述：Ucos内部函数，用于目标事件标志位置位了，使任务就绪
参数：pnode：指向包含等待事件标志位置位的任务数据的指针
                 flags_rdy：包含事件标志的位模式，它将引起事件就绪
返回：无
由OSFlagsPost() OS_FLAG.C调用
备注：1、调用此函数要关中断
                 2、内部函数，应用程序不能调用
*********************************************************************************************************
*/

static  BOOLEAN  OS_FlagTaskRdy (OS_FLAG_NODE *pnode, OS_FLAGS flags_rdy)
{
    OS_TCB   *ptcb;
    BOOLEAN   sched;


    ptcb                = (OS_TCB *)pnode->OSFlagNodeTCB;  /* Point to TCB of waiting task             */
	//取出指向等待事件的PCB指针
    ptcb->OSTCBDly      = 0;//不延时
    ptcb->OSTCBFlagsRdy = flags_rdy;//将需要就绪任务的标志取出来
    ptcb->OSTCBStat    &= ~OS_STAT_FLAG;//消除事件标志组中挂起状态
    if (ptcb->OSTCBStat == OS_STAT_RDY) {      /* Put task into ready list                 *///交它放入就绪列表
        OSRdyGrp               |= ptcb->OSTCBBitY;
        OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;//加入就绪列表，具体算法见BLOG
        sched                   = TRUE;//需要任务调度
    } else {//否则不需要任务调度
        sched                   = FALSE;
    }
    OS_FlagUnlink(pnode);//将结点从等待列表中删除
    return (sched);
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                  UNLINK EVENT FLAG NODE FROM WAITING LIST
*
* Description: This function is internal to uC/OS-II and is used to unlink an event flag node from a
*              list of tasks waiting for the event flag.
*
* Arguments  : pnode         is a pointer to a structure which contains data about the task waiting for
*                            event flag bit(s) to be set.
*
* Returns    : none
*
* Called by  : OS_FlagTaskRdy() OS_FLAG.C
*              OSFlagPend()     OS_FLAG.C
*              OSTaskDel()      OS_TASK.C
*
* Note(s)    : 1) This function assumes that interrupts are disabled.
*              2) This function is INTERNAL to uC/OS-II and your application should not call it.
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                     将事件标志结点从等待列表中删除
描述：这个函数是ucos内部的，用于将事件标志结点和等待事件标志的一系列任务解链。
参数：pnode:它是一个结构指针，包含任务等待事件标志位将被设置的数据。
返回：无
   由下面函数调用： OS_FlagTaskRdy() OS_FLAG.C
*                                               OSFlagPend()     OS_FLAG.C
*                                               OSTaskDel()      OS_TASK.C
备注：1、这个函数假定中断不能使
                2、此函数属于ucos内部，应用函数不能调用。

*********************************************************************************************************
*/


void  OS_FlagUnlink (OS_FLAG_NODE *pnode)
{
#if OS_TASK_DEL_EN > 0//允许 包含任务删除代码
    OS_TCB       *ptcb;//任务控制块
#endif
    OS_FLAG_GRP  *pgrp;//事件控制群
    OS_FLAG_NODE *pnode_prev;//事件标志等待链表接点前端
    OS_FLAG_NODE *pnode_next;//事件标志等待链表接点后端


    pnode_prev = (OS_FLAG_NODE *)pnode->OSFlagNodePrev;//等待链表中等待结点指针前端赋给pnode_prev
    pnode_next = (OS_FLAG_NODE *)pnode->OSFlagNodeNext;//等待链表中等待结点指针后端赋给pnode_next
    if (pnode_prev == (OS_FLAG_NODE *)0) {                      /* Is it first node in wait list?      */
		//如果是第一个结点
        pgrp                 = (OS_FLAG_GRP *)pnode->OSFlagNodeFlagGrp;
		//等待链表中事件控制群指针赋给事件控制群
        pgrp->OSFlagWaitList = (void *)pnode_next;              /*      Update list for new 1st node   */
		//事件控制群中下一个指针置于等待链表的第一个结点指针
        if (pnode_next != (OS_FLAG_NODE *)0) {
            pnode_next->OSFlagNodePrev = (OS_FLAG_NODE *)0;     /*      Link new 1st node PREV to NULL */
			//新结点的前端指向空指针
        }
    } else {                                                    /* No,  A node somewhere in the list   */
    //如果不是第一个结点    
        pnode_prev->OSFlagNodeNext = pnode_next;                /*      Link around the node to unlink */
	//前端的后端为pnode_next
        if (pnode_next != (OS_FLAG_NODE *)0) {                  /*      Was this the LAST node?        */
			//如果不是是后一个结点
            pnode_next->OSFlagNodePrev = pnode_prev;            /*      No, Link around current node   */
			//后端的前端为pnode的前端
        }
    }
#if OS_TASK_DEL_EN > 0//能便删除任务代码
    ptcb                = (OS_TCB *)pnode->OSFlagNodeTCB;//等待任务控制块指针赋给任务控制块
    ptcb->OSTCBFlagNode = (OS_FLAG_NODE *)0;//事件标志结点清零
#endif
}
#endif
