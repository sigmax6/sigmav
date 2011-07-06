#include<reg52.h>
#include <intrins.h>
#include <api.h>
#define uchar unsigned char
#define uint unsigned int 
#define NUMOFCARDS 10                  //实验室成员总数量


/***************************************************/
#define TX_ADR_WIDTH   5  // 5字节宽度的发送/接收地址
#define TX_PLOAD_WIDTH 10 // 数据通道有效数据宽度
#define RX_PLOAD_WIDTH 1

uchar code TX_ADDRESS[TX_ADR_WIDTH] = {0x05,0x04,0x03,0x02,0x01};  // 定义一个静态发送地址

uchar idata TX_BUF[TX_PLOAD_WIDTH];	//记录读进来的卡号---->  二进制格式
uchar  RX_BUF[RX_PLOAD_WIDTH];
uchar flag1,flag2,flag3;
uchar bdata sta;
sbit  RX_DR	 = sta^6;
sbit  TX_DS	 = sta^5;
sbit  MAX_RT = sta^4;

sbit led = P0^7;           //指示灯，1 表示门开

sbit button = P2^0;        //出实验时的开关按钮，按下后为0, 表示门开
sbit DATA0=P3^2;   			//中断it0
sbit DATA1=P3^3;        	//中断it1
uchar count_num = 0;       //用于计数wiegand的26位

/**************************************************/


long data card_num[NUMOFCARDS]=			//实验室成员卡号
{
8761673,//邱铁老师   6
881801,//申珅         7
9089737,//韩旭       8
15334330,//黄贺      9
863152,//孙铜轩      10
15179581,//杜春明    11
5635756,//王伟       12
15319555,//周云斌    13
868189,//岳北        14
9126742,//李兴旺     15


};


long card_num_in = 0;        //计算后的读进来的卡号---->  十进制格式



int judge_card();         //判断输入卡号是否为实验室成员
void delay_ms(uint x);
void init_51();
void check_count_num();
void check_button();
uchar Check_ACK(bit clear);
void init_io(void);
void RX_Mode(void);
void TX_Mode(uchar * BUF);
uchar SPI_RW(uchar byte);
uchar SPI_RW_Reg(uchar reg, uchar value);
uchar SPI_Read(uchar reg);
uchar SPI_Read_Buf(uchar reg, uchar * pBuf, uchar bytes);
uchar SPI_Write_Buf(uchar reg, uchar * pBuf, uchar bytes);


void main(void)   
{  
      
    init_51();
	init_io();
	RX_Mode();

 while(1)
 {
	flag1=flag2=flag3=0;
	check_count_num();
	check_button();
	if(count_num>9)
	{
		EX0=0;
		EX1=0;
		led=0;
		count_num=0;
		delay_ms(500);
		led=1;
		TX_Mode(TX_BUF);
		Check_ACK(1);	
		RX_Mode();
		EX0=1;
		EX1=1;
		
	}
	delay_ms(50);
	sta = SPI_Read(STATUS);	  // 读状态寄存器
	if(RX_DR)				  // 判断是否接受到数据
	{

		SPI_Read_Buf(RD_RX_PLOAD, RX_BUF, RX_PLOAD_WIDTH);  // 从RX FIFO读出数据
		flag3 = 1;
	}
	SPI_RW_Reg(WRITE_REG + STATUS, sta);  // 清除RX_DS中断标志
	if(flag3)
	{

		flag3=0;
		if(RX_BUF[0]==0x55)
		{
			EA=0;
			led=0;                            //开门
			delay_ms(1000);
			led=1;                            //关门
			IE0=IE1=0;
			count_num=0;
			EA=1;
		}
	}


 }
}
/**************************************************/
uchar Check_ACK(bit clear)
{
	while(IRQ);	
	sta = SPI_RW(NOP);                    // 返回状态寄存器
	if(MAX_RT)
		if(clear)                         // 是否清除TX FIFO，没有清除在复位MAX_RT中断标志后重发
			SPI_RW(FLUSH_TX);

	SPI_RW_Reg(WRITE_REG + STATUS, sta);  // 清除TX_DS或MAX_RT中断标志
	IRQ = 1;
	if(TX_DS)
		return(0x00);
	else
		return(0xff);
}
/**************************************************/

