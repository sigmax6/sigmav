/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                            MEMORY MANAGEMENT
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File : OS_MEM.C
* By   : Jean J. Labrosse
* 翻译： likee
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)//允许包含内在管理代码且内在分区数大于0
/*
*********************************************************************************************************
*                                        CREATE A MEMORY PARTITION
*
* Description : Create a fixed-sized memory partition that will be managed by uC/OS-II.
*
* Arguments   : addr     is the starting address of the memory partition
*
*               nblks    is the number of memory blocks to create from the partition.
*
*               blksize  is the size (in bytes) of each block in the memory partition.
*
*               err      is a pointer to a variable containing an error message which will be set by
*                        this function to either:
*
*                        OS_NO_ERR            if the memory partition has been created correctly.
*                        OS_MEM_INVALID_ADDR  you are specifying an invalid address for the memory 
*                                             storage of the partition.
*                        OS_MEM_INVALID_PART  no free partitions available
*                        OS_MEM_INVALID_BLKS  user specified an invalid number of blocks (must be >= 2)
*                        OS_MEM_INVALID_SIZE  user specified an invalid block size
*                                             (must be greater than the size of a pointer)
* Returns    : != (OS_MEM *)0  is the partition was created
*              == (OS_MEM *)0  if the partition was not created because of invalid arguments or, no
*                              free partition is available.
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                           建立一个内存分区
描述：建立一个可由ucosII管理的大小固定的内存块
参数：addr ：内存分区的起始地址
          nblks：从分割区中要建立内在块的数目
          blksize：内在分割区中每块内存的大小（用字节表示）
           err：是由下列函数决定的，包含错误信息的可变指针：
               OS_NO_ERR：如果内存块建立正确
               OS_MEM_INVALID_ADDR：你为分割区的内在空间指定了不可用地址
               OS_MEM_INVALID_PART：没有多余的区域了。
               OS_MEM_INVALID_BLKS：用户指定分割数目不合法（必须大于等于2）
               OS_MEM_INVALID_SIZE：用户指定了非法的内存块大小（必须大于指针的大小）
返回： != (OS_MEM *)0：分区已经建立
            == (OS_MEM *)0：因为非法参数或者没有空余块而没有建立分区

*********************************************************************************************************
*/


