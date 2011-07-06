/*
    NRF24L01.C    NRF24L01驱动程序 基于YC2440平台
    Sigmax6      2011年3月3日18:51:32
*/

//*系统
#include <linux/init.h>
#include <linux/module.h>         
#include <linux/kernel.h> 
//-功能
#include <linux/fs.h>                  //包含了struct inode 的定义，MINOR、MAJOR的头文件。
#include <linux/delay.h>            //系统提供的延时工具
#include <linux/miscdevice.h>  //包含了miscdevice结构的定义及相关的操作函数。
#include <asm/uaccess.h>       //包含了copy_to_user、copy_from_user等内核访问用户进程内存地址的函数定义。
//!平台
#include <asm/arch/gpio.h>
#include <asm/arch/regs-gpio.h>    //平台相关的GPIO操作工具与变量定义
//=自定义
#include "nrf24l01.h"

//#define DEBUG 1
#define DEBUG2 1

//*LICENSE　指定本模块使用的许可证
MODULE_LICENSE("Dual BSD/GPL");
//-作者信息
MODULE_AUTHOR("sigmax6@live.com");
//-模块叙述
MODULE_DESCRIPTION("nrf24l01 driver for Mini2440");


//============================================================

/**************************************************
函数:int io_nrf24l01_init(void)

描述:
    初始化nrf24l01的IO端口 初始化成功返回1
**************************************************/
int io_nrf24l01_init(void)
{
	//方向和上拉
    MISO_UP;             //MISO上拉
    IRQ_UP;
    CE_OUT;
    CSN_OUT;
    SCK_OUT;
    MOSI_OUT;
    MISO_IN;
    IRQ_IN;
    
    //初始化端口
    udelay(500); 		 //SPI时序延时

    CE_L;
    ndelay(60);
    CSN_H;
    ndelay(60);
    SCK_L;
    ndelay(60);
    IRQ_H;
    ndelay(60);

    nrf24l01_setRx();            //Set to Rx mode
/*    
    //24l01寄存器配置复位
    SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);         // 写本地地址 
    SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);  // 写接收端地址
    SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);                  //  频道0自动 ACK应答允许 
    SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);        //  允许接收地址只有频道0
    SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x0a);         // 自动重发延时等待250us+86us，自动重发10次
    SPI_RW_Reg(WRITE_REG + RF_CH, 40);                        //   设置信道工作为2.4GHZ，选择射频通道0x40,收发必须一致
    SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为4字节
    SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);            //设置发射速率为1MHZ，发射功率为最大值0dB
    SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);               // IRQ收发完成中断响应，16位CRC ，主接收

    CE_H;
i*/
   // mdelay(1000);             //等待配置完成
    
    return 1;
}

/**************************************************
函数：SPI_RW(uchar byte)

描述：
    根据SPI协议，写一字节数据到nRF24L01，同时从nRF24L01
	读出一字节
**************************************************/
uchar SPI_RW(uchar byte)
{
	uchar i;
#ifdef DEBUG
       printk(">> SPI_RW byte to read is %x\n",byte);
#endif

   	for(i=0; i<8; i++)          // 循环8次
   	{
   		if (byte & 0x80)     // byte最高位输出到MOSI
   		{
   			MOSI_H;            //高则置1

#ifdef DEBUG
    printk("MOSI_H\n");
#endif
   		}
   		else
   		{
   			MOSI_L;             //低则置0

#ifdef DEBUG
    printk("MOSI_L\n");
#endif
   		}
   		byte <<= 1;             // 低一位移位到最高位

                udelay(2);                  //Tdc min 2ns

   		SCK_H;                      // 拉高SCK，nRF24L01从MOSI读入1位数据，同时从MISO输出1位数据

#ifdef DEBUG
    printk("SCK_H\n");
#endif


   		byte |= MISO_STU;       	// 读MISO到byte最低位

   		udelay(4);                 //Tch min 40 ns

   		SCK_L;            	// SCK置低
#ifdef DEBUG
    printk("SCK_L\n");
#endif
   		udelay(2);
   	}


#ifdef DEBUG2
//    printk(">> SPI_RW return :%x\n",byte);
#endif
    return(byte);           	// 返回读出的一字节

}

