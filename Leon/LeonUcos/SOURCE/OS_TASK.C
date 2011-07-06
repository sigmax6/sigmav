/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                            TASK MANAGEMENT
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File : OS_TASK.C
* By   : Jean J. Labrosse
* 翻译： likee
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

/*
*********************************************************************************************************
*                                        CHANGE PRIORITY OF A TASK
*
* Description: This function allows you to change the priority of a task dynamically.  Note that the new
*              priority MUST be available.
*
* Arguments  : oldp     is the old priority
*
*              newp     is the new priority
*
* Returns    : OS_NO_ERR        is the call was successful
*              OS_PRIO_INVALID  if the priority you specify is higher that the maximum allowed
*                               (i.e. >= OS_LOWEST_PRIO)
*              OS_PRIO_EXIST    if the new priority already exist.
*              OS_PRIO_ERR      there is no task with the specified OLD priority (i.e. the OLD task does
*                               not exist.
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                   改变任务优先级
描述：此函数允许你动态改变一个任务的优先级。但新的优先级必须可行
参数：oldp ：旧的优先级
                 newp：新的优先级
返回：OS_NO_ERR：改变成功
                OS_PRIO_INVALID：指定的优先级不合法：超过最大值
                OS_PRIO_EXIST：新优先级已经存在
                OS_PRIO_ERR：旧优先级任务不存在

*********************************************************************************************************
*/


#if OS_TASK_CHANGE_PRIO_EN > 0     //如果OS_TASK_CHANGE_PRIO_EN设置为1,能使包含下面代码
INT8U  OSTaskChangePrio (INT8U oldprio, INT8U newprio)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
                                                 //为CPU状态寄存器分配存储器
    OS_CPU_SR    cpu_sr;     //OS_CPU_SR即为unsigned int 
#endif

#if OS_EVENT_EN > 0
//OS_EVENT_EN 定义为：(((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0))
//OS_EVENT_EN 定义为：能使队列代码产生&&申请队列控制块最大数不为零||能使邮箱代码产生||
//能使信号量代码产生||能使互斥量代码产生 
    OS_EVENT    *pevent;
#endif

    OS_TCB      *ptcb;
    INT8U        x;
    INT8U        y;
    INT8U        bitx;
    INT8U        bity;



#if OS_ARG_CHK_EN > 0//允许参数检测
    if ((oldprio >= OS_LOWEST_PRIO && oldprio != OS_PRIO_SELF)  ||
         newprio >= OS_LOWEST_PRIO) {//旧新优先级都不合法
        return (OS_PRIO_INVALID);
    }
#endif
    OS_ENTER_CRITICAL();//如果合法
    if (OSTCBPrioTbl[newprio] != (OS_TCB *)0) {                 /* New priority must not already exist */
        OS_EXIT_CRITICAL();
        return (OS_PRIO_EXIST);//新优先级必须不存在,存在就重复了
    } else {
        OSTCBPrioTbl[newprio] = (OS_TCB *)1;                    /* Reserve the entry to prevent others */
		//保留入口,防止其它任务占用此优先级
        OS_EXIT_CRITICAL();
        y    = newprio >> 3;                                    /* Precompute to reduce INT. latency   */
		//此函数会预先计算新优先级任务的任务控制块中的某些值,使用这些值
		//可以将任务放入就绪步或者从该表中移除任务.
        bity = OSMapTbl[y];
        x    = newprio & 0x07;
        bitx = OSMapTbl[x];
        OS_ENTER_CRITICAL();
        if (oldprio == OS_PRIO_SELF) {                          /* See if changing self                */
			//如果改变自己
            oldprio = OSTCBCur->OSTCBPrio;                      /* Yes, get priority                   */
        }//是的,得到优先级
        ptcb = OSTCBPrioTbl[oldprio];//得到该优先级TCB指针
        if (ptcb != (OS_TCB *)0) {                              /* Task to change must exist           */
			//优先级存在,如果要改变的是当前任务,由一定会成功
            OSTCBPrioTbl[oldprio] = (OS_TCB *)0;                /* Remove TCB from old priority        */
		//通过放入空闲指针,将指向当前任务的TCB指针从优先级列表中删除,
		//使当前旧的优先级空闲,可以被其它任务占用.
            if ((OSRdyTbl[ptcb->OSTCBY] & ptcb->OSTCBBitX) != 0x00) {  /* If task is ready make it not */
                if ((OSRdyTbl[ptcb->OSTCBY] &= ~ptcb->OSTCBBitX) == 0x00) {	//如果要改变优先级的任务就绪
                    OSRdyGrp &= ~ptcb->OSTCBBitY;//不能让它就绪,
                }
                OSRdyGrp    |= bity;                            /* Make new priority ready to run      */
                OSRdyTbl[y] |= bitx;//从就绪表中移除,然后在新优先级下,将任务插入就绪表,
                //注意:OSTaskChangePrio是利用预先计算的值(见前面)将任务插入就绪表中的.
#if OS_EVENT_EN > 0
//#define  OS_EVENT_EN       (((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0))
			//能使队列代码产生&&申请队列控制块最大数不为零||能使邮箱代码产生||
			//能使信号量代码产生||能使互斥量代码产生

            } else {
                pevent = ptcb->OSTCBEventPtr;
                if (pevent != (OS_EVENT *)0) {                  /* Remove from event wait list  */
	//如果任务没有就绪,那么可能在等一个信号量,一个互斥型信号量,一个邮箱,队列
	//等,如果OSTCBEventPtr非空,那么此函数会知道任务正在等以上的某件事.
                    if ((pevent->OSEventTbl[ptcb->OSTCBY] &= ~ptcb->OSTCBBitX) == 0) {
                        pevent->OSEventGrp &= ~ptcb->OSTCBBitY;
                    }
                    pevent->OSEventGrp    |= bity;              /* Add new priority to wait list       */
	//如果任务正在等某事件发生,OSTCBEventPtr必须将任务从事件控制块的等待队列(旧
	//的优先级下)中移除,并在新的优先级下将事件插入到等待队列中.任务也可能正
	//在等待延时时间到,或被挂起,上面几行可以省略
                    pevent->OSEventTbl[y] |= bitx;
                }
#endif
            }
            OSTCBPrioTbl[newprio] = ptcb;                       /* Place pointer to TCB @ new priority */
			//将指向任务的OS-TCB的指针存到OSTCBPrioTbl[]中.
            ptcb->OSTCBPrio       = newprio;                    /* Set new task priority               */
			//新的优先级保存在OSTCB中,预先值也保存在OSTCB中.
            ptcb->OSTCBY          = y;
            ptcb->OSTCBX          = x;
            ptcb->OSTCBBitY       = bity;
            ptcb->OSTCBBitX       = bitx;
            OS_EXIT_CRITICAL();
            OS_Sched();                                         /* Run highest priority task ready     */
            //任务调度,运行最高优先级任务,在新的优先级高于旧的优先有或者新的优先级高于调
            //用此函数任务优先级的时候,此函数会被调用
            return (OS_NO_ERR);
        } else {
            OSTCBPrioTbl[newprio] = (OS_TCB *)0;                /* Release the reserved prio.          */
			//如果任务不存在,释放新优先级的TCB
            OS_EXIT_CRITICAL();
            return (OS_PRIO_ERR);                               /* Task to change didn't exist         */
        }//返回
    }
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                            CREATE A TASK
*
* Description: This function is used to have uC/OS-II manage the execution of a task.  Tasks can either
*              be created prior to the start of multitasking or by a running task.  A task cannot be
*              created by an ISR.
*
* Arguments  : task     is a pointer to the task's code
*
*              pdata    is a pointer to an optional data area which can be used to pass parameters to
*                       the task when the task first executes.  Where the task is concerned it thinks
*                       it was invoked and passed the argument 'pdata' as follows:
*
*                           void Task (void *pdata)
*                           {
*                               for (;;) {
*                                   Task code;
*                               }
*                           }
*
*              ptos     is a pointer to the task's top of stack.  If the configuration constant
*                       OS_STK_GROWTH is set to 1, the stack is assumed to grow downward (i.e. from high
*                       memory to low memory).  'pstk' will thus point to the highest (valid) memory
*                       location of the stack.  If OS_STK_GROWTH is set to 0, 'pstk' will point to the
*                       lowest memory location of the stack and the stack will grow with increasing
*                       memory locations.
*
*              prio     is the task's priority.  A unique priority MUST be assigned to each task and the
*                       lower the number, the higher the priority.
*
* Returns    : OS_NO_ERR        if the function was successful.
*              OS_PRIO_EXIT     if the task priority already exist
*                               (each task MUST have a unique priority).
*              OS_PRIO_INVALID  if the priority you specify is higher that the maximum allowed
*                               (i.e. >= OS_LOWEST_PRIO)
*********************************************************************************************************
*/
/*
*********************************************************************************************
                                                           建立一个任务
 描述：这个函数用于ucosII处理完成一个任务，它要么在多任务处理之前建立，
                  要么运行任务建立，它不够由中断服务程序建立。
参数：task: 指向任务代码的指针。
                 pdata:是一个指向非强制性数据区域的指针，当任务优先运行时传递
                 参数给任务。任务有关部分假想它被唤醒，然后按照以下方式传递pdata：
*                           void Task (void *pdata)
*                           {
*                               for (;;) {
*                                   Task code;
*                               }
*                           }
                ptos：指向任务堆栈顶部的指针，如果配置常数 OS_STK_GROWTH 设置为1的话，堆栈则会由高到低增长（由高地址向低地址存储）。所以
“pstk”会指向堆栈存储器位置的最高地址；如果 OS_STK_GROWTH 设置为0的话，“pstk”将指向堆栈
最低存储器位置，堆栈将按存储器位置递增。
             prio：是任务的优先级，一个独特的优先级必须指定给每个任务，最小的数对应最高优先级。

                 返回：OS_NO_ERR   ：如果函数成功。
                                  OS_PRIO_EXIT ：如果优先级已经存在。
                                  OS_PRIO_INVALID：如果定义优先级的数大于最大值
                 
*********************************************************************************************
*/