OS_MEM  *OSMemCreate (void *addr, INT32U nblks, INT32U blksize, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr;//typedef unsigned int OS_CPU_SR
#endif    
    OS_MEM    *pmem;//内存控制块数据结构
    INT8U     *pblk;
    void     **plink;//指向指针的指针
    INT32U     i;


#if OS_ARG_CHK_EN > 0//允许参数检验
    if (addr == (void *)0) {                          /* Must pass a valid address for the memory part. */
        *err = OS_MEM_INVALID_ADDR;//要为内存块建立一个合法地址，即地址要有效
        return ((OS_MEM *)0);//返回
    }
    if (nblks < 2) {                                  /* Must have at least 2 blocks per partition      */
        *err = OS_MEM_INVALID_BLKS;//内存分块太少了，必须最小两块
        return ((OS_MEM *)0);
    }
    if (blksize < sizeof(void *)) {                   /* Must contain space for at least a pointer      */
        *err = OS_MEM_INVALID_SIZE;//每个内存块至少要容不得得下一个指针，因为
        //同一分区中的所有空余内在块是由指针链串联起来的。
        return ((OS_MEM *)0);
    }
#endif
    OS_ENTER_CRITICAL();
    pmem = OSMemFreeList;                             /* Get next free memory partition                */
	//从系统中的空余内存控制块链表中取得一个内存控制块，该项内存
	//控制块中包含相应内在分区的运行状态信息。
    if (OSMemFreeList != (OS_MEM *)0) {               /* See if pool of free partitions was empty      */
		//内存分区的空链表指针是否为空，如不为空，
        OSMemFreeList = (OS_MEM *)OSMemFreeList->OSMemFreeList;//这是什么意思？将空余块赋给空余内存块？
    }
    OS_EXIT_CRITICAL();
    if (pmem == (OS_MEM *)0) {                        /* See if we have a memory partition             */
        *err = OS_MEM_INVALID_PART;//看是否有多余的内存控制块
        return ((OS_MEM *)0);
    }
    plink = (void **)addr;             /* Create linked list of free memory blocks  当条件都满足时    */
    pblk  = (INT8U *)addr + blksize;//所要建立的内存分区内的所有内存块链按成一个单向
    //链表。因为在单向链表中插入和删除元素都是从链表的顶端开始向下执行的。
    for (i = 0; i < (nblks - 1); i++) {
        *plink = (void *)pblk;
        plink  = (void **)pblk;//这两句为什么要这么写？
        pblk   = pblk + blksize;//这个我懂。
    }
    *plink              = (void *)0;                  /* Last memory block points to NULL              */
	//单向链表从上分到下，最后一块指向零。
	//下面这几句是保存相关的信息
    pmem->OSMemAddr     = addr;                       /* Store start address of memory partition       */
    pmem->OSMemFreeList = addr;                       /* Initialize pointer to pool of free blocks     */
    pmem->OSMemNFree    = nblks;                      /* Store number of free blocks in MCB            */
    pmem->OSMemNBlks    = nblks;
    pmem->OSMemBlkSize  = blksize;                    /* Store block size of each memory blocks        */
	//保存每一块内存块的大小
    *err                = OS_NO_ERR;//没有错误
    return (pmem);//操作完成后，返回指向该内存控制块的指针。该指针在以后对该
    //内存分区的操作中使用。
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                          GET A MEMORY BLOCK
*
* Description : Get a memory block from a partition
*
* Arguments   : pmem    is a pointer to the memory partition control block
*
*               err     is a pointer to a variable containing an error message which will be set by this
*                       function to either:
*
*                       OS_NO_ERR           if the memory partition has been created correctly.
*                       OS_MEM_NO_FREE_BLKS if there are no more free memory blocks to allocate to caller
*                       OS_MEM_INVALID_PMEM if you passed a NULL pointer for 'pmem'
*
* Returns     : A pointer to a memory block if no error is detected
*               A pointer to NULL if an error is detected
*********************************************************************************************************
*/
 /*
 *********************************************************************************************************
                                             分配内存块
 描述：从分割区得到一块内存块
 参数：pmem：一个指向内存分割控制块的指针
			err：可以包含以下错误信息的指针：
					OS_NO_ERR：如果内存分割正确创建
					OS_MEM_NO_FREE_BLKS：没有多余的内存控制块调用
					OS_MEM_INVALID_PMEM：如果传一个空指针给"pmem"
 
 返回：如果无错误则返回一个指向内存控制块的指针，如果有错误返回一个空指针	
备注：应用程序应该知道分配内在大小，并在使用的时候不能超过它的大小
应用程序不用的时候，应当将其释放，重新回到相应的相应的内在分区中
 *********************************************************************************************************
*/


void  *OSMemGet (OS_MEM *pmem, INT8U *err)
{//传递指针pmem，它指向希望从中分配到的内存块的内存分区
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr;
#endif    
    void      *pblk;


#if OS_ARG_CHK_EN > 0
    if (pmem == (OS_MEM *)0) {                        /* Must point to a valid memory partition         */
        *err = OS_MEM_INVALID_PMEM;//内存块地址为零，不可用
        return ((OS_MEM *)0);
    }
#endif
    OS_ENTER_CRITICAL();
    if (pmem->OSMemNFree > 0) {                       /* See if there are any free memory blocks       */
		//看是不是有多余的内存块
        pblk                = pmem->OSMemFreeList;    /* Yes, point to next free memory block          */
		//有多余的，指向向下一个内存块，即删除第一块
        pmem->OSMemFreeList = *(void **)pblk;         /*      Adjust pointer to new free list          */
       //调整新的链表指针
        pmem->OSMemNFree--;                           /*      One less memory block in this partition  */
	   //块数减一
        OS_EXIT_CRITICAL();
        *err = OS_NO_ERR;                             /*      No error                                 */
		//没有错误
        return (pblk);                                /*      Return memory block to caller            */
		//返回得到的内存块
    }
    OS_EXIT_CRITICAL();
    *err = OS_MEM_NO_FREE_BLKS;                       /* No,  Notify caller of empty memory partition  */
	//没有多余的，
    return ((void *)0);                               /*      Return NULL pointer to caller            */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                         RELEASE A MEMORY BLOCK
*
* Description : Returns a memory block to a partition
*
* Arguments   : pmem    is a pointer to the memory partition control block
*
*               pblk    is a pointer to the memory block being released.
*
* Returns     : OS_NO_ERR            if the memory block was inserted into the partition
*               OS_MEM_FULL          if you are returning a memory block to an already FULL memory 
*                                    partition (You freed more blocks than you allocated!)
*               OS_MEM_INVALID_PMEM  if you passed a NULL pointer for 'pmem'
*               OS_MEM_INVALID_PBLK  if you passed a NULL pointer for the block to release.
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                        释放一个内存块
描述：返回一个内存块到分割区中
参数；pmem：指向内存分割控制块的指针
                pblk：指向将被释放内存块的指针
 返回：OS_NO_ERR：如果内存块成功插入到分割区
                  OS_MEM_FULL：如果返回内存块到一个已满的内存分割区中
                  OS_MEM_INVALID_PMEM：如果你传递一个空指针到“pmem”
                  OS_MEM_INVALID_PBLK：如果传一个空指针到将释放的块
备注：此函数并不知道内存块是属于哪个分区的，也就是说，如果用户程序
从一个包含32B内存块的分区中分配了一个内存块，用完后千万不要将一个包含
120B内存块给返回来，因为如果下次一个程序申请120B内存时，只会得到32B
*********************************************************************************************************
*/



INT8U  OSMemPut (OS_MEM  *pmem, void *pblk)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
#if OS_ARG_CHK_EN > 0
    if (pmem == (OS_MEM *)0) {                   /* Must point to a valid memory partition             */
        return (OS_MEM_INVALID_PMEM);//必须是可行的内存分割区
    }
    if (pblk == (void *)0) {                     /* Must release a valid block                         */
        return (OS_MEM_INVALID_PBLK);//必须释放一个可行的内存块
    }
#endif
    OS_ENTER_CRITICAL();
    if (pmem->OSMemNFree >= pmem->OSMemNBlks) {  /* Make sure all blocks not already returned          */
        OS_EXIT_CRITICAL();//检查内存分区是否已满，
        return (OS_MEM_FULL);//如果满了，返回满了
    }
    *(void **)pblk      = pmem->OSMemFreeList;   /* Insert released block into free block list         */
	//如果没有满，就插入
    pmem->OSMemFreeList = pblk;//这是什么意思，出现两次了。它变成新的表头？
    pmem->OSMemNFree++;                          /* One more memory block in this partition            */
	//块数加一
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);                          /* Notify caller that memory block was released       */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                          QUERY MEMORY PARTITION
*
* Description : This function is used to determine the number of free memory blocks and the number of
*               used memory blocks from a memory partition.
*
* Arguments   : pmem    is a pointer to the memory partition control block
*
*               pdata   is a pointer to a structure that will contain information about the memory
*                       partition.
*
* Returns     : OS_NO_ERR            If no errors were found.
*               OS_MEM_INVALID_PMEM  if you passed a NULL pointer for 'pmem'
*               OS_MEM_INVALID_PDATA if you passed a NULL pointer for the block to release.
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                查询一个内存分区状态
描述：查询一亿内存分割区的空内存块和已用内存块
参数：pmem：指向一个内存分割控制块的指针
                 pdata:    将要包含内存分割区信息的结构指针
返回：OS_NO_ERR：没有发现错误
                 OS_MEM_INVALID_PMEM ：如果传送一个空指针给‘pmem’
                 OS_MEM_INVALID_PDATA:如果传一个空指针到将释放的块
*********************************************************************************************************
*/



#if OS_MEM_QUERY_EN > 0
INT8U  OSMemQuery (OS_MEM *pmem, OS_MEM_DATA *pdata)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
#if OS_ARG_CHK_EN > 0
    if (pmem == (OS_MEM *)0) {                   /* Must point to a valid memory partition             */
        return (OS_MEM_INVALID_PMEM);//必须是可行的分割区
    }
    if (pdata == (OS_MEM_DATA *)0) {             /* Must release a valid storage area for the data     */
        return (OS_MEM_INVALID_PDATA);//必须为数据释放一个可行的存储空间
    }//OS_MEM_DATA中存放了特定内存分区中内存块的大小，可用内存块数目和
    //已用内存块数目等信息。
#endif
    OS_ENTER_CRITICAL();
    pdata->OSAddr     = pmem->OSMemAddr;//将指定内存分区的信息全部复制到指定变量的
    pdata->OSFreeList = pmem->OSMemFreeList;//相应区域，此过程中，中断被关，防止在此过程中
    pdata->OSBlkSize  = pmem->OSMemBlkSize;//数据被改
    pdata->OSNBlks    = pmem->OSMemNBlks;
    pdata->OSNFree    = pmem->OSMemNFree;
    OS_EXIT_CRITICAL();
    pdata->OSNUsed    = pdata->OSNBlks - pdata->OSNFree;//计算已用块
    return (OS_NO_ERR);
}
#endif                                           /* OS_MEM_QUERY_EN                                    */
/*$PAGE*/
/*
*********************************************************************************************************
*                                    INITIALIZE MEMORY PARTITION MANAGER
*
* Description : This function is called by uC/OS-II to initialize the memory partition manager.  Your
*               application MUST NOT call this function.
*
* Arguments   : none
*
* Returns     : none
*
* Note(s)    : This function is INTERNAL to uC/OS-II and your application should not call it.
*********************************************************************************************************
*/
/*
*********************************************************************************************************
                                                            初始化内存分割区管理
描述：ucos内部调用，其它应用程序不能调用
参数：无
返回：元
备注：ucos内部调用，其它应用程序不能调用
*********************************************************************************************************
*/



