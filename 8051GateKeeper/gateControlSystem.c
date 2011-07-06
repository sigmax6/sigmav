#include<reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int 
#define NUMOFCARDS 24                  //实验室成员总数量


long data card_num[NUMOFCARDS]=			//实验室成员卡号
{
710474,//邱铁老师   1
881801,//申珅            2
15319555,//周云斌     3
3258102,//丁宁老师   4
5668765,//陆文金       5
878149,//胡波            6
15329672,//庞旭远	   7
14792863,//王革正     8
876217,//郑爽             9
6569951,//熊倩子       10
6569705，//王宇航    11
15318846，//王杰     12
6567820,//王德威       13
15330248,//翁逍克     14
6554111,//于颜硕       15
5659606,//张民垒       16
755715 ,//崔彦芳        17
6567820 ,//王德威      18
670144 ,//邬金钊        19
15330773,//左熠琳     20
737291,//李瑞            21
703752,//王长俊         22
15315666,//刘芳冰     23   
50015335536,//李洋  24
};

uchar idata wiegand[26];	//记录读进来的卡号---->  二进制格式
long card_num_in = 0;        //计算后的读进来的卡号---->  十进制格式

sbit led = P2^6;           //指示灯，1 表示门开
sbit button = P2^1;        //出实验时的开关按钮，按下后为0, 表示门开

sbit DATA0=P3^2;   			//中断it0
sbit DATA1=P3^3;        	//中断it1

uint count_num = 0;       //用于计数wiegand的26位

int judge_card();         //判断输入卡号是否为实验室成员

int delay1m(uint z); 



void main(void)   
{  
      
    led=0;				//初始化，门关
	button = 1;
/******************中断初始化************************/
 EA=0;
 EX0=1;
 EX1=1;

 IT0=1;
 IT1=1;
 EA=1;
/**********************************************/
 while(1)
 {
/***************************读卡进入*******************************/
   if(count_num>25)
   {	
   		EA=0;                       //关闭中断总开关
        EX0=0;						//关闭INT0中断
        EX1=0;						//关闭INT1中断

        judge_card();               //进入判断函数
    	
		IE0 = IE1 = 0;              //清除中断请求
		count_num = 0;              //计数清零

		EX0=1;						//开启中断总开关
        EX1=1;						//开启INT0中断
	    EA=1;						//开启INT1中断
   }
/***************************按钮出去*******************************/ 
 	if(button==0)
 	{
     	 EA=0;
	  	 led=1;
	 	 delay1m(10000);
		 led=0;
    	 IE0 = IE1 = 0; 
		 count_num=0; 
		 EA=1;
		 button = 1;
 	}
 }
}
/****************************卡号判别函数***********************************************/
int judge_card()
{ 
    // num=0; 

   	uint m;
	uint n;

	 card_num_in=wiegand[1];
/******************************计算二进制韦根码为十进制格式****************************/
	 for(m=2;m<25;m++)
	 { 
	   card_num_in = card_num_in*2 + wiegand[m]; 
	 }
/************************判断十进制卡号是否为card_num数组中的卡号*********************************************/
	 for(n=0;n<NUMOFCARDS;n++)
	 {
		 if(card_num_in == card_num[n])
		 {  
		    led=1;                            //开门
			delay1m(10000);
			led=0;                            //关门
			return 0;
		 }
	 }
	 return 0;
}



void data0() interrupt 0
{
 EX0=0;
 wiegand[count_num]=0;
 count_num++;
 EX0=1;
}  

void data1() interrupt 2
{
 EX1=0;
 wiegand[count_num]=1;
 count_num++;
 EX1=1;
} 

int delay1m(uint z) 
{
  uchar j;
   while(z--)
    for(j = 0; j < 120; j++);
	return 0;
}
 