#if OS_TASK_CREATE_EN > 0   //能使包含任务创建代码
INT8U  OSTaskCreate (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT8U prio)//上面有说明
{
#if OS_CRITICAL_METHOD == 3                  /* Allocate storage for CPU status register               */
                                //  为CPU状态寄存器分配存储空间
    OS_CPU_SR  cpu_sr;         //CPU状态字是十六位 OS_CPU_SR为unsigned int 
#endif
    OS_STK    *psp;
    INT8U      err;


#if OS_ARG_CHK_EN > 0
//如果OS_ARG_CHK_EN 设为1，OSTaskCreate会检查分配给任务的优先级是否有效。
//系统在执行初始化的时候，已经把最低优先级分配给了空闲任务。
//所以不能用最低优先级来创建任务。
    if (prio > OS_LOWEST_PRIO) {             /* Make sure priority is within allowable range           */
		                                 //保证优先级在允许范围内
        return (OS_PRIO_INVALID);
    }
#endif
    OS_ENTER_CRITICAL();                  //进入临界状态
    if (OSTCBPrioTbl[prio] == (OS_TCB *)0) { /* Make sure task doesn't already exist at this priority  */
		                                   //保证优先级没有被其它任务占用
        OSTCBPrioTbl[prio] = (OS_TCB *)1;    /* Reserve the priority to prevent others from doing ...  */
                                             /* ... the same thing until task is created.              */
								//放置一个非空指针，表示已经占用
        OS_EXIT_CRITICAL();        //退出临界状态
        psp = (OS_STK *)OSTaskStkInit(task, pdata, ptos, 0);    /* Initialize the task's stack         */
		                     //初始化任务堆栈,即建立任务堆栈
        err = OS_TCBInit(prio, psp, (OS_STK *)0, 0, 0, (void *)0, 0);
							 //初始化任务控制块，从空闲的OS_TCB缓冲池
							 //中获得并初始化一个任务控制块
        if (err == OS_NO_ERR) {         //如果初始化没有错
            OS_ENTER_CRITICAL();//进入临界状态
            OSTaskCtr++;                                        /* Increment the #tasks counter        */
			//任务数量加一
            OS_EXIT_CRITICAL();//退出临界状态
            if (OSRunning == TRUE) {         /* Find highest priority task if multitasking has started */
				//如果多任务开始，寻找最高优先级任务
                OS_Sched();
            }
        } else {              //如果初始化任务控制块有错
            OS_ENTER_CRITICAL();//进入临界状态
            OSTCBPrioTbl[prio] = (OS_TCB *)0;/* Make this priority available to others                 */
			//把这一优先级给其它任务
            OS_EXIT_CRITICAL();//退出临界状态
        }
        return (err);//返回错误信息
    }
    OS_EXIT_CRITICAL();//如果优先级占用，退出临界状态
    return (OS_PRIO_EXIST);//返回优先级存在
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                     CREATE A TASK (Extended Version)
*
* Description: This function is used to have uC/OS-II manage the execution of a task.  Tasks can either
*              be created prior to the start of multitasking or by a running task.  A task cannot be
*              created by an ISR.  This function is similar to OSTaskCreate() except that it allows
*              additional information about a task to be specified.
*
* Arguments  : task     is a pointer to the task's code
*
*              pdata    is a pointer to an optional data area which can be used to pass parameters to
*                       the task when the task first executes.  Where the task is concerned it thinks
*                       it was invoked and passed the argument 'pdata' as follows:
*
*                           void Task (void *pdata)
*                           {
*                               for (;;) {
*                                   Task code;
*                               }
*                           }
*
*              ptos     is a pointer to the task's top of stack.  If the configuration constant
*                       OS_STK_GROWTH is set to 1, the stack is assumed to grow downward (i.e. from high
*                       memory to low memory).  'pstk' will thus point to the highest (valid) memory
*                       location of the stack.  If OS_STK_GROWTH is set to 0, 'pstk' will point to the
*                       lowest memory location of the stack and the stack will grow with increasing
*                       memory locations.  'pstk' MUST point to a valid 'free' data item.
*
*              prio     is the task's priority.  A unique priority MUST be assigned to each task and the
*                       lower the number, the higher the priority.
*
*              id       is the task's ID (0..65535)
*
*              pbos     is a pointer to the task's bottom of stack.  If the configuration constant
*                       OS_STK_GROWTH is set to 1, the stack is assumed to grow downward (i.e. from high
*                       memory to low memory).  'pbos' will thus point to the LOWEST (valid) memory
*                       location of the stack.  If OS_STK_GROWTH is set to 0, 'pbos' will point to the
*                       HIGHEST memory location of the stack and the stack will grow with increasing
*                       memory locations.  'pbos' MUST point to a valid 'free' data item.
*
*              stk_size is the size of the stack in number of elements.  If OS_STK is set to INT8U,
*                       'stk_size' corresponds to the number of bytes available.  If OS_STK is set to
*                       INT16U, 'stk_size' contains the number of 16-bit entries available.  Finally, if
*                       OS_STK is set to INT32U, 'stk_size' contains the number of 32-bit entries
*                       available on the stack.
*
*              pext     is a pointer to a user supplied memory location which is used as a TCB extension.
*                       For example, this user memory can hold the contents of floating-point registers
*                       during a context switch, the time each task takes to execute, the number of times
*                       the task has been switched-in, etc.
*
*              opt      contains additional information (or options) about the behavior of the task.  The
*                       LOWER 8-bits are reserved by uC/OS-II while the upper 8 bits can be application
*                       specific.  See OS_TASK_OPT_??? in uCOS-II.H.
*
* Returns    : OS_NO_ERR        if the function was successful.
*              OS_PRIO_EXIT     if the task priority already exist
*                               (each task MUST have a unique priority).
*              OS_PRIO_INVALID  if the priority you specify is higher that the maximum allowed
*                               (i.e. > OS_LOWEST_PRIO)
*********************************************************************************************************
*/
/*
*****************************************************************************
                                                     建立一个任务的扩展版函数
描述：这个函数用于ucosII处理完成一个任务，它要么在多任务处理之前建立，
				 要么运行任务建立，它不够由中断服务程序建立。
                   它与OSTaskCreate()功能相似，但它允许一个特殊任务的更多信息。
参数：task: 指向任务代码的指针
      pdata：任务开始执行时，传递给任务参数的指针，用法见上。
      ptos：分配给任务堆栈的栈顶指针
      prio:分配给任务的优先级
      id:为任务创建一个特殊标志符，扩展版本有用。在这里设为和优先级一样即可。
      pbos：指向任务堆栈栈底指针，用于堆栈检验
      stk_size:用于指定堆栈容量。例：如果堆栈入口宽度为4B，那么stk_size为1000，
               说明堆栈有4000B
      pext：指向用户附加的数据域指针，用来扩展任务的任务控制块OS_TCB
      opt：设定OSTaskCreateExt的选项，指定是否允许堆栈检验，是否将堆栈清零，是否
           进行浮点数操作等。具体见uCOS_II中每一位定义。只要将相应位与opt相或即可
返回：OS_NO_ERR   ：如果函数成功。
		  OS_PRIO_EXIT ：如果优先级已经存在。
		  OS_PRIO_INVALID：如果定义优先级的数大于最大值



*****************************************************************************
*/
/*$PAGE*/
#if OS_TASK_CREATE_EXT_EN > 0 //能使包含创建代码
INT8U  OSTaskCreateExt (void   (*task)(void *pd),
                        void    *pdata,
                        OS_STK  *ptos,
                        INT8U    prio,
                        INT16U   id,
                        OS_STK  *pbos,
                        INT32U   stk_size,
                        void    *pext,
                        INT16U   opt)//具体含义见上
{
#if OS_CRITICAL_METHOD == 3                  /* Allocate storage for CPU status register               */
//为CPU状态寄存器分配存储空间
    OS_CPU_SR  cpu_sr;
#endif
    OS_STK    *psp;//任务堆栈参数指针
    INT8U      err;//错误参数


#if OS_ARG_CHK_EN > 0//允许检验什么东西，没有看懂
    if (prio > OS_LOWEST_PRIO) {             /* Make sure priority is within allowable range           */
		//优先级超出范围
        return (OS_PRIO_INVALID);//返回优先级不可行
    }
#endif
    OS_ENTER_CRITICAL();//如果可行，进入临界状态
    if (OSTCBPrioTbl[prio] == (OS_TCB *)0) { /* Make sure task doesn't already exist at this priority  */
		//保证此优先级不存在
        OSTCBPrioTbl[prio] = (OS_TCB *)1;    /* Reserve the priority to prevent others from doing ...  */
                                             /* ... the same thing until task is created.              */
											 //保留优先级，说明此优先级已经占用
        OS_EXIT_CRITICAL();//退出临界状态，这样可以使此函数在定义任务数据结构
                            //其它部分的时候，能够开中断

        if (((opt & OS_TASK_OPT_STK_CHK) != 0x0000) ||   /* See if stack checking has been enabled     */
			//允许检验堆栈
            ((opt & OS_TASK_OPT_STK_CLR) != 0x0000)) {   /* See if stack needs to be cleared           */
            //任务建立时清除堆栈
            #if OS_STK_GROWTH == 1//堆栈从低到高增长
            (void)memset(pbos, 0, stk_size * sizeof(OS_STK));
			//memset是一个标准的ANSI函数，编译器商家会使之最优化
            #else//这个函数具体是什么不知道。
            (void)memset(ptos, 0, stk_size * sizeof(OS_STK));
            #endif
        }

        psp = (OS_STK *)OSTaskStkInit(task, pdata, ptos, opt); /* Initialize the task's stack          */
		//初始化任务堆栈
        err = OS_TCBInit(prio, psp, pbos, id, stk_size, pext, opt);
		//任务控制块初始化
        if (err == OS_NO_ERR) {//如果成功则OS_TCBInit返回OS_NO_ERR
            OS_ENTER_CRITICAL();//进入临界状态
            OSTaskCtr++;                                       /* Increment the #tasks counter         */
			//创建任务数加一
            OS_EXIT_CRITICAL();//退出临界状态
            if (OSRunning == TRUE) {                           /* Find HPT if multitasking has started */
				//如果多任务运行的话，检查高优先级
                OS_Sched();//任务调度
            }
        } else {//如果任务控制块初始化失败
            OS_ENTER_CRITICAL();
            OSTCBPrioTbl[prio] = (OS_TCB *)0;                  /* Make this priority avail. to others  */
			//将这一优先级给其它任务
            OS_EXIT_CRITICAL();
        }
        return (err);//如果不允许检验堆栈
    }
    OS_EXIT_CRITICAL();
    return (OS_PRIO_EXIST);//如果此优先级存在，返回。
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                            DELETE A TASK
*
* Description: This function allows you to delete a task.  The calling task can delete itself by
*              its own priority number.  The deleted task is returned to the dormant state and can be
*              re-activated by creating the deleted task again.
*
* Arguments  : prio    is the priority of the task to delete.  Note that you can explicitely delete
*                      the current task without knowing its priority level by setting 'prio' to
*                      OS_PRIO_SELF.
*
* Returns    : OS_NO_ERR           if the call is successful
*              OS_TASK_DEL_IDLE    if you attempted to delete uC/OS-II's idle task
*              OS_PRIO_INVALID     if the priority you specify is higher that the maximum allowed
*                                  (i.e. >= OS_LOWEST_PRIO) or, you have not specified OS_PRIO_SELF.
*              OS_TASK_DEL_ERR     if the task you want to delete does not exist
*              OS_TASK_DEL_ISR     if you tried to delete a task from an ISR
*
* Notes      : 1) To reduce interrupt latency, OSTaskDel() 'disables' the task:
*                    a) by making it not ready
*                    b) by removing it from any wait lists
*                    c) by preventing OSTimeTick() from making the task ready to run.
*                 The task can then be 'unlinked' from the miscellaneous structures in uC/OS-II.
*              2) The function OS_Dummy() is called after OS_EXIT_CRITICAL() because, on most processors,
*                 the next instruction following the enable interrupt instruction is ignored.  
*              3) An ISR cannot delete a task.
*              4) The lock nesting counter is incremented because, for a brief instant, if the current
*                 task is being deleted, the current task would not be able to be rescheduled because it
*                 is removed from the ready list.  Incrementing the nesting counter prevents another task
*                 from being schedule.  This means that an ISR would return to the current task which is
*                 being deleted.  The rest of the deletion would thus be able to be completed.
*********************************************************************************************************
*/
/*
*****************************************************************************************************
                                                                 删除一个任务
描述：此函数允许你删除一个任务，这个正在调用的任务能通过它自己的优先级数删除它自己，
                 被删除的任务返回睡眠状态，它能通过创建一个已删除任务再次激活。
参数：prio：被删除任务的优先级，当将prio设成OS_PRIO_SELF时，我们不知道它的优先级也
                   能够删除它
返回：OS_NO_ERR:删除成功
                 OS_TASK_DEL_IDLE：如果试图删除空闲任务
                 OS_PRIO_INVALID：如果指定优先级高过允许值
                 OS_TASK_DEL_ERR：如果想删除的任务不存在
                 OS_TASK_DEL_ISR：如果想从中断服务程序中删除任务
备注：1、为减少中断延时，OSTaskDel()通过三个手段不能使任务：
                   a、指定它没有就绪b、从等待列表中删除c、在OSTimeTick()中设置阻止任务准备运行。
                然后任务会被从ucos的复杂结构中解链
                    2、函数OS_Dummy() 在OS_EXIT_CRITICAL()调用之后调用是因为在多数处理器中，在能使中断
                       指令的下一条指令将被忽略。
                      3、一个中断服务程序不能删除一个任务
                     4、锁定嵌套数增加是因为作为一个即时启动，如果当前任务被删除，这个当前任务不能重新被
                       安排因为它已经从就绪列表中删除。增加嵌套数防止另一个任务被调度。这意味着中断服务程序
                    将返回被删除的当前任务，余下的删除工作将被中止。


*********************************************************************************************************
*/