/**************************************************
函数：SPI_RW_Reg()

描述：
    写数据value到reg寄存器
**************************************************/
uchar SPI_RW_Reg(uchar reg, uchar value)
{
	uchar status;

  	CSN_L;                      // CSN置低，开始传输数据
#ifdef DEBUG
    printk("CSN_L\n");
#endif
  	//udelay(3);                 //Tcc min 2ns

  	status = SPI_RW(reg);      // 选择寄存器，同时返回状态字
       // udelay(5);
        ndelay(20);
  	SPI_RW(value);             // 然后写数据到该寄存器

  	CSN_H;                   // CSN拉高，结束数据传输

#ifdef DEBUG
    printk("CSN_H\n");
#endif
  	udelay(10);                //Tcwh min 50ns

  	return(status);            // 返回状态寄存器
}

/**************************************************
函数：SPI_Read()

描述：
    从reg寄存器读一字节
**************************************************/
uchar SPI_Read(uchar reg)
{
	uchar reg_val;

  	CSN_L;                    // CSN置低，开始传输数据
#ifdef DEBUG
    printk("CSN_L\n");
#endif
  	udelay(3);

  	SPI_RW(reg);                // 选择寄存器
       // udelay(5);
        ndelay(20);
  	reg_val = SPI_RW(0);        // 然后从该寄存器读数据

  	CSN_H;                    // CSN拉高，结束数据传输
#ifdef DEBUG
    printk("CSN_H\n");
#endif
  	udelay(10);

  	return(reg_val);            // 返回寄存器数据
}

/**************************************************
函数：SPI_Read_Buf()

描述：
    从reg寄存器读出bytes个字节，通常用来读取接收通道
	数据或接收/发送地址
**************************************************/
uchar SPI_Read_Buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
  	CSN_L;                    // CSN置低，开始传输数据
#ifdef DEBUG
    printk("CSN_L\n");
#endif
  	udelay(3);

  	status = SPI_RW(reg);       // 选择寄存器，同时返回状态字
#ifdef DEBUG
        printk("SPI_Read_Buf sta:%d:",status);
#endif
  	for(i=0; i<bytes; i++)
        {
        	pBuf[i] = SPI_RW(0);    // 逐个字节从nRF24L01读出
                ndelay(20);
          //      udelay(5);
        }

  	CSN_H;                    // CSN拉高，结束数据传输
#ifdef DEBUG
    printk("CSN_H\n");
#endif
  	udelay(10);

  	return(status);             // 返回状态寄存器
}

/**************************************************
函数：SPI_Write_Buf()

描述：
    把pBuf缓存中的数据写入到nRF24L01，通常用来写入发
	射通道数据或接收/发送地址
**************************************************/
uchar SPI_Write_Buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;

  	CSN_L;                                 // CSN置低，开始传输数据

#ifdef DEBUG
    printk("CSN_L\n");
#endif
  	udelay(3);

  	status = SPI_RW(reg);       // 选择寄存器，同时返回状态字
#ifdef DEBUG
        printk("SPI_Write_Buf sta: %d",status);
#endif 
  	for(i=0; i<bytes; i++)
       {
    	    SPI_RW(pBuf[i]);               // 逐个字节写入nRF24L01
            ndelay(20);
//           udelay(5);
       }

  	CSN_H;                              // CSN拉高，结束数据传输
#ifdef DEBUG
    printk("CSN_H\n");
#endif
  	udelay(10);

  	return(status);                    // 返回状态寄存器
}

/**************************************************
函数：nrf24l01_rxPacket(uchar * rx_buf)

描述：
    向rx_buf 指向的缓冲区位置按照RX_PLOAD_WIDTH宽度数据接收
**************************************************/
int nrf24l01_rxPacket(uchar * rx_buf)
{
        int rx_result=0;

	sta=SPI_Read(STATUS);
#ifdef DEBUG
       printk("Statue is %x",sta);
#endif 

	if(sta & (1<<RX_DR) )           //收到数据
    {
#ifdef DEBUG
       printk("Data received!\n");
#endif       
//    	    CE_L;
//    	    udelay(10);           
    	    SPI_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);  //从节点读取数据到RxBuffer
    	    rx_result = 1;
    }

    SPI_RW_Reg(WRITE_REG + STATUS, sta);  // 清除RX_DS中断标志

    return rx_result;

}

/**************************************************
函数：nrf24l01_txPacket(uchar * tx_buf)

描述：
    从tx_buf 指向的缓冲区位置按照TX_PLOAD_WIDTH宽度数据发送
**************************************************/
void nrf24l01_txPacket(uchar * tx_buf)
{
	CE_L;
//	ndelay(60);
        udelay(2);

	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
  	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
  	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);                  // 写数据包到TX FIFO
  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);       // 使能接收通道0自动应答
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);   // 使能接收通道0
  	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x0a);  // 自动重发延时等待250us+86us，自动重发10次
  	SPI_RW_Reg(WRITE_REG + RF_CH, 40);         // 选择射频通道0x40
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
  	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);      // CRC使能，16位CRC校验，上电
	udelay(6);