void  OS_MemInit (void)
{
#if OS_MAX_MEM_PART == 1//分割区为一块
    OSMemFreeList                = (OS_MEM *)&OSMemTbl[0]; /* Point to beginning of free list          */
//指向空闲列表开始的指针
    OSMemFreeList->OSMemFreeList = (void *)0;              /* Initialize last node                     */
//初始化最后结点？什么意思？它是指向空内存块指针的
    OSMemFreeList->OSMemAddr     = (void *)0;              /* Store start address of memory partition  */
//保存内存分割区的开始地址
    OSMemFreeList->OSMemNFree    = 0;                      /* No free blocks                           */
//为零表明没有空块
    OSMemFreeList->OSMemNBlks    = 0;                      /* No blocks                                */
//分割区中没有内存块
    OSMemFreeList->OSMemBlkSize  = 0;                      /* Zero size                                */
//大小为零
#endif

#if OS_MAX_MEM_PART >= 2//如果分割区有两块以上
    OS_MEM  *pmem;//内存结构
    INT16U   i;


    pmem = (OS_MEM *)&OSMemTbl[0];                    /* Point to memory control block (MCB)           */
	//指向内存控制块的指针
    for (i = 0; i < (OS_MAX_MEM_PART - 1); i++) {     /* Init. list of free memory partitions          */
		//对每一块内存分割区进行初始化
        pmem->OSMemFreeList = (void *)&OSMemTbl[i+1]; /* Chain list of free partitions                 */
		//内存管理空间链表
        pmem->OSMemAddr     = (void *)0;              /* Store start address of memory partition       */
		//内存分割区存储开始地址
        pmem->OSMemNFree    = 0;                      /* No free blocks                                */
		//无空闲块
        pmem->OSMemNBlks    = 0;                      /* No blocks                                     */
		//无块
        pmem->OSMemBlkSize  = 0;                      /* Zero size                                     */
		//大小为零
        pmem++;//内存控制块加一？
    }
    pmem->OSMemFreeList = (void *)0;                  /* Initialize last node                          */
//初始化最后一个结点，空余内存块列表指针
    pmem->OSMemAddr     = (void *)0;                  /* Store start address of memory partition       */
//开始地址
    pmem->OSMemNFree    = 0;                          /* No free blocks                                */
    pmem->OSMemNBlks    = 0;                          /* No blocks                                     */
    pmem->OSMemBlkSize  = 0;                          /* Zero size                                     */

    OSMemFreeList       = (OS_MEM *)&OSMemTbl[0];     /* Point to beginning of free list               */
	//空表开始指针
#endif
}
#endif                                           /* OS_MEM_EN                                          */