/*$PAGE*/
#if OS_TASK_DEL_EN > 0//允许包含任务删除代码
INT8U  OSTaskDel (INT8U prio)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR     cpu_sr;//为CPU状态寄存器分配存储空间
#endif

#if OS_EVENT_EN > 0
	//能使队列代码产生&&申请队列控制块最大数不为零||能使邮箱代码产生||
	//能使信号量代码产生||能使互斥量代码产生 

    OS_EVENT     *pevent;//OS_EVENT：事件控制块
#endif    
#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
//OS版本大于等于251，能使事件标志，事件标志最在值大于零
    OS_FLAG_NODE *pnode;//事件标志等待列表
#endif
    OS_TCB       *ptcb;//任务控制块
    BOOLEAN       self;//#define BOOLEAN  unsigned  char



    if (OSIntNesting > 0) {                                     /* See if trying to delete from ISR    */
		//看是不是要从ISR中删除
        return (OS_TASK_DEL_ISR);//返回
    }
#if OS_ARG_CHK_EN > 0//允许参数检测？
    if (prio == OS_IDLE_PRIO) {                                 /* Not allowed to delete idle task     */
		//保证删除的不是空闲任务
        return (OS_TASK_DEL_IDLE);
    }
    if (prio >= OS_LOWEST_PRIO && prio != OS_PRIO_SELF) {       /* Task priority valid ?               */
		//如果被删除任务优先级违法
        return (OS_PRIO_INVALID);
    }