/*
   	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
   	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
    SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);              // 装载数据 
    SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);            // IRQ收发完成中断响应，16位CRC，主发送
*/
        CE_H; 
        udelay(20);            //激活发送
  }
  
/**************************************************
函数：nrf24l01_setRx()

描述：
    设置节点切换为接收模式
**************************************************/
void nrf24l01_setRx(void)
{
    CE_L;
#ifdef DEBUG
    printk("CE_L\n");
#endif
    udelay(2);
/*
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);         // 写本地地址 
    SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);     // 写接收端地址
    SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);                        //  频道0自动 ACK应答允许 
    SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);        //  允许接收地址只有频道0
    SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x0a);         // 自动重发延时等待250us+86us，自动重发10次
    SPI_RW_Reg(WRITE_REG + RF_CH, 40);                        //   设置信道工作为2.4GHZ，选择射频通道0x40,收发必须一致
    SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为4字节
    SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);            //设置发射速率为1MHZ，发射功率为最大值0dB
    SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);               // IRQ收发完成中断响应，16位CRC ，主接收
*/
    SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);  // 接收设备接收通道0使用和发送设备相同的发送地址
  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);               // 使能接收通道0自动应答
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);           // 使能接收通道0
  	SPI_RW_Reg(WRITE_REG + RF_CH, 40);                 // 选择射频通道0x40
  	SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  // 接收通道0选择和发送通道相同有效数据宽度
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);            // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
  	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);              // CRC使能，16位CRC校验，上电，接收模式

#ifdef DEBUG
    printk("=========================================\n");
    printk("WRITE_REG + EN_AA : %x\n",SPI_Read(WRITE_REG + EN_AA));
	printk("WRITE_REG + EN_RXADDR : %x \n",SPI_Read(WRITE_REG + EN_RXADDR));
    printk("WRITE_REG + RF_CH : %x \n",SPI_Read(WRITE_REG + RF_CH));
	printk("WRITE_REG + RX_PW_P0 : %x \n",SPI_Read(WRITE_REG + RX_PW_P0));
	printk("WRITE_REG + RF_SETUP : %x \n",SPI_Read(WRITE_REG + RF_SETUP));
	printk("WRITE_REG + CONFIG : %x \n",SPI_Read(WRITE_REG + CONFIG));
  	//SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);               // 使能接收通道0自动应答
  	//SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);           // 使能接收通道0
  	//SPI_RW_Reg(WRITE_REG + RF_CH, 40);                 // 选择射频通道0x40
  	//SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  // 接收通道0选择和发送通道相同有效数据宽度
  	//SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);            // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
  	//SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);              // CRC使能，16位CRC校验，上电，接收模式
 #endif
    udelay(6);
    CE_H;
#ifdef DEBUG
    printk("CE_H\n");
#endif
     
    udelay(130);
 


}
//==========================================================

/**************************************************
驱动函数：nrf24l01_write()

描述：
    写设备函数
**************************************************/
static ssize_t nrf24l01_write(struct file *filp, const char *buffer,size_t count, loff_t *ppos)
{
	uchar * TxBuf = 
	(uchar *)kmalloc(((count+TX_PLOAD_WIDTH-1)/TX_PLOAD_WIDTH)*TX_PLOAD_WIDTH * sizeof(uchar),0);                                                                                                            //申请缓冲区作为发送缓冲
	if (copy_from_user(TxBuf,buffer,count))                           //从用户空间复制发送内容 如果成功返回0
	{
		printk(DEVICE_NAME "Can't get user data!\n");              //若失败 则错误退出
		kfree(TxBuf);
		return -EFAULT;
	}
	else
	{
		int countDevide = (count+TX_PLOAD_WIDTH-1)/TX_PLOAD_WIDTH;      
		uchar * TxBufPtr = TxBuf;
		//   count /TX_PLOAD_WIDTH向上取整作为发送次数 保证count 数目数据全部发送
		while(countDevide)
		{
			nrf24l01_txPacket(TxBufPtr);
			SPI_RW_Reg(WRITE_REG+STATUS,0XFF); 
			TxBufPtr += TX_PLOAD_WIDTH;
			countDevide--;
		}
	}
	kfree(TxBuf);

        udelay(200);

	nrf24l01_setRx();                                     //发送完毕 切换设备为接收状态

	return count;
}