void init_51()
{
	led=1;				//初始化，门关
	button = 1;
/******************中断初始化************************/
	EA=0;
	EX0=1;
	EX1=1;

	IT0=1;
	IT1=1;
	EA=1;
/**********************************************/
}

void check_count_num()
{
	/***************************读卡进入*******************************/
   if(count_num>9)
   {	
   		EA=0;                       //关闭中断总开关
        EX0=0;						//关闭INT0中断
        EX1=0;						//关闭INT1中断
		flag2=1;
		flag1=0;
        judge_card();               //进入判断函数
    	
		IE0 = IE1 = 0;              //清除中断请求

		EX0=1;						//开启中断总开关
        EX1=1;						//开启INT0中断
	    EA=1;						//开启INT1中断
   }
}

void check_button()
{
	/***************************按钮出去*******************************/ 
 	if(button==0)
 	{
     	 EA=0;
	  	 led=1;
	 	 delay_ms(1000);
		 led=0;
    	 IE0 = IE1 = 0; 
		 count_num=0; 
		 EA=1;
		 button = 1;
		 flag1=1;
 	}
}

/****************************卡号判别函数***********************************************/
int judge_card()
{ 
   
   	uint m;
	uint n;

	card_num_in=TX_BUF[1];
/******************************计算二进制韦根码为十进制格式****************************/
	 for(m=2;m<9;m++)
	 { 
	   card_num_in = card_num_in*2 + TX_BUF[m]; 
	 }
/************************判断十进制卡号是否为card_num数组中的卡号*********************************************/
	 for(n=0;n<NUMOFCARDS;n++)
	 {
		 if(card_num_in == card_num[n])
		 {  
		    led=1;                            //开门
			delay_ms(1000);
			led=0;                            //关门
			flag1=1;                           //标记门已经开
			return 0;
		 }
	 }

	 return 0;
}


void data0() interrupt 0
{
 EX0=0;
 TX_BUF[count_num]=0x55;
 count_num++;
 //delay_ms(150);
 EX0=1;
}  

void data1() interrupt 2
{
 EX1=0;
 TX_BUF[count_num]=0xaa;
 count_num++;
 //delay_ms(150);
 EX1=1;
} 


/**************************************************
函数：delay_ms()

描述：
    延迟x毫秒
/**************************************************/
void delay_ms(uint x)
{
    uint i, j;
    i = 0;
    for(i=0; i<x; i++)
    {
       j = 250;
       while(--j);
	   j = 250;
       while(--j);
    }
}
/**************************************************/

/**************************************************
函数: init_io()

描述:
    初始化IO
/**************************************************/
void init_io(void)
{
	CE  = 0;        // 待机
	CSN = 1;        // SPI禁止
	SCK = 0;        // SPI时钟置低
	IRQ = 1;        // 中断复位
}
/**************************************************/


/**************************************************
函数：SPI_RW()

描述：
    根据SPI协议，写一字节数据到nRF24L01，同时从nRF24L01
	读出一字节
/**************************************************/
uchar SPI_RW(uchar byte)
{
	uchar i;
   	for(i=0; i<8; i++)          // 循环8次
   	{
   		MOSI = (byte & 0x80);   // byte最高位输出到MOSI
   		byte <<= 1;             // 低一位移位到最高位
   		SCK = 1;                // 拉高SCK，nRF24L01从MOSI读入1位数据，同时从MISO输出1位数据
   		byte |= MISO;       	// 读MISO到byte最低位
   		SCK = 0;            	// SCK置低
   	}
    return(byte);           	// 返回读出的一字节
}
/**************************************************/