#endif
    OS_ENTER_CRITICAL();//进入临界状态
    if (prio == OS_PRIO_SELF) {                                 /* See if requesting to delete self    */
		//如果删除本身
        prio = OSTCBCur->OSTCBPrio;                             /* Set priority to delete to current   */
		//设置当前删除任务状态优先级
    }
    ptcb = OSTCBPrioTbl[prio];//取出已建立TCB的指针表
    if (ptcb != (OS_TCB *)0) {                                       /* Task to delete must exist      */
		//如果不为空，表示指针存在
        if ((OSRdyTbl[ptcb->OSTCBY] &= ~ptcb->OSTCBBitX) == 0x00) {  /* Make task not ready            */
            OSRdyGrp &= ~ptcb->OSTCBBitY;//就绪表中清空，保证不被挂起。
        }//如果任务在就绪表中，则将其在就绪表中删除
#if OS_EVENT_EN > 0
		//能使队列代码产生&&申请队列控制块最大数不为零||能使邮箱代码产生||
		//能使信号量代码产生||能使互斥量代码产生 

        pevent = ptcb->OSTCBEventPtr;//将事件控制块指针放入事件控制块
        if (pevent != (OS_EVENT *)0) {                          /* If task is waiting on event         */
   //如果任务在事件中等待。即如果任务处于互斥型信号量，邮箱，
   //消息队列或者信号量的等待表中它就从自己所处的表中被去除
            if ((pevent->OSEventTbl[ptcb->OSTCBY] &= ~ptcb->OSTCBBitX) == 0) { /* ... remove task from */
                pevent->OSEventGrp &= ~ptcb->OSTCBBitY;                        /* ... event ctrl block */
            }//将任务从事件控制块中删除，
        }
#endif
#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
		//OS版本大于等于251，能使事件标志，事件标志最在值大于零

        pnode = ptcb->OSTCBFlagNode;//事件标志结合点赋给事件标志控制块
        //如果任务处于事件标志的等待表中，就会从此表中删除
        if (pnode != (OS_FLAG_NODE *)0) {                       /* If task is waiting on event flag    */
			//如果任务在事件标志中等待
            OS_FlagUnlink(pnode);                               /* Remove from wait list               */
			//从等待列表中删除
        }
#endif
        ptcb->OSTCBDly  = 0;                                    /* Prevent OSTimeTick() from updating  */
//删除任务后，将任务时钟节拍延迟数清0，确保自己重新开中断
//时，中断服务子程序ISR不会使该任务就绪
        ptcb->OSTCBStat = OS_STAT_RDY;                          /* Prevent task from being resumed     */
//任务状态设为就绪态，OSTaskDel()置任务的OSTCBStat标志为OS_STAT_RDY
//OSTaskDel()并不试图使任务处于就绪态，只是阻止其它任务或者中断服务
//程序让该任务重新开始执行（通过调用OSTaskResume()）.若不设置任务的OSTCBStat标
//志为OS_STAT_RDY，也可以清除OS_STAT_SUSPEND() 。
		if (OSLockNesting < 255) {//多任务处理结点嵌套数小于255
            OSLockNesting++;//嵌套数加一
		}
//至此，被删除的任务就不会被其它任务或者中断服务子程序置于就绪态
//因为该任务已经从就绪任务表中被删了，为了在室删除任务目的，任务
//被置于休眠状态，正因为任务处于休眠状态，OSTaskDel()要防止任务调度
//程序在删除过程中切换到其它任务中去，因为当前任务如果正在被删除，
//是不可能被再次调度。
        OS_EXIT_CRITICAL();                                     /* Enabling INT. ignores next instruc. */
//重新开中断，以缩短响应时间，这样，OSTaskDel()就能够处理中断了，但是
//由于给OSLockNesting加了一，ISR执行完后，会返回到被中断了的任务，从而
//继续任务的删除工作。
        OS_Dummy();                                             /* ... Dummy ensures that INTs will be */
//开中断指令到来后要再执行一条指令才真正开中断，所以执行一条空指令。
        OS_ENTER_CRITICAL();                                    /* ... disabled HERE!                  */
		//能使中断，忽略下一条指令，虚假函数保证了中断在这里不能使
		if (OSLockNesting > 0) {//如果多任务处理结点嵌套大于零
            OSLockNesting--;//嵌套数减一
		}//可以继续执行删除工作了，在OSTaskDel()重新关中断后，它通过锁定嵌套
		 //计数器减一，重新允许任务调度。
        OSTaskDelHook(ptcb);                                    /* Call user defined hook              */
		 //调用自定义删除任务接口函数，可在这里删除或者释放自定义的
		 //TCB附加数据域
        OSTaskCtr--;                                            /* One less task being managed         */
		 //任务计数器减一，表明管理的任务减少了一个。
        OSTCBPrioTbl[prio] = (OS_TCB *)0;                       /* Clear old priority entry            */
		 //OSTaskDel()函数简单地指向被删除任务的TCB的指针置为NULL，就从优先级
		 //表中把OS_TCB给删除了。
        if (ptcb->OSTCBPrev == (OS_TCB *)0) {                   /* Remove from TCB chain               */
		//从以OSTCBList开头的OS_TCB双向链表中删除被删任务的TCB，
            ptcb->OSTCBNext->OSTCBPrev = (OS_TCB *)0;//如果是表头
            OSTCBList                  = ptcb->OSTCBNext;
        } else {//如果不是表头
            ptcb->OSTCBPrev->OSTCBNext = ptcb->OSTCBNext;
            ptcb->OSTCBNext->OSTCBPrev = ptcb->OSTCBPrev;
        }//这里没有必要检验表尾的情况，因为不可能删除空闲任务
        ptcb->OSTCBNext = OSTCBFreeList;                        /* Return TCB to free TCB list         */
		//被删任务的OS_TCB被退回到空闲的OS_TCB中，供其它任务使用
        OSTCBFreeList   = ptcb;//ptcb变成新的表头
        OS_EXIT_CRITICAL();
        OS_Sched();                                             /* Find new highest priority task      */
        //任务调度，看此函数开中断时，ESR时否曾使更高优先级的任务进入
        //了就绪态
        return (OS_NO_ERR);//删除成功
    }
    OS_EXIT_CRITICAL();
    return (OS_TASK_DEL_ERR);//如果指针为空，删除出现错误
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                    REQUEST THAT A TASK DELETE ITSELF
*
* Description: This function is used to:
*                   a) notify a task to delete itself.
*                   b) to see if a task requested that the current task delete itself.
*              This function is a little tricky to understand.  Basically, you have a task that needs
*              to be deleted however, this task has resources that it has allocated (memory buffers,
*              semaphores, mailboxes, queues etc.).  The task cannot be deleted otherwise these
*              resources would not be freed.  The requesting task calls OSTaskDelReq() to indicate that
*              the task needs to be deleted.  Deleting of the task is however, deferred to the task to
*              be deleted.  For example, suppose that task #10 needs to be deleted.  The requesting task
*              example, task #5, would call OSTaskDelReq(10).  When task #10 gets to execute, it calls
*              this function by specifying OS_PRIO_SELF and monitors the returned value.  If the return
*              value is OS_TASK_DEL_REQ, another task requested a task delete.  Task #10 would look like
*              this:
*
*                   void Task(void *data)
*                   {
*                       .
*                       .
*                       while (1) {
*                           OSTimeDly(1);
*                           if (OSTaskDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ) {
*                               Release any owned resources;
*                               De-allocate any dynamic memory;
*                               OSTaskDel(OS_PRIO_SELF);
*                           }
*                       }
*                   }
*
* Arguments  : prio    is the priority of the task to request the delete from
*
* Returns    : OS_NO_ERR          if the task exist and the request has been registered
*              OS_TASK_NOT_EXIST  if the task has been deleted.  This allows the caller to know whether
*                                 the request has been executed.
*              OS_TASK_DEL_IDLE   if you requested to delete uC/OS-II's idle task
*              OS_PRIO_INVALID    if the priority you specify is higher that the maximum allowed
*                                 (i.e. >= OS_LOWEST_PRIO) or, you have not specified OS_PRIO_SELF.
*              OS_TASK_DEL_REQ    if a task (possibly another task) requested that the running task be
*                                 deleted.
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                 任务请求删除自己
描述：此函数功能用于：
          a、通知任务删除自己。
          b、查看是否一个任务请求当前任务要删除它自己。
          这个函数要比较灵活去理解。基本上，你有一个任务需要删除，但是它还有分配的资源（比          如： 存储器缓冲区、信号量、邮箱、队列等），这些任务不能被删除，否则这些资源将被释          放。请求任务调用OSTaskDelReq()去表明当前任务需要被删除，然而删除任务将被延迟，比 
          如：假设任务十将被删除，请求任务（比如五）将调用OSTaskDelReq(10)。当任务十运行的            时候，它将调用此函数中的OS_PRIO_SELF再监视返回值，如果返回的值是OS_TASK_DEL_REQ
          另外一个任务请求删除任务，任务十将看成这样：
