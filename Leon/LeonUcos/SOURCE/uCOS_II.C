/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File : uCOS_II.C
* By   : Jean J. Labrosse
* 翻译： likee
* 修订 ： 2010 11 15
*********************************************************************************************************
*/

#define  OS_GLOBALS                           /* 定义 GLOBAL 变量   */
#include "includes.h"                            /* 标准C头文件的打包 */ 


#define  OS_MASTER_FILE                     /* Prevent the following files from including includes.h */
#include "\software\ucos-ii\source\os_core.c"  
#include "\software\ucos-ii\source\os_flag.c"
#include "\software\ucos-ii\source\os_mbox.c"
#include "\software\ucos-ii\source\os_mem.c"
#include "\software\ucos-ii\source\os_mutex.c"
#include "\software\ucos-ii\source\os_q.c"
#include "\software\ucos-ii\source\os_sem.c"
#include "\software\ucos-ii\source\os_task.c"
#include "\software\ucos-ii\source\os_time.c"