/**************************************************
驱动函数：nrf24l01_read()

描述：
    读设备函数
**************************************************/
static ssize_t nrf24l01_read(struct file *filp, const char *buffer,size_t count, loff_t *ppos)
{

#ifdef DEBUG
	printk("================================================");
        printk("Reading ... \n");
        printk("The read count is %d \n",count);
       
#endif
    //    nrf24l01_setRx();

	uchar * RxBuf = 
	(uchar *)kmalloc(((count+RX_PLOAD_WIDTH-1)/RX_PLOAD_WIDTH)*RX_PLOAD_WIDTH * sizeof(uchar),0);                                                                                                            //申请缓冲区作为接收缓冲
	int countDevide = (count+RX_PLOAD_WIDTH-1)/RX_PLOAD_WIDTH;
	int countDevideTmp = countDevide;
#ifdef DEBUG
        printk("countDevide is %d \n",countDevide);
#endif
	int readCount = 0;
	uchar * RxBufPtr = RxBuf;
		//   count /TX_PLOAD_WIDTH向上取整作为发送次数 保证count 数目数据全部发送
	while(countDevide)
	{
		    if(nrf24l01_rxPacket(RxBufPtr))          //若读到数据
		    {  
			    RxBufPtr += RX_PLOAD_WIDTH;                   //向后移位 准备放入新数据
				SPI_RW_Reg(WRITE_REG+STATUS,0XFF);            //清除接受状态
			    readCount ++;                                 //读取计数器自增
			}
			countDevide--;
	}
	if (copy_to_user(buffer,RxBuf,count))                           //向用户空间复制接收内容 如果成功返回0
	{
		printk(DEVICE_NAME "Can't send data to user!\n");              //若失败 则错误退出
		kfree(RxBuf);
		return -EFAULT;
	}      
	kfree(RxBuf);
#ifdef DEBUG
        printk("readCount is %d \n",readCount);
#endif

	if(readCount == countDevideTmp)
	{

		return count;
	}

	return readCount*RX_PLOAD_WIDTH;		

}

/**************************************************
驱动函数：nrf24l01_open()

描述：
    开启设备函数
**************************************************/
static int nrf24l01_open(struct inode *node, struct file *file)
{
	int open_result = 0;
	if(deviceCounter == 1)
	{
			return -EBUSY;                                   //若设备已经打开 则报错退出
	}
	open_result = io_nrf24l01_init();               //初始化设备
	if(open_result == 0)                                  //初始化失败
	{
		printk(DEVICE_NAME "Unable to open!\n");  
		return -1;
	}
	else
	{
		deviceCounter ++;
		printk(DEVICE_NAME "Device opened!\n");
		return 0;
	}
}

/**************************************************
驱动函数：nrf24l01_release()

描述：
    释放设备函数
**************************************************/
static int nrf24l01_release(struct inode *node, struct file *file)
{
  deviceCounter --;
  printk(DEVICE_NAME "Device released !\n");
  return 0;
}

/*           连接各个函数与驱动操作    */

static struct file_operations nrf24l01_fops = {
  .owner = THIS_MODULE,
  .open = nrf24l01_open,
  .write = nrf24l01_write,
  .read = nrf24l01_read,
  .release = nrf24l01_release
};

static struct miscdevice nrf24l01_dev = {
  NRF24L01_MAJOR,
  DEVICE_NAME,
  &nrf24l01_fops
};
 
/**************************************************
驱动函数：nrf24l01_init()

描述：
    设备驱动初始化函数
**************************************************/
static int __init nrf24l01_init(void)
{
	int ret = 0;
	printk(DEVICE_NAME "Initializing ...... \n");
//	ret = register_chrdev(NRF24L01_MAJOR,DEVICE_NAME,&nrf24l01_fops);
       ret = misc_register(&nrf24l01_dev);
/*        devfs_mk_cdev(
                      MKDEV(NRF24L01_NAJOR,NRF24L01_MINOR),
                      S_IFCHR|S_IRUSR|S_IWUSR|S_IRGRP,
                      DEVICE_NAME
                     );
*/
	mdelay(10);
	if(ret < 0)
	{
		printk(DEVICE_NAME "Can't register device \n");
		return ret;
	}
	else
	{
		printk(DEVICE_NAME "Register success \n");
		deviceCounter = 0;
		return 0;
	}
}

/**************************************************
驱动函数：nrf24l01_exit()

描述：
    设备驱动注销函数
**************************************************/
static void __exit nrf24l01_exit(void)
{
//	unregister_chrdev(NRF24L01_MAJOR,DEVICE_NAME);
//        devfs_remove(DEVICE_NAME);
        misc_deregister(&nrf24l01_dev);
	printk(DEVICE_NAME "Unregister success \n");
}

module_init(nrf24l01_init);
module_exit(nrf24l01_exit);