*                   void Task(void *data)
*                   {
*                       .
*                       .
*                       while (1) {
*                           OSTimeDly(1);
*                           if (OSTaskDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ) {
*                               Release any owned resources;
*                               De-allocate any dynamic memory;
*                               OSTaskDel(OS_PRIO_SELF);
*                           }
*                       }
*                   }//不明白什么意思
参数：   prio：请求删除任务的优先级
返回值： OS_NO_ERR :如果任务存在，且请求通过。
             OS_TASK_NOT_EXIST：如果任务已经删除，这需要请求者知道请求是否已经执行
             OS_TASK_DEL_IDLE ：如果要删除空闲任务
             OS_PRIO_INVALID：如果优先级数值大于最大允许值，或者不是指向OS_PRIO_SELF.
             OS_TASK_DEL_REQ：如果一个任务（也许是其它任务）请求正在运行的任务要删除。

*********************************************************************************************************
*/


/*$PAGE*/
#if OS_TASK_DEL_EN > 0
INT8U  OSTaskDelReq (INT8U prio)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;//分配CPU状态寄存器存储空间
#endif
    BOOLEAN    stat;//typedef unsigned char BOOLEAN
    INT8U      err;
    OS_TCB    *ptcb;


#if OS_ARG_CHK_EN > 0//允许参数检测
    if (prio == OS_IDLE_PRIO) {                                 /* Not allowed to delete idle task     */
        return (OS_TASK_DEL_IDLE);//如果想删除空闲任务，这是不允许的。
    }
    if (prio >= OS_LOWEST_PRIO && prio != OS_PRIO_SELF) {       /* Task priority valid ?               */
        return (OS_PRIO_INVALID);//优先级不合法
    }