/**************************************************
函数：SPI_RW_Reg()

描述：
    写数据value到reg寄存器
/**************************************************/
uchar SPI_RW_Reg(uchar reg, uchar value)
{
	uchar status;
  	CSN = 0;                   // CSN置低，开始传输数据
  	status = SPI_RW(reg);      // 选择寄存器，同时返回状态字
  	SPI_RW(value);             // 然后写数据到该寄存器
  	CSN = 1;                   // CSN拉高，结束数据传输
  	return(status);            // 返回状态寄存器
}
/**************************************************/

/**************************************************
函数：SPI_Read()

描述：
    从reg寄存器读一字节
/**************************************************/
uchar SPI_Read(uchar reg)
{
	uchar reg_val;
  	CSN = 0;                    // CSN置低，开始传输数据
  	SPI_RW(reg);                // 选择寄存器
  	reg_val = SPI_RW(0);        // 然后从该寄存器读数据
  	CSN = 1;                    // CSN拉高，结束数据传输
  	return(reg_val);            // 返回寄存器数据
}
/**************************************************/

/**************************************************
函数：SPI_Read_Buf()

描述：
    从reg寄存器读出bytes个字节，通常用来读取接收通道
	数据或接收/发送地址
/**************************************************/
uchar SPI_Read_Buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
  	CSN = 0;                    // CSN置低，开始传输数据
  	status = SPI_RW(reg);       // 选择寄存器，同时返回状态字
  	for(i=0; i<bytes; i++)
    	pBuf[i] = SPI_RW(0);    // 逐个字节从nRF24L01读出
  	CSN = 1;                    // CSN拉高，结束数据传输
  	return(status);             // 返回状态寄存器
}
/**************************************************/

/**************************************************
函数：SPI_Write_Buf()

描述：
    把pBuf缓存中的数据写入到nRF24L01，通常用来写入发
	射通道数据或接收/发送地址
/**************************************************/
uchar SPI_Write_Buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
  	CSN = 0;                    // CSN置低，开始传输数据
  	status = SPI_RW(reg);       // 选择寄存器，同时返回状态字
  	for(i=0; i<bytes; i++)
    	SPI_RW(pBuf[i]);        // 逐个字节写入nRF24L01
  	CSN = 1;                    // CSN拉高，结束数据传输
  	return(status);             // 返回状态寄存器
}
/**************************************************/

/**************************************************
函数：RX_Mode()

描述：
    这个函数设置nRF24L01为接收模式，等待接收发送设备的数据包
/**************************************************/
void RX_Mode(void)
{
	CE = 0;
  	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 接收设备接收通道0使用和发送设备相同的发送地址
  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);               // 使能接收通道0自动应答
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);           // 使能接收通道0
  	SPI_RW_Reg(WRITE_REG + RF_CH, 40);                 // 选择射频通道0x40
  	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);  // 接收通道0选择和发送通道相同有效数据宽度
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);            // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
  	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);              // CRC使能，16位CRC校验，上电，接收模式
  	CE = 1;                                            // 拉高CE启动接收设备
}
/**************************************************/

/**************************************************
函数：TX_Mode()

描述：
    这个函数设置nRF24L01为发送模式，（CE=1持续至少10us），
	130us后启动发射，数据发送结束后，发送模块自动转入接收
	模式等待应答信号。
/**************************************************/
void TX_Mode(uchar * BUF)
{
	CE = 0;
  	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
  	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
  	SPI_Write_Buf(WR_TX_PLOAD, BUF, TX_PLOAD_WIDTH);                  // 写数据包到TX FIFO
  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);       // 使能接收通道0自动应答
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);   // 使能接收通道0
  	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x0a);  // 自动重发延时等待250us+86us，自动重发10次
  	SPI_RW_Reg(WRITE_REG + RF_CH, 40);         // 选择射频通道0x40
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
  	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);      // CRC使能，16位CRC校验，上电
	CE = 1;
}
/**************************************************/
 
