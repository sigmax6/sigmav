/*
    NRF24L01.H    NRF24L01驱动程序 基于Mini2440平台
    头文件 数据定义
    Sigmax6      2011年3月3日18:51:32
*/

#ifndef NRF24L01_H
#define NRF24L01_H

//定义两个别名 方便使用数据类型
typedef unsigned int uint16;
typedef unsigned char uchar;

//定义设备名称
#define DEVICE_NAME "nrf24l01"

//定义主设备号
#define NRF24L01_MAJOR 241
#define NRF24L01_NINOR 0

/*  平台移植请更改以下区域  */

// mini 2440 上 GPF0-5 对应 EINT0-5 6脚接出 CON6 */
/*
    GPF1/EINT1    --      CSN 
    GPF2/EINT2    --      CE
    GPF3/EINT3    --      MOSI
    GPF4/EINT4    --      SCK
    GPG9/EINT17    --      IRQ
    GPF10/EINT18    --      MISO
*/

//定义NRF24L01连接管脚   和方向宏

#define SCK		S3C2410_GPF4
#define SCK_OUTP	S3C2410_GPF4_OUTP

#define MISO            S3C2410_GPG10
#define MISO_INP     S3C2410_GPG10_INP

#define IRQ                S3C2410_GPG9
#define IRQ_INP     S3C2410_GPG9_INP

#define MOSI			 S3C2410_GPF3
#define MOSI_OUTP S3C2410_GPF3_OUTP

#define CSN               S3C2410_GPF1
#define CSN_OUTP    S3C2410_GPF1_OUTP

#define CE				S3C2410_GPF2
#define CE_OUTP 		S3C2410_GPF2_OUTP

// 定义管脚操作工具宏
#define CE_OUT		s3c2410_gpio_cfgpin(CE,CE_OUTP)
#define CE_UP			s3c2410_gpio_pullup(CE,1)
#define CE_L              s3c2410_gpio_setpin(CE,0)
#define CE_H             s3c2410_gpio_setpin(CE,1)

#define SCK_OUT		s3c2410_gpio_cfgpin(SCK,SCK_OUTP)
#define SCK_UP			s3c2410_gpio_pullup(SCK,1)
#define SCK_L              s3c2410_gpio_setpin(SCK,0)
#define SCK_H             s3c2410_gpio_setpin(SCK,1)

#define MISO_IN   		s3c2410_gpio_cfgpin(MISO,MISO_INP)
#define MISO_UP			s3c2410_gpio_pullup(MISO,1)
#define MISO_STU        s3c2410_gpio_getpin(MISO)

#define IRQ_IN   	   s3c2410_gpio_cfgpin(IRQ,IRQ_INP)
#define IRQ_UP		   s3c2410_gpio_pullup(IRQ,1)
#define IRQ_STU          s3c2410_gpio_getpin(IRQ)
#define IRQ_H            s3c2410_gpio_setpin(IRQ,1)

#define MOSI_OUT		s3c2410_gpio_cfgpin(MOSI,MOSI_OUTP)
#define MOSI_UP			s3c2410_gpio_pullup(MOSI,1)
#define MOSI_L              s3c2410_gpio_setpin(MOSI,0)
#define MOSI_H             s3c2410_gpio_setpin(MOSI,1)

#define CSN_OUT		   s3c2410_gpio_cfgpin(CSN,CSN_OUTP)
#define CSN_UP			s3c2410_gpio_pullup(CSN,1)
#define CSN_L              s3c2410_gpio_setpin(CSN,0)
#define CSN_H             s3c2410_gpio_setpin(CSN,1)

/*  平台移植请更改以上区域  */

//=======================================

/*  NRF24L01 设备配置在以下区域        */

#define TX_ADR_WIDTH  5    //TX 地址长度
#define RX_ADR_WIDTH  5   //RX 地址长度

#define TX_PLOAD_WIDTH  4 // TX 负载
#define RX_PLOAD_WIDTH  4 // RX 负载

#define RX_DR  6
#define TX_DS  5 
#define MAX_RT 4

// SPI(nRF24L01) commands
#define READ_REG    0x00  // Define read command to register
#define WRITE_REG   0x20  // Define write command to register
#define RD_RX_PLOAD 0x61  // Define RX payload register address
#define WR_TX_PLOAD 0xA0  // Define TX payload register address
#define FLUSH_TX    0xE1  // Define flush TX register command
#define FLUSH_RX    0xE2  // Define flush RX register command
#define REUSE_TX_PL 0xE3  // Define reuse TX payload register command
#define NOP         0xFF  // Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses)
#define CONFIG      0x00  // 'Config' register address
#define EN_AA       0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02  // 'Enabled RX addresses' register address
#define SETUP_AW    0x03  // 'Setup address width' register address
#define SETUP_RETR  0x04  // 'Setup Auto. Retrans' register address
#define RF_CH       0x05  // 'RF channel' register address
#define RF_SETUP    0x06  // 'RF setup' register address
#define STATUS      0x07  // 'Status' register address
#define OBSERVE_TX  0x08  // 'Observe TX' register address
#define CD          0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0  0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F  // 'RX address pipe5' register address
#define TX_ADDR     0x10  // 'TX address' register address
#define RX_PW_P0    0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17  // 'FIFO Status Register' register address

//发送、接收静态地址初始化配置
uchar const TX_ADDRESS[TX_ADR_WIDTH]=
{
	0x34,
	0x43,
	0x10,
	0x10,
	0x01
};
uchar const  RX_ADDRESS[RX_ADR_WIDTH]=
{
	0x34,
	0x43,
	0x10,
	0x10,
	0x01
};
/*  NRF24L01 设备配置在以上区域        */

//===========================================

/*   函数声明区域    */

uchar SPI_RW(uchar byte);        							 // 根据SPI协议，写一字节数据到nRF24L01，同时从nRF24L01读出一字节
uchar SPI_RW_Reg(uchar reg, uchar value);    		 // 写数据value到reg寄存器
uchar SPI_Read(uchar reg);        							 // 从reg寄存器读一字节
uchar SPI_Read_Buf(uchar reg, uchar * pBuf, uchar bytes);      //  从reg寄存器读出bytes个字节，通常用来读取接收通道数据或接收/发送地址
uchar SPI_Write_Buf(uchar reg, uchar * pBuf, uchar bytes);     // 把pBuf缓存中的数据写入到nRF24L01，通常用来写入发射通道数据或接收/发送地址
void nrf24l01_setRx();

uchar sta;                                                              //存储节点状态信息
int deviceCounter;                                               //驱动开启计数器

//============================================

#endif