#endif
    if (prio == OS_PRIO_SELF) {                                 /* See if a task is requesting to ...  */
        OS_ENTER_CRITICAL();                                    /* ... this task to delete itself      */
        stat = OSTCBCur->OSTCBDelReq;                           /* Return request status to caller     */
		//如果想删除自己，那么返回请求状态到调用者
        OS_EXIT_CRITICAL();
        return (stat);//那么返回请求状态到调用者
    }
    OS_ENTER_CRITICAL();
    ptcb = OSTCBPrioTbl[prio];//已创建TCB指针表
    if (ptcb != (OS_TCB *)0) {                                  /* Task to delete must exist           */
		//如果需要删除的任务存在
        ptcb->OSTCBDelReq = OS_TASK_DEL_REQ;                    /* Set flag indicating task to be DEL. */
     //如果用优先级而不是用OS_PRIO_SELF指定任务，且任务存在，设置标志表示即将删除
        err               = OS_NO_ERR;//返回成功
    } else {
        err               = OS_TASK_NOT_EXIST;                  /* Task must be deleted                */
    }//需要删除的任务不存在，表明任务可以已经删除自己了
    OS_EXIT_CRITICAL();
    return (err);//其它错误
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                        RESUME A SUSPENDED TASK
*
* Description: This function is called to resume a previously suspended task.  This is the only call that
*              will remove an explicit task suspension.
*
* Arguments  : prio     is the priority of the task to resume.
*
* Returns    : OS_NO_ERR                if the requested task is resumed
*              OS_PRIO_INVALID          if the priority you specify is higher that the maximum allowed
*                                       (i.e. >= OS_LOWEST_PRIO)
*              OS_TASK_RESUME_PRIO      if the task to resume does not exist
*              OS_TASK_NOT_SUSPENDED    if the task to resume has not been suspended
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                               恢复任务：
描述：这个函数功能是去恢复一个先前挂起的任务，这个函数只有在去除挂起时候调用。
          被挂起的任务只有通过调用它才能够被恢复。
参数：prio： 需要恢复函数的优先级
返回：OS_NO_ERR：如果请求的任务被恢复
          OS_PRIO_INVALID：如果优先级无效
          OS_TASK_RESUME_PRIO：如果要恢复的任务优先级不存在
          OS_TASK_NOT_SUSPENDED：如果要恢复的任务没有被挂起

*********************************************************************************************************
*/



#if OS_TASK_SUSPEND_EN > 0//允许包含任务挂起和任务恢复代码
INT8U  OSTaskResume (INT8U prio)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;//分配CPU状态寄存器存储空间
#endif
    OS_TCB    *ptcb;


#if OS_ARG_CHK_EN > 0
    if (prio >= OS_LOWEST_PRIO) {                               /* Make sure task priority is valid    */
        return (OS_PRIO_INVALID);//保证任务优先级有效
    }
#endif
    OS_ENTER_CRITICAL();
    ptcb = OSTCBPrioTbl[prio];//调用此任务的TCB指针
    if (ptcb == (OS_TCB *)0) {                                  /* Task to suspend must exist          */
        OS_EXIT_CRITICAL();//如果任务不存在
        return (OS_TASK_RESUME_PRIO);
    }
    if ((ptcb->OSTCBStat & OS_STAT_SUSPEND) != OS_STAT_RDY) {              /* Task must be suspended   */
		//任务必须存在且被挂起
        if (((ptcb->OSTCBStat &= ~OS_STAT_SUSPEND) == OS_STAT_RDY) &&      /* Remove suspension        */
		//是通过清除OSTCBStat域中的OS_STAT_SUSPEND位而取消挂起的
             (ptcb->OSTCBDly  == 0)) {                                     /* Must not be delayed      */
       //要使任务处于就绪态，OSTCBDly须为0。因为没有任何标志表明任务
       //正在等待延迟时间到
            OSRdyGrp               |= ptcb->OSTCBBitY;                     /* Make task ready to run   */
            OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;//当以上两个条件满足时，任务才处于就绪状态
            OS_EXIT_CRITICAL();
            OS_Sched();//任务调度会检查被恢复的任务拥有的优先级是否比调用本函数的任务
                      //优先级高。
        } else {
            OS_EXIT_CRITICAL();
        }
        return (OS_NO_ERR);//恢复成功
    }
    OS_EXIT_CRITICAL();
    return (OS_TASK_NOT_SUSPENDED);//如果任务没有被挂起
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                             STACK CHECKING
*
* Description: This function is called to check the amount of free memory left on the specified task's
*              stack.
*
* Arguments  : prio     is the task priority
*
*              pdata    is a pointer to a data structure of type OS_STK_DATA.
*
* Returns    : OS_NO_ERR           upon success
*              OS_PRIO_INVALID     if the priority you specify is higher that the maximum allowed
*                                  (i.e. > OS_LOWEST_PRIO) or, you have not specified OS_PRIO_SELF.
*              OS_TASK_NOT_EXIST   if the desired task has not been created
*              OS_TASK_OPT_ERR     if you did NOT specified OS_TASK_OPT_STK_CHK when the task was created
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                                                 堆栈检验
描述：这个函数用于检验指定任务堆栈的剩余存储空间数量
参数：prio：任务优先级
           pdata：OS_STK_DATA结构类型的数据指针。
返回：OS_NO_ERR ：成功。
            OS_PRIO_INVALID：优先级数值大于最大值或者没有指定OS_PRIO_SELF
            OS_TASK_NOT_EXIST：如果指定任务没有被创建
            OS_TASK_OPT_ERR：如果任务创建时没有指定OS_TASK_OPT_STK_CHK

*********************************************************************************************************
*/


#if OS_TASK_CREATE_EXT_EN > 0
INT8U  OSTaskStkChk (INT8U prio, OS_STK_DATA *pdata)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif
    OS_TCB    *ptcb;
    OS_STK    *pchk;
    INT32U     free;
    INT32U     size;


#if OS_ARG_CHK_EN > 0
    if (prio > OS_LOWEST_PRIO && prio != OS_PRIO_SELF) {        /* Make sure task priority is valid    */
        return (OS_PRIO_INVALID);//如果OS_ARG_CHK_EN设置为1，优先级值大于最大值且不是 OS_PRIO_SELF
    }
#endif
    pdata->OSFree = 0;                                          /* Assume failure, set to 0 size       */
    pdata->OSUsed = 0;//初始化为0
    OS_ENTER_CRITICAL();
    if (prio == OS_PRIO_SELF) {                        /* See if check for SELF                        */
        prio = OSTCBCur->OSTCBPrio;//如果想知道当前任务堆栈信息
    }
    ptcb = OSTCBPrioTbl[prio];//提取优先级，如果非零，代表任务存在
    if (ptcb == (OS_TCB *)0) {                         /* Make sure task exist                         */
        OS_EXIT_CRITICAL();
        return (OS_TASK_NOT_EXIST);//任务不存在
    }
    if ((ptcb->OSTCBOpt & OS_TASK_OPT_STK_CHK) == 0) { /* Make sure stack checking option is set       */
		//要保证允许检验，要保证已经创建了任务，并传递了参数OS_TASK_OPT_STK_CHK
        //如果建立任务的是OSTaskCreate(),而不是OSTaskCreateExt(),那么因为参数opt为零，
        //所以检验失败。
        OS_EXIT_CRITICAL();
        return (OS_TASK_OPT_ERR);
    }
    free = 0;
    size = ptcb->OSTCBStkSize;//如果所有条件满足，OSTaskStkChk就会像前面描述那样，从堆栈栈底
                              //开始统计堆栈的空闲空间，直到发现一个储存值非零的堆栈入口。
    pchk = ptcb->OSTCBStkBottom;
    OS_EXIT_CRITICAL();
#if OS_STK_GROWTH == 1//如果堆栈设置成从高到低增长
    while (*pchk++ == (OS_STK)0) {                    /* Compute the number of zero entries on the stk */
        free++;//计算空块
    }
#else//如果从低到高增长
    while (*pchk-- == (OS_STK)0) {
        free++;
    }
#endif
    pdata->OSFree = free * sizeof(OS_STK);            /* Compute number of free bytes on the stack     */
//计算空堆栈字节
    pdata->OSUsed = (size - free) * sizeof(OS_STK);   /* Compute number of bytes used on the stack     */
//计算已用堆栈字节
    return (OS_NO_ERR);
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                            SUSPEND A TASK
*
* Description: This function is called to suspend a task.  The task can be the calling task if the
*              priority passed to OSTaskSuspend() is the priority of the calling task or OS_PRIO_SELF.
*
* Arguments  : prio     is the priority of the task to suspend.  If you specify OS_PRIO_SELF, the
*                       calling task will suspend itself and rescheduling will occur.
*
* Returns    : OS_NO_ERR                if the requested task is suspended
*              OS_TASK_SUSPEND_IDLE     if you attempted to suspend the idle task which is not allowed.
*              OS_PRIO_INVALID          if the priority you specify is higher that the maximum allowed
*                                       (i.e. >= OS_LOWEST_PRIO) or, you have not specified OS_PRIO_SELF.
*              OS_TASK_SUSPEND_PRIO     if the task to suspend does not exist
*
* Note       : You should use this function with great care.  If you suspend a task that is waiting for
*              an event (i.e. a message, a semaphore, a queue ...) you will prevent this task from
*              running when the event arrives.
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                    挂起一个任务
描述：调用此函数去挂起一个任务，如果传送到OSTaskSuspend()的任务的优先级是要挂起的任务或者是
           OS_PRIO_SELF，那么这个任务将被挂起。
参数： prio：需要挂起任务的优先级。如果指定OS_PRIO_SELF，那么这个任务将自己挂起，再发生再
                 次调度。
返回：OS_NO_ERR：如果请求的任务被挂起。
            OS_TASK_SUSPEND_IDLE：如果想挂起空闲任务
            OS_PRIO_INVALID  ：想挂起任务优先级不合理
            OS_TASK_SUSPEND_PRIO：需要挂起的任务不存在。
备注：调用时要十分小心，如果你想挂起一个等待事件（邮箱，消息，队列）的任务，事件到来的时候
            你将阻止这个任务运行。

*********************************************************************************************************
*/



#if OS_TASK_SUSPEND_EN > 0
INT8U  OSTaskSuspend (INT8U prio)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif
    BOOLEAN    self;
    OS_TCB    *ptcb;


#if OS_ARG_CHK_EN > 0
    if (prio == OS_IDLE_PRIO) {                                 /* Not allowed to suspend idle task    */
        return (OS_TASK_SUSPEND_IDLE);//不能挂起空闲任务
    }
    if (prio >= OS_LOWEST_PRIO && prio != OS_PRIO_SELF) {       /* Task priority valid ?               */
        return (OS_PRIO_INVALID);//任务优先级不合理
    }
#endif
    OS_ENTER_CRITICAL();
    if (prio == OS_PRIO_SELF) {                                 /* See if suspend SELF                 */
		//是不是要挂起自己，这样，OSTaskSuspend 会从当前任务的任务控制块中
		//获得当前任务的优先级
        prio = OSTCBCur->OSTCBPrio;
        self = TRUE;
    } else if (prio == OSTCBCur->OSTCBPrio) {                   /* See if suspending self              */
        self = TRUE;//也可以通过指定优先级，挂起调用本函数的任务。这两种情况下，
        //任务调度都被调用，这是为什么要定义局部变量self的原因，该变量在
        //适当的时候会被测试，如果没有挂起调用本函数的任务，OSTaskSus_pend()
        //就没有必要运行任务调度程序，因为调用该函数的任务正在挂起一个
        //优先级比较低的任务
    } else {
        self = FALSE;                                           /* No suspending another task          */
    //不是要挂起自己
    }
    ptcb = OSTCBPrioTbl[prio];//取需要挂起任务的TCB
    if (ptcb == (OS_TCB *)0) {                                  /* Task to suspend must exist          */
        OS_EXIT_CRITICAL();
        return (OS_TASK_SUSPEND_PRIO);//如果要挂起的任务不存在
    }
    if ((OSRdyTbl[ptcb->OSTCBY] &= ~ptcb->OSTCBBitX) == 0x00) { /* Make task not ready                 */
        OSRdyGrp &= ~ptcb->OSTCBBitY;//如果该任务存在，就会从就绪表中去掉。
//要挂起的任务可能不在就绪表中，有可能在等待事件发生或者延迟。要
//挂起的任务在OSRdyTbl[]中对应位已被清除。再次清除该位，比先检验该位是否被
//清除，如果没有清除再清除快得多，所以就没有检验了。
    }
    ptcb->OSTCBStat |= OS_STAT_SUSPEND;                         /* Status of task is 'SUSPENDED'       */
	//终于挂起来了
    OS_EXIT_CRITICAL();
    if (self == TRUE) {                                         /* Context switch only if SELF         */
        OS_Sched();//仅仅在挂起任务自己的情况下才调用任务调度
    }
    return (OS_NO_ERR);
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                            QUERY A TASK
*
* Description: This function is called to obtain a copy of the desired task's TCB.
*
* Arguments  : prio     is the priority of the task to obtain information from.
*
* Returns    : OS_NO_ERR       if the requested task is suspended
*              OS_PRIO_INVALID if the priority you specify is higher that the maximum allowed
*                              (i.e. > OS_LOWEST_PRIO) or, you have not specified OS_PRIO_SELF.
*              OS_PRIO_ERR     if the desired task has not been created
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                                                                  查询一个任务
描述：此函数调用去获得一个指定任务TCB的副本
参数：prio:指定函数的优先级
返回:    OS_NO_ERR:请求的函数被挂起
                 OS_PRIO_INVALID:任务优先级不合法
                 OS_PRIO_ERR如果指定的函数还没有创建
*********************************************************************************************************
*/


#if OS_TASK_QUERY_EN > 0
INT8U  OSTaskQuery (INT8U prio, OS_TCB *pdata)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif
    OS_TCB    *ptcb;


#if OS_ARG_CHK_EN > 0
    if (prio > OS_LOWEST_PRIO && prio != OS_PRIO_SELF) {   /* Task priority valid ?                    */
        return (OS_PRIO_INVALID);//优先级不合法
    }
#endif
    OS_ENTER_CRITICAL();
    if (prio == OS_PRIO_SELF) {                            /* See if suspend SELF                      */
        prio = OSTCBCur->OSTCBPrio;//挂起自己
    }
    ptcb = OSTCBPrioTbl[prio];//不挂起自己
    if (ptcb == (OS_TCB *)0) {                             /* Task to query must exist                 */
		//任务不存在
        OS_EXIT_CRITICAL();
        return (OS_PRIO_ERR);//返回错误
    }
    memcpy(pdata, ptcb, sizeof(OS_TCB));                   /* Copy TCB into user storage area          */
	//任务的TCB复制到用户的存储空间
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif
