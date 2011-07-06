#include    <no_float.h>         
#include    <stdio.h>
#include    <machine.h>
#include    "h8_3048.h"

#define         TIMER_CYCLE     3071    /* Timer cycle 1ms              */
                                        /* When it is to be used by f/8 */
                                        /* f / 8 = 325.5[ns]            */
                                        /* Therefore, TIMER_CYCLE       */
                                        /*          = 1[ms] / 325.5[ns] */
                                        /*          = 3072              */
#define         PWM_CYCLE       49151   /* PWM cycle 16ms               */
                                        /* Therefore, PWM_CYCLE         */
                                        /*         = 16[ms] / 325.5[ns] */
                                        /*         = 49152              */
#define         SERVO_CENTER   4352 	//4362//4460   /* Center value of Servo        */
#define         HANDLE_STEP     26      /* 1 degree part value          */

/* Mask value setting x: With Mask (Invalid) o:Without mask (Valid)     */
#define         MASK2_2         0x66    /* xooxxoox                     */
#define         MASK2_0         0x60    /* xooxxxxx                     */
#define         MASK0_2         0x06    /* xxxxxoox                     */
#define         MASK3_3         0xe7    /* oooxxooo                     */
#define         MASK0_3         0x07    /* xxxxxooo                     */
#define         MASK3_0         0xe0    /* oooxxxxx                     */
#define         MASK4_0         0xf0    /* ooooxxxx                     */
#define         MASK0_4         0x0f    /* xxxxoooo                     */
#define         MASK1_1         0x81    /* oxxxxxxo                     */
#define			MASK4_4			0xff
//光电编码部分
//***********************************************************************************
/*======================================*/
/* encoder declaration                */
/*======================================*/
//程序基本思路：在一定时间内记录脉冲数目，通过固定周长的脉冲数，通过公式实现速度的获取

/************************************************************************/
//光电编码初始化变量定义
//程序基本思路：在一定时间内记录脉冲数目，通过固定周长的脉冲数，通过公式实现速度的获取


#define ePulesNum	(200)		//旋转编码器的脉冲数
#define eSpeedNum   (20)		//定时时间的次数，每一次为1MS
#define eWheelGirth (125)		//车轮周长，单位为米(125mm)
#define eSpeedTime	(0.02)		//定时周期，0.1s

unsigned long iPulesNumAll;		//需要观察数目越界的问题

double  eCurSpeed = 0.0;		//实时计算速度变量
unsigned long  timerCnt=0;
double   abvSpeed	=0.0;


int speedBrake_var=0;
int crankFlag=0;
//double Ki_Speed = 0.5;
//double Kp_Speed = 1;
//double lastError = 0.0;

int speedControl(int expSpeed);	//速度处理函数：返回值为调整的速度



/*======================================*/
/* Prototype declaration                */
/*======================================*/
void init(void );
void eInit(void);	//光电编码初始化函数

void timer( unsigned long timer_set );
int check_crossline( void );
unsigned char sensor_inp( unsigned char mask );
unsigned char dipsw_get( void );
unsigned char pushsw_get( void );
unsigned char startbar_get( void );
void led_out( unsigned char led );
void speed( int accele_l, int accele_r );
void handle( int angle );
char unsigned bit_change( char unsigned in );


/***********************************************************************************************/

#define			st0				0
#define			st1				1
#define			st2				2
#define			st3				3
#define			st4				4
#define			st5				5
#define			st_1			-1
#define			st_2			-2
#define			st_3			-3
#define			st_4			-4
#define			st_5			-5

//st0
#define			speedLevel_st0	100
#define			midDegree_st0	0
//st1
#define			speedLevel_st1	95
#define			midDegree_st1	3
//st2
#define			speedLevel_st2	90
#define			midDegree_st2	10
//st3
#define			speedLevel_st3	85
#define			midDegree_st3	15
//st4
#define			speedLevel_st4	80
#define			midDegree_st4	27
//st5
#define			speedLevel_st5	70
#define			midDegree_st5	35

//转弯标准量定义
#define std0	0
#define std1	4
#define std2 	10
#define std3 	15
#define std4 	27
#define std_1	-5
#define std_2 	-10
#define std_3 	-15
#define std_4 	-27

	
const double length = 17;
const double width = 15.5;
const double tan[46] ={ 0, 0.017455,0.034921,0.052408,0.069927,0.087489,
						   0.105104,0.122785,0.140541,0.158384,0.176327,
						   0.194380,0.212557,0.230868,0.249328,0.267949,
						   0.286745,0.305731,0.324920,0.344328,0.363970,
						   0.383864,0.404026,0.424475,0.445229,0.466308,
						   0.487733,0.509525,0.531709,0.554309,0.577350,
						   0.600861,0.624869,0.649408,0.674509,0.700208,
						   0.726543,0.753554,0.781286,0.809784,0.839100,
						   0.869287,0.900404,0.932515,0.965689,1.000000};

unsigned long   cnt0;
unsigned long   cnt1;                  
unsigned long	cnt2;


//方向变量
int direction;
int turn_direction;
int blackArea_direction;

//控制变量
int pattern = 0;	//状态
int curSt;
int lastSt;

int degree;		//角度
int lastDegree;

int speedLevel = 0;	//速度
int lastSpeed_l;
int lastSpeed_r;

int var;		//辅助

//my function 
void myInit(void);
void start();
void straight_run();
void right_turn();
void left_turn();
void rightAngle();
void rightAngle2();
int check_blackArea(void);
void right_blackArea();
void left_blackArea();
int get_inner_speed(int degree,int speed_outer);
int get_speedLevel(int curSt);
int get_midDegree(int curSt);
void do_speed(int degree,int speed_outer);
int get_near_std(int cur_speed);
void print();
void getData(unsigned char var_,int debug);
char * getInfo(int id,int dir);
void stop();
short int getDegree_(int degree);
void getLightLed(unsigned char led);
int getSpeed_(int speed);
int getLightLedSum();


#define arrayLength 200

unsigned char pattern_[arrayLength] = { 0 };
unsigned char ledValue_[arrayLength]= {0};
short int duration_[arrayLength] = { 0 }; // 负数时表示调试信息
unsigned char degree_[arrayLength] = {0};	//+0~49  -:50~
unsigned char leftSpeed_[arrayLength] = { 0 };
unsigned char rightSpeed_[arrayLength]={ 0 };
int instantSpeed[arrayLength]={0};

unsigned char curLedValue;
unsigned char lastLedValue;

int isDebug = 0;
int curSuffix = 1;
int lastSuffix = 0;
int printNow = 0;

int pushSw = 0;	//调试用:取数据中断，打印数据开关
int isStop = 0;
int isStart = 0;

int turnSpeed ;
int lastDegree;

void main( void )
{
    int  i;	
	init();                             /* Initialization function     */
    eInit();
	init_sci1( 0x00, 79 );              /* Initialization of SCI1      */
    set_ccr( 0x00 );                    /* Entire interrupt permission */
		
	myInit();  //初始化函数  3个direction = 0

	while(!pushsw_get())
	{
		if(cnt1<100)
			led_out(0x1);
		else if(cnt1<200)
			led_out(0x2);
		else	cnt1 = 0;
	}
	cnt1 = 0;
	while(startbar_get())
	{
		if(cnt1<50)
			led_out(0x1);
		else if(cnt1<100)
			led_out(0x2);
		else	cnt1 = 0;
	}
	/*
	while(cnt1<100)
	{
		if(cnt1<50)
			led_out(0x1);
		else
			led_out(0x2);
	}*/
	
	led_out(0);
	start();
	pattern = 10;	
	while(1)
	{

		switch(pattern)
		{

			case 10:	//直线
					if(check_crossline()){	pattern = 30;break;	}
					if(check_blackArea()){	pattern = 40;break;	}
					
					straight_run();				//直线前行，弯道检测，盲区检测
					break;
			
			case 20:	//弯道
						switch(turn_direction)
						{
							case -1:
									handle(-1*midDegree_st4);
									left_turn();
									break;
							case 1:
									handle(midDegree_st4);
									right_turn();
									break;
							default:break;
						}
						break;


			case 30:	//直角
						rightAngle2();						
						break;


			case 40:	//盲区
						switch(blackArea_direction)
						{
							case -1:
									left_blackArea();
									break;
							case 1:
									right_blackArea();
									break;
							default:break;									
						}
						break;

			default:break;

		}//end switch

	}//end while
	
	
}//end main

/************************************************************************/
/* H8/3048F-ONE Built in Peripheral Function Initialization             */
/************************************************************************/
void init( void )
{
    /* I/O port Setting */
    P1DDR = 0xff;
    P2DDR = 0xff;
    P3DDR = 0xff;
    P4DDR = 0xff;
    P5DDR = 0xff;
    P6DDR = 0xf0;                       /* DIP SW on CPU board          */
    P8DDR = 0xff;
    P9DDR = 0xf7;                       /* Communication Port           */
    PADDR = 0xff;
	
    PBDR  = 0xc0;
    PBDDR = 0xfe;                       /* Motor Drive Board Vol.3      */
    /* As P7 of the sensor board is an exclusive input, there are no input output settings. */

    /* ITU0 Interrupt at every 1ms */
    ITU0_TCR = 0x23;
    ITU0_GRA = TIMER_CYCLE;
    ITU0_IER = 0x01;

    /* ITU3, 4 reset-synchronized PWM mode for right-left motor and servo */
    ITU3_TCR = 0x23;
    ITU_FCR  = 0x3e;
    ITU3_GRA = PWM_CYCLE;                   /* Setting of cycle         */
    ITU3_GRB = ITU3_BRB = 0;                /* PWM Setting of left  motor*/
    ITU4_GRA = ITU4_BRA = 0;                /* PWM Setting of right motor*/
    ITU4_GRB = ITU4_BRB = SERVO_CENTER;     /* PWM Setting of servo     */
    ITU_TOER = 0x38;

    /* Count start of ITU */
    ITU_STR = 0x09;//0x09;
}
//定时器量变化:每1ms发生一次中断进入
//光电编码器初始化

void eInit(void)
{
	//寄存器端口方向初始化
	//P8DDR:bit0-->0(建议方法),IER(中断使能寄存器):bit0-->1(IRQ0,input)
	P8DDR = 0xff;
	P8DDR &= 0xfe;
	
	IER = 0x00;
	IER |= 0x01;	//IRQ0 ,enable

	//中断初始化：下降沿捕获
	ISCR = 0x01;	//高电平向低电平跳变是触发
	ISR =	0xfe;		//清楚标志寄存器	
	//开启中断

	
	//计算速度时的系数
	abvSpeed =(double)((eWheelGirth)/(ePulesNum*eSpeedTime));

}
//我的初始化变量
void myInit(void)
{
	if((~P6DR)&0x08) isDebug = 1;	
	else isDebug = 0;

	lastSpeed_l = 0;
	lastSpeed_r = 0;
	lastDegree = 0;
	
	pushSw = 0;
	turnSpeed = 95;
//	turnSpeed = 100 - 2*(dipsw_get()&0x07);
}
//角度函数
void handle( int angle )
{
	int i;
	degree = angle;
	
	if(printNow) print(); //调试函数
	if(isStop) stop();
	
	if(angle>44)
		angle = 44;
	else if(angle<-44)
		angle = -44;
		
	if(angle==lastDegree) return;
	else
	{
		if(angle-lastDegree>4)
			for(i=lastDegree;i<angle;i+=3)
				ITU4_BRB = SERVO_CENTER - i * HANDLE_STEP;
		else if(angle-lastDegree<-4)
			for(i=lastDegree;i>angle;i-=3)
				ITU4_BRB = SERVO_CENTER - i * HANDLE_STEP;
		ITU4_BRB = SERVO_CENTER - angle * HANDLE_STEP;
		lastDegree = angle;
	}
		
}
//速度函数
void speed( int accele_l, int accele_r )
{
	unsigned char sw_data;
	unsigned long speed_max;
    sw_data= dipsw_get()+5;
	speed_max=(unsigned long)(PWM_CYCLE-1) *sw_data/20;
    if(accele_l>100)
		accele_l = 100;
	else if(accele_l<-100)
		accele_l = -100;
	if(accele_r>100)
		accele_r = 100;
	else if(accele_r<-100)
		accele_r = -100;
	
    /* Left motor */
    if( accele_l >= 0 ) {
        PBDR &= 0xfb;
        ITU3_BRB = (unsigned long)speed_max* accele_l / 100;
    } else {
        PBDR |= 0x04;
        accele_l = -accele_l;
        ITU3_BRB = (unsigned long)speed_max * accele_l / 100;
    }

    /* Right motor */
    if( accele_r >= 0 ) {
        PBDR &= 0xf7;
        ITU4_BRA = (unsigned long)speed_max* accele_r / 100;
    } else {
        PBDR |= 0x08;
        accele_r = -accele_r;
        ITU4_BRA = (unsigned long)speed_max * accele_r / 100;
    }	

}
/************************************************************************/
/* external Interrupt process                                               */
/************************************************************************/

#pragma interrupt( interrupt_capture )
void interrupt_capture( void )////////////////////////////////////////////
{
	
	ISR &=	0xfe;		//清楚标志寄存器				
	iPulesNumAll++;		//脉冲计数:
	//此处需要考虑数目越界问题
}


/************************************************************************/
/* ITU0 Interrupt process                                               */
/************************************************************************/

#pragma interrupt( interrupt_timer0 )
void interrupt_timer0( void )
{
    ITU0_TSR &= 0xfe;                   /* Flag clear                   */
    cnt0++;
    cnt1++;
	cnt2++;
/////////////////////////////////////////////////////////////////
 timerCnt++;	
	if((eSpeedNum) == timerCnt)	//100,时计数
	{
		//此时进行一次速度的计算
		eCurSpeed=(double)((iPulesNumAll*abvSpeed)/10);	//速度计算公式:
															//				      脉冲数     ×  周长
															//		实时速度=---------------------------
															//					一圈的脉冲数 × 采集时间
		//复位
		iPulesNumAll = 0;	//复位脉冲数目
		timerCnt = 0;		//复位时间数目
		
	}//if
///////////////////encode for straight line/////////////////////////////
	if((int)eCurSpeed>295) 
	{
		
		//led_out(0x03);
	
	
		ITU4_BRA=(unsigned long)(PWM_CYCLE-1)*5/100;
		ITU3_BRB=(unsigned long)(PWM_CYCLE-1)*5/100;
	
		for(speedBrake_var=500;speedBrake_var!=0;speedBrake_var--);
	}
////////////////////encoder for crank////////////////////////////////////
	
if(printNow!=1){
	if(crankFlag)
	{
		if((int)eCurSpeed>160){
		ITU4_BRA=(unsigned long)(PWM_CYCLE-1)*10/100;
		ITU3_BRB=(unsigned long)(PWM_CYCLE-1)*10/100;
		for(speedBrake_var=2000;speedBrake_var!=0;speedBrake_var--);
		}
		/*
		if((int)eCurSpeed<120){
		ITU4_BRA=(unsigned long)(PWM_CYCLE-1)*90/100;
		ITU3_BRB=(unsigned long)(PWM_CYCLE-1)*90/100;
		for(speedBrake_var=600;speedBrake_var!=0;speedBrake_var--);
		}*/
	}
}
////////////////////////////////////////////////////////////////////	
	if(isStart)
	{
		if(isDebug)
		{
			if(!printNow)
			{
				if((~PBDR&0x01) && pushSw){ isStart = 0;isDebug = 0;printNow = 1;}//行进中途读调试数据
				curLedValue = P7DR;
				if(curLedValue!=lastLedValue)
					getData(curLedValue,0);
				else if(bit_change(P7DR)==0xff && cnt0>800)	//调试出轨  before p7dr=0xff
				{
					printNow = 1;
					ITU4_BRA=0;
					ITU3_BRB=0;	
				}
			}
		}
		else //非调试出轨
		{
			if(lastLedValue!=P7DR)
			{
				lastLedValue = P7DR;
				cnt0 = 0;	
			}else if(bit_change(P7DR) ==0xff && cnt0>800)	isStop = 1;// before p7dr=0xff
		}
	}
}
void getData(unsigned char ledValue,int debug) //debug取值=>  0:不调试	<0:左调试	>0：右调试 =>debug的决定值是信息码
{											   //具体在数组中的值按duration[]数组值区分:	>=0正常状态持续时间   -2：左调试  -1:右调试 (由printDebug()函数解析)
	short int temp;
	unsigned char var;
	if(isDebug)
	{
		if(curSuffix<arrayLength)
		{	
			pattern_[curSuffix] = pattern;  	//当前模式	
			ledValue_[curSuffix] = ledValue;	//当前led数据
			if(lastSuffix+1 ==curSuffix)//不等说明上一次加入了调试信息，不用修改lastSuffix下标对于的值，该下标在上一次调试时已经统计完毕
				duration_[lastSuffix] = cnt0;		//上一状态持续时间
			
			temp = (short int)(SERVO_CENTER-ITU4_BRB)/HANDLE_STEP;
			if(temp<0) temp = -1*temp+50;
			degree_[curSuffix] =  temp; 			//当前led状态相应的角度调整
			
			var = (unsigned char)((1.0*ITU3_BRB/(PWM_CYCLE-1))*100);
			if(PBDR&0x04)
				leftSpeed_[curSuffix] = var+150;		//当前led状态相应的左轮速度调整
			else leftSpeed_[curSuffix] = var;
			var = (unsigned char)((1.0*ITU4_BRA/(PWM_CYCLE-1))*100);
			if(PBDR&0x08)
				rightSpeed_[curSuffix] = var+150;		//当前led状态相应的右轮速度调整
			else rightSpeed_[curSuffix] = var;
	///////////////光电编码的调试信息//////////////////////////
			instantSpeed[curSuffix]=(int)eCurSpeed;
	//////////////////////////////////////////////////////////		
			
			if(debug)
			{
				if(debug>0)
					duration_[curSuffix] = -1*debug;		//标志该数据是左调试数据,如-43
				else duration_[curSuffix] = -100+debug;		//标志该数据是右调试数据,如-143(与上对应)
				lastLedValue = 0xaa;		
				curSuffix++;
			}
			else
			{
				lastLedValue = ledValue;
				lastSuffix = curSuffix;
				curSuffix++;			
			}

			cnt0 = 0;
		}else printNow = 0 ;  //before printNow = 1;
	}
	else return;		
}

void print()//该函数中不能调用handle(),因为只有handle()可能调用此函数
{

	int i ;
	int sum;
	isStart = 0;
	isDebug = 0;
	printNow = 0;
	isStop = 0;
	
	cnt1 = 0;
	while(cnt1<=650)
	{
		//580ms
		if(cnt1<=100)
			speedLevel = (int)3*cnt1/10;
		else if(cnt1<=200)
			speedLevel = (int)(cnt1-100)/4+30;
		else if(cnt1<=325)
			speedLevel = (int)4*(cnt1-200)/25+55;
		else if(cnt1<=475)
			speedLevel = (int)(cnt1-325)/10+75;
		else speedLevel  = (int)2*(cnt1-475)/35+90;
		
		speedLevel = 100-speedLevel;
		switch(sensor_inp(MASK3_3))
		{
			case 0x00:
					handle(0);
					speed(speedLevel,speedLevel);
					break;
			case 0x04:
					handle(5);
					do_speed(5,speedLevel);
					break;
			case 0x06:
					handle(10);
					do_speed(-10,speedLevel);
					break;
			case 0x07:
					handle(15);
					do_speed(15,speedLevel);
			case 0x03:
					handle(20);
					do_speed(20,speedLevel);
					break;
					
			//left
			case 0x20:
					handle(-5);
					do_speed(-5,speedLevel);
					break;
			case 0x60:
					handle(-10);
					do_speed(-10,speedLevel);
					break;
			case 0xe0:
					handle(-15);
					do_speed(-15,speedLevel);
					break;
			case 0xc0:
					handle(-20);
					do_speed(-20,speedLevel);
					break;				
		}
	}
	
	speed(0,0);
	
	while(!pushsw_get()){ if(cnt1<300) led_out(0x01); else if(cnt1<600) led_out(0x02); else cnt1 = 0; }


	sum = 0;
	for(i=0;i<curSuffix;i++)
	{
		
		if(duration_[i]<0)
		{			
			printf("sum time:%d\n",sum);
			if(duration_[i]<-100)
				printf(getInfo(-1*(duration_[i]+100),-1));
			else printf(getInfo(-1*duration_[i],1));
			
			printf("\n%3d>:  %2d (%02X,%02X)[",i,pattern_[i],bit_change(0xe7&ledValue_[i]),bit_change(ledValue_[i]));		
			getLightLed(bit_change(ledValue_[i]));
			printf("] %4d     ||     %3d (%3d,%3d)      %3d          \n",duration_[i],getDegree_(degree_[i]),getSpeed_(leftSpeed_[i]),getSpeed_(rightSpeed_[i]),instantSpeed[i]);
			
			sum = 0;
		}else
		{
			sum +=duration_[i];
			
			printf("%3d>:  %2d (%02X,%02X)[",i,pattern_[i],bit_change(0xe7&ledValue_[i]),bit_change(ledValue_[i]));		
			getLightLed(bit_change(ledValue_[i]));
			printf("] %4d     ||     %3d (%3d,%3d)      %3d          \n",duration_[i],getDegree_(degree_[i]),getSpeed_(leftSpeed_[i]),getSpeed_(rightSpeed_[i]),instantSpeed[i]);			
		}
	}
	printf("sum time:%d\n\n",sum);

	while(!pushsw_get()){ if(cnt1<500) led_out(0x01); else if(cnt1<1000) led_out(0x02); else cnt1 = 0; }
	printf("ht,good!^_^\n");
	stop();
}

/************************************************************************/
/* Timer main unit                                                      */
/* Argument Timer value 1=1ms                                           */
/************************************************************************/
void timer( unsigned long timer_set )
{
    cnt0 = 0;
    while( cnt0 < timer_set );
}

unsigned char sensor_inp( unsigned char mask )
{
    unsigned char sensor;
/*
    sensor  = P7DR;

    sensor  = bit_change( sensor );     // Bit replacement 
    sensor &= mask;
*/
	sensor = ~P7DR;
	sensor &= 0xef;
	if( sensor & 0x08 ) sensor |= 0x10;
	
	sensor &=mask;
	
    return sensor;
}
int check_crossline( void )
{
    unsigned char b;
    int ret;

    ret = 0;
 /*   b = sensor_inp(MASK2_2);
    if( b==0x66 || b==0x64 || b==0x26 || b==0x62 || b==0x46 ) {
        ret = 1;
    }*/
	b = bit_change(P7DR);
	if((b&0xc0) && (b&0x3c) && (b&0x03))
		ret = 1;
    return ret;
}

int check_blackArea(void)
{
	unsigned char b = bit_change(P7DR);
	if((b&0x07)==0x07 && (b&0x38) && !(b&0xc0))
	{
		blackArea_direction = 1;
		return 1;
	}
	else if((b&0xe0)==0xe0 && (b&0x1c) && !(b&0x03))
	{
		blackArea_direction = -1;
		return -1;
	}
	else return 0;	
}

unsigned char dipsw_get( void )
{
    unsigned char sw;

    sw  = ~P6DR;                        /* Reading of DIP switch */
    sw &= 0x0f;

    return  sw;
}

unsigned char pushsw_get( void )
{
    unsigned char sw;

    sw  = ~PBDR;                        /* Reading of port having a push switch */
    sw &= 0x01;

    return  sw;
}
unsigned char startbar_get( void )
{
    unsigned char b;

    b  = ~P7DR;                        /* Read start bar signal  */
    b &= 0x10;
    b >>= 4;

    return  b;
}
void led_out( unsigned char led )
{
    unsigned char data;

    led = ~led;
    led <<= 6;
    data = PBDR & 0x3f;
    PBDR = data | led;
}

char unsigned bit_change( char unsigned in )
{
    /*
	unsigned char ret;
    int i;
	
   for( i=0; i<8; i++ ) {
        ret >>= 1;                      // Right shift of return value 
        ret |= in & 0x80;               // Ret bit7 = in bit7           
        in  <<= 1;                      // Left shift of argument       
    }*/
	unsigned char sensor;
	sensor = ~in;
	sensor &=0xef;
	if( sensor & 0x08) sensor |= 0x10;
    return sensor; 
}

void straight_run()
{
	//直线代码
	switch( sensor_inp(MASK3_3) )
	{
		case 0x00:
					if(curSt!=st0)
					{
						lastSt = curSt;
						curSt = st0;	
					}						
					break;

		//right
		case 0x04: /* st1 */ 
					if(curSt!=st1)
					{
						lastSt = curSt;
						curSt = st1;	
					}					
					break;					
					
		case 0x06: /* st2 */
					if(curSt!=st2)
					{
						lastSt = curSt;
						curSt = st2;	
					}					
					break;
		case 0x02:					
		case 0x07:	
					if(check_crossline()){	pattern = 30;return;	}
					if(check_blackArea()){	pattern = 40;return;	}
									
					if(curSt!=st3)
					{
						lastSt = curSt;
						curSt = st3;
					}					
					break;
					
		case 0x03:
					if(check_crossline()){	pattern = 30;return;	}
					if(check_blackArea()){	pattern = 40;return;	}
										
					if(curSt!=st4) 
					{
						lastSt = curSt;
						curSt = st4;
					}
					if((bit_change(P7DR) )&0x18) return; //不是真正的弯道
					else
					{
						turn_direction = 1; //方向
						pattern = 20;
						return;
					}
		case 0x01:
					if(!(bit_change(P7DR) &0x18)) 
					{
						turn_direction = 1;
						pattern = 20;
						return;
					}
					break;

		//left
		case 0x20:
					if(curSt !=st_1)
					{
						lastSt = curSt;
						curSt = st_1;	
					}	
					break;
					
		case 0x60:
					if(curSt !=st_2)
					{
						lastSt = curSt;
						curSt = st_2;	
					}						
					break;
		case 0x40:
		case 0xe0:
					if(check_crossline()){	pattern = 30;return;	}
					if(check_blackArea()){	pattern = 40;return;	}
					
					if(curSt!=st_3)
					{
						lastSt = curSt;
						curSt = st_3;
					}					
					break;
					
		case 0xc0:
					if(check_crossline()){	pattern = 30;return;	}
					if(check_blackArea()){	pattern = 40;return;	}
										
					if(curSt!=st_4)
					{
						lastSt = curSt;
						curSt = st_4;
					}
					if(( bit_change(P7DR) )&0x18) return; //不是真正的弯道
					else
					{
						turn_direction = -1; //方向
						pattern = 20;
						return;
					}
		case 0x80:
					if(!(bit_change(P7DR) &0x18)) 
					{
						turn_direction = -1;
						pattern = 20;
						return;
					}
					break;
		case 0x81:
					if(!(bit_change(P7DR) &0x18))
					{
						if(curSt>0)	turn_direction = 1;
						else turn_direction = -1;
						pattern = 20;
						return;	
					}
					break;
		default:break;
	}
	
	degree = get_midDegree(curSt);
	speedLevel = get_speedLevel(curSt);
	
	handle(degree);
	do_speed(degree,speedLevel);

}
//弯道
void left_turn()
{	

	int	std_st; 
	unsigned char b;
	int max = 0;	 
	int loop;
	int isSteady;	
	getData(P7DR,-21); //debug-21
	
	//step1:检测是否真是弯道	
	if(sensor_inp(MASK3_3)==0xc0 && (bit_change(P7DR) &0x18)){ pattern = 10; return; }
				
	//step2:左偏27度
	speed(0,60);
	for(degree=-15;degree>=-27;degree--)
		handle(degree);
		
	//step3:到最大偏转程度处理
	curSt = -4;
	loop = 1;
	max = 4;
	cnt2 = 0;
	cnt1 = 0;
	isSteady = 0;
	while(loop || !isSteady)
	{
		b = bit_change(P7DR);
		//step3_1:右侧有灯亮的处理(角度)
		if(b&0x07)
		{
			if((b&0x0f)==0x0f)
			{
				if(curSt!=8)
				{ 
					curSt=8; 
					max = curSt;				
					if(degree>-38) degree = -38;
					else degree--;
					cnt1 = 0; 
				}
				else if(cnt1>60){	degree--; cnt1 = 0;  }	
			}
			//右3灯亮
			else if((b&0x07)==0x07)
			{
				if(curSt!=7)
				{ 
					curSt=7; 
					max = curSt;				
					if(degree>-35) degree = -35;
					else degree--;
					cnt1 = 0; 
				}
				else if(cnt1>60){	degree--; cnt1 = 0;  }	
			}//右2灯亮
			else if((b&0x03)==0x03)
			{
				if(curSt!=6)
				{
					curSt = 6;
					if(curSt>max)
					{
						max = curSt;
						if(degree>-33) degree=-33;
						else degree--;
						cnt1 = 0;
					}
					else loop=0;	
				}else if(cnt1>60){	degree--; cnt1 = 0; }	
			}//右1灯亮
			else if((b&0x01)==0x01 || (b&0xe0)==0x80)
			{
				if(curSt!=5)
				{
					curSt = 5;
					if(curSt>max)
					{
						max = curSt;
						if(degree>-31) degree=-31;
						else degree--;
						cnt1 = 0;
					}
					else loop=0;	
				}else if(cnt1>60){	degree--; cnt1 = 0; }	
			}
			
			handle(degree);
			if(cnt2>400) speed(40,100);
			else if(cnt2>300) speed(20,90);
			else if(cnt2>200) speed(10,85);
			else if(cnt2>100) speed(7,80);
			else if(cnt2>50) speed(3,75);
			else speed(0,60);
		}
		else if((b&0xe7)==0xc0)
		{
			if(max>4)loop = 0;
			if(cnt2<50){ handle(-27);speed(0,60); }
			else if(cnt2<90){ handle(-28); do_speed(-28,70); }
			else if(cnt2<130){ handle(-29);do_speed(-29,80); }
			else{ handle(-30); do_speed(-30,90); }
		}else if((b&0xe7)==0x40 || (b&0xe7)==0xe0 || (b&0xe7)==0x60 || (b&0x20))
			loop = 0;
			
		if(!loop)
		{
			switch(max)
			{
				case 4: if(degree>-27)degree = -27;break;
				case 5: if(degree>-31)degree = -31;break;
				case 6: if(degree>-33)degree = -33;break;
				case 7: if(degree>-35)degree = -35;break;
				case 8:if(degree>-38)degree = -38;break;
			}
			handle(degree);
			b = bit_change(P7DR);
			while(b&0x07)
			{ 
				b = bit_change(P7DR);
				switch(b&0x0f)
				{
					case 0x01: curSt = 5;break;
					case 0x03: curSt = 6;break;
					case 0x07: curSt = 7;break;
					case 0x0f: curSt = 8;break;
					default: curSt = 8;
				}
				if(curSt>=7) speed(0,70);
				else if(max>=7)do_speed(degree,95);
				else do_speed(degree,90);
			}
			isSteady = 1;
		}			
	}//end while
	
	//step4:状态变小处理	
//	degree = (-27+degree)/2;
	if(degree>-27) degree = -27;
	handle(degree);

	curSt = -4;
	speedLevel = 95;	
	cnt1 = 0;
	do
	{
		//step4_1:程序还没出弯时也能检测出弯道
		if(check_crossline())	{	pattern = 30;return;  }				//直角检测

		//step4_2:计算出车身度数对应的标准状态值
		switch(get_near_std(degree))
		{
			case std_4: std_st = -3;break;	/*  (-~-25) 	*/
			case std_3: std_st = -2;break;	/*  [-25,-15) 	*/
			case std_2: std_st = -1;break;	/*  [-15,-10) 	*/
			case std_1: std_st = 0;break;	/*  [-10,-5) 	*/
			case std0:  					/*  [-5,+~) 	*/
						getData(P7DR,-23);  //debug-23
						pattern = 10;
						speedLevel = turnSpeed;
						cnt2 = 0;
						return;
		}

		//step4_3:取传感器当前状态,与车身状态值计算偏转角度
		b= bit_change(P7DR);			
		switch(b&0xe7)
		{
			case 0x80:	degree = -29;break;
			case 0xc0:	if(curSt!=-4)   
						{ 
							curSt=-4;    
							if(degree>-27)
								degree = -27;
							else degree+=(curSt-std_st)*4;  
						}
						break;
			case 0xe0:	if(curSt!=-3) { curSt=-3;    degree+=(curSt-std_st)*4;   }break;
			case 0x60:	if(curSt!=-2) { curSt=-2;    degree+=(curSt-std_st)*4;   }break;
			case 0x20:	if(curSt!=-1) { curSt=-1;    degree+=(curSt-std_st)*4;   }break;
			case 0x00:	if(curSt!=0)  { curSt=0;     degree+=(curSt-std_st)*4;   }break;
			case 0x04:	if(curSt!=1)  { curSt=1;     degree+=(curSt-std_st)*4;   }break;
			case 0x06:  if(curSt!=2)  { curSt=2;     degree+=(curSt-std_st)*4;   }break;													
			default:
				curSt = 1;
				cnt1 = 0;
				max = 0;
				while((bit_change(P7DR)&0x07))
				{
					if((bit_change(P7DR)&0x07)==0x07)
					{
						if(curSt!=3)
						{ 
							curSt = 3;
							cnt1 = 0; 
							if(degree<-35)degree--;
							else degree = -35;
						}
						else if(cnt1>100)
						{
							degree--;
							cnt1 = 0;	
						}
						if(3>max){  max = 3; speedLevel = turnSpeed-21; }
					}
					else if((bit_change(P7DR)&0x03)==0x03)
					{
						if(curSt!=2)
						{ 
							curSt = 2;
							cnt1 = 0; 
							if(degree<-33)degree--;
							else degree = -33;
						}
						else if(cnt1>100)
						{
							degree--;
							cnt1 = 0;	
						}
						if(2>max){  max = 2; speedLevel = turnSpeed-14; }
						else speedLevel = turnSpeed - 5;
					}else if(bit_change(P7DR)&0x01)
					{
						if(curSt!=1)
						{ 
							curSt = 1;
							cnt1 = 0; 
							if(degree<-31)degree--;
							else degree = -31;
						}
						else if(cnt1>100)
						{
							degree--;
							cnt1 = 0;	
						}
						if(1>max){  max = 1; speedLevel = turnSpeed-7; }
						else speedLevel = turnSpeed + 10;
					}
					
					handle(degree);
					do_speed(degree,speedLevel);							
				}//end while
				speedLevel = turnSpeed;
				curSt = -4;
		}//end switch

		handle(degree);
		do_speed(degree,speedLevel);
	}while(1);
}

void right_turn()
{
	
	int	std_st; 
	unsigned char b;
	int max = 0;	 
	int loop;
	int isSteady;
	
	
	getData(P7DR,21); //debug21
	
	//step1:检测是否真是弯道	
	if(sensor_inp(MASK3_3)==0x03 && (bit_change(P7DR) &0x18)){ pattern = 10; return; }
				
	//step2:右偏27度
	speed(60,0);
	for(degree=15;degree<=27;degree++)
		handle(degree);
		
	//step3:到最大偏转程度处理
	curSt = 4;
	loop = 1;
	max = 4;
	cnt2 = 0;
	cnt1 = 0;
	isSteady = 0;
	while(loop || !isSteady)
	{
		b = bit_change(P7DR);
		if(b&0xe0)
		{
			if((b&0xf0)==0xf0)
			{
				if(curSt!=8)
				{ 
					curSt=8; 
					max = curSt;				
					if(degree<38) degree = 38;
					else degree++;
					cnt1 = 0; 
				}
				else if(cnt1>60){	degree++; cnt1 = 0;  }	
			}
			//左3灯亮
			else if((b&0xe0)==0xe0)
			{
				if(curSt!=7)
				{ 
					curSt=7; 
					max = curSt;				
					if(degree<35) degree = 35;
					else degree++;
					cnt1 = 0; 
				}
				else if(cnt1>60){	degree++; cnt1 = 0;  }	
			}//左2灯亮
			else if((b&0xc0)==0xc0)
			{
				if(curSt!=6)
				{
					curSt = 6;
					if(curSt>max)
					{
						max = curSt;
						if(degree<33) degree=33;
						else degree++;
						cnt1 = 0;
					}
					else loop=0;	
				}else if(cnt1>60){	degree++; cnt1 = 0; }	
			}//左1灯亮
			else if((b&0x80)==0x80 || (b&0x07)==0x01)
			{
				if(curSt!=5)
				{
					curSt = 5;
					if(curSt>max)
					{
						max = curSt;
						if(degree<31) degree=31;
						else degree++;
						cnt1 = 0;
					}
					else loop=0;	
				}else if(cnt1>60){	degree++; cnt1 = 0; }	
			}
			
			handle(degree);
			if(cnt2>400) speed(100,40);
			else if(cnt2>300) speed(90,20);
			else if(cnt2>200) speed(85,10);
			else if(cnt2>100) speed(80,7);
			else if(cnt2>50) speed(75,3);
			else speed(60,0);
		}
		else if((b&0xe7)==0x03)
		{
			if(max>4)loop = 0;
			if(cnt2<50){ handle(27);speed(60,0); }
			else if(cnt2<90){ handle(28); do_speed(28,70); }
			else if(cnt2<130){ handle(29);do_speed(29,80); }
			else{ handle(30); do_speed(30,90); }
		}else if((b&0xe7)==0x02 || (b&0xe7)==0x07 || (b&0xe7)==0x06 || (b&0x04))
			loop = 0;
			
		if(!loop)
		{
			switch(max)
			{
				case 4: if(degree<27)degree = 27;break;
				case 5: if(degree<31)degree = 31;break;
				case 6: if(degree<33)degree = 33;break;
				case 7: if(degree<35)degree = 35;break;
				case 8: if(degree<38)degree = 38;break;
			}
			handle(degree);
			b = bit_change(P7DR);
			while(b&0xe0)
			{ 
				b = bit_change(P7DR);
				switch(b&0xf0)
				{
					case 0x80: curSt = 5;break;
					case 0xc0: curSt = 6;break;
					case 0xe0: curSt = 7;break;
					case 0xf0: curSt = 8;break;
					default: curSt = 8;
				}
				if(curSt>=7) speed(70,0);
				else if(max>=7)do_speed(degree,95);
				else do_speed(degree,90);
			}
			isSteady = 1;
		}			
	}//end while
	
	//step4:状态变小处理	
	degree = (27+degree)/2;
	if(degree<27) degree = 27;
	handle(degree);

	curSt = 4;
	speedLevel = 95;	
	cnt1 = 0;
	do
	{
		if(check_crossline())		//直角检测
		{
			pattern = 30;
			return;
		}
		switch(get_near_std(degree))
		{
			case std4: std_st = 3;break;  /* (25,+~) */					
			case std3: std_st = 2;break;	/* (15,25] */
			case std2: std_st = 1;break;	/* (10,15] */
			case std1: std_st = 0;break;	/* (5,10] */
			case std0: /* [-5,+~) */
			getData(P7DR,23); //debug23
						pattern = 10;
						speedLevel = turnSpeed;
						cnt2 = 0;
						return;
		}
			
		switch(sensor_inp(MASK3_3))
		{
			case 0x01:	degree = 29;break;
			case 0x03:	if(curSt!=4)   
						{ 
							curSt=4;    
							if(degree<27)
								degree = 27;
							else degree+=(curSt-std_st)*4;  
						}
						break;
			case 0x07:	if(curSt!=3)   { curSt=3;    degree+=(curSt-std_st)*4;   }break;
			case 0x06:	if(curSt!=2)   { curSt=2;    degree+=(curSt-std_st)*4;   }break;
			case 0x04:	if(curSt!=1)   { curSt=1;    degree+=(curSt-std_st)*4;   }break;
			case 0x00:	if(curSt!=0)   { curSt=0;    degree+=(curSt-std_st)*4;   }break;
			case 0x20:	if(curSt!=-1)  { curSt=-1;   degree+=(curSt-std_st)*4;   }break;
			case 0x60:  if(curSt!=-2)  { curSt=-2;   degree+=(curSt-std_st)*4;   }break;													
			
			default:
					curSt = 1;
					cnt1 = 0;
					max = 0;
					while((bit_change(P7DR)&0xe0))
					{
						if((bit_change(P7DR)&0xe0)==0xe0)
						{
							if(curSt!=3)
							{ 
								curSt = 3;
								cnt1 = 0; 
								if(degree>35)degree++;
								else degree = 35;
							}
							else if(cnt1>100)
							{
								degree++;
								cnt1 = 0;	
							}
							if(3>max){  max = 3; speedLevel = turnSpeed-21; }
						}
						else if((bit_change(P7DR)&0xc0)==0xc0)
						{
							if(curSt!=2)
							{ 
								curSt = 2;
								cnt1 = 0; 
								if(degree>33)degree++;
								else degree = 33;
							}
							else if(cnt1>100)
							{
								degree++;
								cnt1 = 0;	
							}
							if(2>max){  max = 2; speedLevel = turnSpeed-14; }
							else speedLevel = turnSpeed - 5;
						}else if(bit_change(P7DR)&0x80)
						{
							if(curSt!=1)
							{ 
								curSt = 1;
								cnt1 = 0; 
								if(degree>31)degree++;
								else degree = 31;
							}
							else if(cnt1>100)
							{
								degree++;
								cnt1 = 0;	
							}
							if(1>max){  max = 1; speedLevel = turnSpeed-7; }
							else speedLevel = turnSpeed + 10;
						}
						handle(degree);
						do_speed(degree,speedLevel);							
					}//end while
					speedLevel = turnSpeed;	
					curSt = 4;	
		}//end switch
		
		handle(degree);
		do_speed(degree,speedLevel);
	}while(1);

}

//直角
void rightAngle()
{
	unsigned char b;
	int std_st;	
	int waitForAngle;
	int AngleDirection;	
	int max = 0 ;
	int var;
	int reachMax;	
	int speedLevel_;
	int degree_;
	
	int speedToMax;
	getData(P7DR,31); 								//debug31
	
	//step1:过双白线(31ms 34ms 34ms)
	led_out(0x03);
	handle(0);
	speed(0,0); //before speed(60,60);
	cnt1 = 0;
	var = 0;
	
	
	if(eCurSpeed>130)
	{
		speed(-30,-30);
		timer(180);
	}
	
	while(cnt1<100)
	{
		if(var==0 && getLightLedSum()<=2)var++;
		if(var==1 && getLightLedSum()>2)var++;
		if(var==2 && getLightLedSum()<=2)var++;
		if(var==3)break;
	}
	
	//////////encode flag////////
	
	crankFlag=1;
	//speed(0,0);
	if(cnt1<32) speedLevel_ = 38;
	else if(cnt1<35) speedLevel_ = 42;
	else if(cnt1<40) speedLevel_ = 43;
	else speedLevel_ = 45;		
	led_out(0x00);
	getData(P7DR,32); 								//debug32

	//step2:减速调节(与过双白线的时间有关)
	cnt1 = 0;
	while(cnt1<300)
	{	
		speedLevel = 90;						//fast -> slow
		if(cnt1>250) speedLevel = speedLevel+3; 		//41 45 48 53
		else if(cnt1>200) speedLevel =speedLevel+2; 	//45 49 52 57
		else if(cnt1>150) speedLevel = speedLevel;	//48 52 55 60
				
		switch( sensor_inp(MASK3_3) ) {
            case 0x00:
				degree = 0;
                break;
            case 0x04:
				degree = 5;
				break;
            case 0x06:
				degree = 10;
				break;
            case 0x07:
			case 0x27:
			case 0x67:
				cnt1=1000;
                break;
            case 0x20:
				degree = -5;
				break;
            case 0x60:
				degree = -10;
				break;
            case 0xe0:
			case 0xe4:
			case 0xe6:
				cnt1=1000;
				break;
            default:
                break;
        }
  		handle(degree);
		do_speed(degree,speedLevel);
	}
	getData(P7DR,33); //debug33

	//step3:等待直角到来(175ms)
	speedLevel =90;
	waitForAngle = 1;
	cnt1 = 0;
	while(waitForAngle)
	{
		if(cnt1>100) speedLevel = 90;
		else if(cnt1>50) speedLevel = 90;
		switch( sensor_inp(MASK3_3) ) {	
				case 0xa0:			
	            case 0xe0:
				case 0xe4:
				case 0xe6:
					speed(0,0);
					for(degree=-1;degree>=-44;degree-=1) handle(degree);  //before -40
					led_out( 0x1 );	                
					AngleDirection = -1;
					waitForAngle = 0;
					//step4:忽略直角白线
					getData(P7DR,-34);				//debug-34
	                break;
				case 0x05:
	            case 0x07:
				case 0x27:
					speed(0,0);
					for(degree=1;degree<=44;degree+=1)	handle( degree );   //before 40
					led_out( 0x2 );	                
					AngleDirection = 1;
					waitForAngle = 0;
					//step4:忽略直角白线
					getData(P7DR,34);				//debug34
	                break;
	            case 0x00:
	                handle( 0 );
	                speed(speedLevel,speedLevel);
	                break;
	            case 0x04:
					handle(5);
					do_speed(5,speedLevel);
					break;
	            case 0x06:
					handle(10);
					do_speed(10,speedLevel);
					break;
	            case 0x03:
	                handle( 15 );
				    do_speed(15,speedLevel);
	                break;
	            case 0x20:
					handle(-5);
					do_speed(-5,speedLevel);
					break;
	            case 0x60:
					handle(-10);
				 	do_speed(-10,speedLevel);
					break;
	            case 0xc0:
	                handle( -15 );
				   	do_speed(-15,speedLevel);
	                break;
	            default:
	                break;
	        }
		 	if(!sensor_inp(MASK4_4)) //是盲区误判为直角
			{
				pattern = 40;
				return;
			}
	}	
	///////encode ///////////////
	
	crankFlag=0;
	//转直角过程处理(左右)
	switch(AngleDirection)
	{
		case -1:
				cnt1 = 0;
				while(cnt1<50)
				{
					if(!bit_change(P7DR) ) break;//忽略白线(16ms)
				}
				var = cnt1;
				getData(P7DR,-35); 				//debug-35(经过白线)
				
				//step5:经过黑区(摆头初期)
				if(var<17){ degree_ = -43; speedLevel_ = 45; }
				else if(var<25){ degree_ = -42; speedLevel_ = 48; }
				else if(var<35){ degree_ = -41; speedLevel_ = 51; }
				else if(var<45){ degree_ = -40; speedLevel_ = 54; }
				else { degree_ = -40; speedLevel_ = 57; }
				
				cnt1 = 0;
				degree = degree_;
				speedLevel = speedLevel_;
				while(!bit_change(P7DR) )					//根据白线时间var决定黑区偏转度数
				{
					handle(degree);
					speed(0,speedLevel);	
				}				
				getData(P7DR,-36); 				//debug-36
				
				//step6:摆头过程				
				speedLevel = 60;
				cnt1 = 0;
				reachMax = 0;
				max = 0;
				curSt = 0;
				b = bit_change(P7DR);
				while((b&0xe7)!=0x60 && (b&0xe7)!=0x20)
				{
					if(!reachMax)				//最大状态前
					{
						if(degree>-40)
							degree = -40;
						else degree = degree_;
						
						switch(b&0xe7)
						{
							case 0x00: if(curSt!=0){ curSt = 0; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(0,75);break;
							case 0x20: if(curSt!=1){ curSt = 1; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(0,75);break;
							case 0x60: if(curSt!=2){ curSt = 2; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(0,75);break;
							case 0xe0: if(curSt!=3){ curSt = 3; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(0,75);break;
							case 0xc0: if(curSt!=4){ curSt = 4; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(0,75);break;
							case 0x80: if(curSt!=5){ curSt = 5; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(0,75);break;
							default:
								if((b&0x0f)==0x0f){  curSt = 9; if(degree>-44)degree = -44; max= 9; speed(0,52); }
								else if((b&0x07)==0x07)
								{ 
									if(curSt!=8)
									{ 
										curSt=8; 
										if(degree>-43)
											degree=-43; 
										if(curSt>=max) 
											max = curSt;
										else
										{ 
											reachMax = 1;
											cnt1 = 0; 
										} 
									}
									speed(0,51);
								}
								else if((b&0x03)==0x03)
								{ 
									if(curSt!=7)
									{ 
										curSt=7; 
										if(degree>-42)
											degree=-42; 
										if(curSt>=max) 
											max = curSt;
										else
										{ 
											reachMax = 1;
											cnt1 = 0; 
										} 
									}
									speed(0,50);
								}
								else if(b&0x01)
								{
								  	if(curSt!=6)
									{ 
										curSt=6; 
										if(degree>-41)
											degree=-41; 
										if(curSt>=max) 
											max = curSt;
										else
										{ 
											reachMax = 1;
											cnt1 = 0; 
										}   
									}
									speed(0,45);
								}
						}
						handle(degree);
			
					}else
					{
						if(degree>-40)
							degree = -40;								//最大状态后  (86ms)
						while(bit_change(P7DR)&0x07)
						{ 
							handle(degree);						
							if(cnt1>85)do_speed(-38,95);
							else if(cnt1>65)do_speed(-38,93);
							else if(cnt1>50)do_speed(-38,91);
							else if(cnt1>35)speed(get_inner_speed(-38,89)-5,89);
							else if(cnt1>20)speed(get_inner_speed(-38,87)-10,87);
							else speed(0,85);
						}
						handle(-38);
						do_speed(-38,90);
					}
					
					b = bit_change(P7DR);	
				}//end while
				
				//step7:直角偏转结束,调整车身出直角
				getData(P7DR,-37);		
				speedLevel = 90;
				cnt1 = 0;
				curSt = -2;
				do{				
						switch(get_near_std(degree))
						{
							case std_4: std_st = -3;break; //-27
							case std_3: std_st = -2;break; //-15
							case std_2: std_st = -1;break; //-10
							case std_1: std_st = 0;break; //-5
							case std0:  				  //0
									led_out( 0x0 );									
									pattern = 10;
				//step8:结束函数    
									//crankFlag=0;
									getData(P7DR,-38); //debug-38
									return;
						}
						switch(sensor_inp(MASK3_3))
						{
							case 0xc0:  if(curSt!=-4) { curSt=-4;    degree+=(curSt-std_st)*4;   }break;
							case 0xe0:	if(curSt!=-3) { curSt=-3;    degree+=(curSt-std_st)*4;   }break;
							case 0x60:	if(curSt!=-2) { curSt=-2;    degree+=(curSt-std_st)*4;   }break;
							case 0x20:	if(curSt!=-1) { curSt=-1;    degree+=(curSt-std_st)*4;   }break;
							case 0x00:	if(curSt!=0)  { curSt=0;    degree+=(curSt-std_st)*4;   }break;
							case 0x04:	if(curSt!=1)  { curSt=1;   degree+=(curSt-std_st)*4;   }break;
							case 0x06:  if(curSt!=2)  { curSt=2;   degree+=(curSt-std_st)*4;   }break;
							case 0x07:  if(curSt!=3)  { curSt=3;   degree+=(curSt-std_st)*4;   }break;	
						}
						handle(degree);
						do_speed(degree,speedLevel);
					
				}while(1);
				

		case 1:
				cnt1 = 0;
				while(cnt1<50)
				{
					if(!bit_change(P7DR) ) break;//忽略白线(16ms)
				}
				var = cnt1;
				getData(P7DR,35); 				//debug35(经过白线)
				
				//step5:经过黑区(摆头初期)
				if(var<17){ degree_ = 43; speedLevel_ = 45; }
				else if(var<25){ degree_ = 42; speedLevel_ = 48; }
				else if(var<35){ degree_ = 41; speedLevel_ = 51; }
				else if(var<45){ degree_ = 40; speedLevel_ = 54; }
				else { degree_ = 40; speedLevel_ = 57; }
				
				cnt1 = 0;
				degree = degree_;
				speedLevel = speedLevel_;
				while(!bit_change(P7DR) )					//根据白线时间var决定黑区偏转度数
				{
					handle(degree);
					speed(speedLevel,0);	
				}				
				getData(P7DR,36); 				//debug36
				
				//step6:摆头过程				
				speedLevel = 60;
				cnt1 = 0;
				reachMax = 0;
				max = 0;
				curSt = 0;
				b = bit_change(P7DR);
				while((b&0xe7)!=0x06 && (b&0xe7)!=0x04)
				{
					if(!reachMax)				//最大状态前
					{
						if(degree<40)
							degree = 40;
						else degree = degree_;
						
						switch(b&0xe7)
						{
							case 0x00: if(curSt!=0){ curSt = 0; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(75,0);break;
							case 0x04: if(curSt!=1){ curSt = 1; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(75,0);break;
							case 0x06: if(curSt!=2){ curSt = 2; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(75,0);break;
							case 0x07: if(curSt!=3){ curSt = 3; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(75,0);break;
							case 0x03: if(curSt!=4){ curSt = 4; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(75,0);break;
							case 0x01: if(curSt!=5){ curSt = 5; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(75,0);break;
							default:
								if((b&0xf0)==0xf0){  curSt = 9; if(degree<44)degree = 44; max= 9; speed(52,0); }
								else if((b&0xe0)==0xe0)
								{ 
									if(curSt!=8)
									{ 
										curSt=8; 
										if(degree<43)
											degree=43; 
										if(curSt>=max) 
											max = curSt;
										else
										{ 
											reachMax = 1;
											cnt1 = 0; 
										} 
									}
									speed(51,0);
								}
								else if((b&0xc0)==0xc0)
								{ 
									if(curSt!=7)
									{ 
										curSt=7; 
										if(degree<42)
											degree=42; 
										if(curSt>=max) 
											max = curSt;
										else
										{ 
											reachMax = 1;
											cnt1 = 0; 
										} 
									}
									speed(50,0);
								}
								else if(b&0x80)
								{
								  	if(curSt!=6)
									{ 
										curSt=6; 
										if(degree<41)
											degree=41; 
										if(curSt>=max) 
											max = curSt;
										else
										{ 
											reachMax = 1;
											cnt1 = 0; 
										}   
									}
									speed(45,0);
								}
						}
						handle(degree);
			
					}else
					{
						if(degree<40)
							degree = 40;								//最大状态后  (86ms)
						while(bit_change(P7DR)&0xe0)
						{ 
							handle(degree);						
							if(cnt1>85)do_speed(38,95);
							else if(cnt1>65)do_speed(38,93);
							else if(cnt1>50)do_speed(38,91);
							else if(cnt1>35)speed(get_inner_speed(38,89)-5,89);
							else if(cnt1>20)speed(get_inner_speed(38,87)-10,87);
							else speed(85,0);
						}
						handle(38);
						do_speed(38,90);
					}
					
					b = bit_change(P7DR);	
				}//end while
				
				//step7:直角偏转结束,调整车身出直角
				getData(P7DR,37);							//debug37	
				speedLevel = 90;
				cnt1 = 0;
				curSt = 2;
				do{				
						switch(get_near_std(degree))
						{
							case std4: std_st = 3;break;
							case std3: std_st = 2;break;/* [-25,-15) */
							case std2: std_st = 1;break;/* [-15,-10) */
							case std1: std_st = 0;break;/* [-10,-5) */
							case std0:  /* [-5,+~) */
									led_out( 0x0 );					
									pattern = 10;
									//crankFlag=0;
									getData(P7DR,39); //debug39
									return;
						}
						switch(sensor_inp(MASK3_3))
						{
							case 0xe0:	if(curSt!=-3) { curSt=-3;   degree+=(curSt-std_st)*4;  }break;
							case 0x60:	if(curSt!=-2) { curSt=-2;   degree+=(curSt-std_st)*4;  }break;
							case 0x20:	if(curSt!=-1) { curSt=-1;   degree+=(curSt-std_st)*4;  }break;
							case 0x00:	if(curSt!=0)  { curSt=0;    degree+=(curSt-std_st)*4;  }break;
							case 0x04:	if(curSt!=1)  { curSt=1;    degree+=(curSt-std_st)*4;  }break;
							case 0x06:  if(curSt!=2)  { curSt=2;    degree+=(curSt-std_st)*4;  }break;
							case 0x07:  if(curSt!=3)  { curSt=3;    degree+=(curSt-std_st)*4;  }break;	
							case 0x03:  if(curSt!=4)  { curSt=4;    degree+=(curSt-std_st)*4;  }break;
						}
						handle(degree);
						do_speed(degree,speedLevel);
					
				}while(1);	

	}
}

//********************************* MCU5 0413 修改*******************************************
//直角
void rightAngle2()
{
	unsigned char b;
	int std_st;
	int waitForAngle;
	int AngleDirection;
	int max = 0 ;
	int var;
	int reachMax;
	int speedLevel_;
	int degree_;

	int speedToMax;
	getData(P7DR,31); 								//debug31

	//step1:过双白线(31ms 34ms 34ms)
	handle(0);
	speed(15,15); //before speed(60,60);(-45)
	cnt1 = 0;
	var = 0;
	while(cnt1<100)
	{
		if(var==0 && getLightLedSum()<=2)var++;
		if(var==1 && getLightLedSum()>2)var++;
		if(var==2 && getLightLedSum()<=2)var++;
		if(var==3)break;
	}

	//确定速度的过程
	var = 0;
	if(eCurSpeed>250)speedLevel_ =15+var;
	else if(eCurSpeed>230)speedLevel_ =25+var;
	else if(eCurSpeed>200)speedLevel_ =27+var;
	else if(eCurSpeed>180)speedLevel_ =30+var;
	else if(eCurSpeed>160)speedLevel_ =32+var;
	else if(eCurSpeed>150)speedLevel_ =35+var;
	else speedLevel_ =40+var;

	getData(P7DR,32); 								//debug32


	//step2:减速调节(与过双白线的时间有关)
	speed(17,17);//47
	cnt1=0;
	while(cnt1<110)
	{
		switch( sensor_inp(MASK3_3) ) {
            case 0x00:
				degree = 0;
                break;
            case 0x04:
				degree = 3;
				break;
            case 0x06:
				degree = 7;
				break;
            case 0x20:
				degree = -3;
				break;
            case 0x60:
				degree = -7;
				break;
            default:
                break;
        }
  		handle(degree);
	}

	waitForAngle = 1;
	cnt1 = 0;
	while(cnt1<300)
	{
		speedLevel = speedLevel_;						//fast -> slow

		switch( sensor_inp(MASK3_3) ) {
            case 0x00:
				degree = 0;
                break;
            case 0x04:
				degree = 3;
				break;
            case 0x06:
				degree = 7;
				break;
			case 0x05:
            case 0x07:
			case 0x27:
			case 0x67:
				waitForAngle =0;
				AngleDirection = 1;
				handle(45);
				cnt1=1000;
                break;
            case 0x20:
				degree = -3;
				break;
            case 0x60:
				degree = -7;
				break;
			case 0xa0:
            case 0xe0:
			case 0xe4:
			case 0xe6:
				waitForAngle =0;
				AngleDirection = -1;
				handle(-45);
				cnt1=1000;
				break;
            default:
                break;
        }
  		handle(degree);
		do_speed(degree,speedLevel);
	}
	getData(P7DR,33); //debug33

	//step3:等待直角到来(175ms)
	speedLevel = 48; //48
	waitForAngle=1;
	cnt1 = 0;
	while(waitForAngle)
	{
		if(cnt1>100) speedLevel = 57;//57
		else if(cnt1>50) speedLevel = 56;//56
		switch( sensor_inp(MASK3_3) ) {
				case 0xa0:
	            case 0xe0:
				case 0xe4:
				case 0xe6:
					speed(0,0);
					//for(degree=-1;degree>=-44;degree-=1) handle(degree);  //before -40
					handle(-45);
					AngleDirection = -1;
					waitForAngle = 0;
					//step4:忽略直角白线
					getData(P7DR,-34);				//debug-34
	                break;
				case 0x05:
	            case 0x07:
				case 0x27:
					speed(0,0);
					//for(degree=1;degree<=44;degree+=1)	handle( degree );   //before 40
					handle(53);//4.13 45
					AngleDirection = 1;
					waitForAngle = 0;
					//step4:忽略直角白线
					getData(P7DR,34);				//debug34
	                break;
	            case 0x00:
	                handle( 0 );
	                speed(speedLevel,speedLevel);
	                break;
	            case 0x04:
					handle(5);
					do_speed(5,speedLevel);
					break;
	            case 0x06:
					handle(10);
					do_speed(10,speedLevel);
					break;
	            case 0x03:
	                handle( 15 );
				    do_speed(15,speedLevel);
	                break;
	            case 0x20:
					handle(-5);
					do_speed(-5,speedLevel);
					break;
	            case 0x60:
					handle(-10);
				 	do_speed(-10,speedLevel);
					break;
	            case 0xc0:
	                handle( -15 );
				   	do_speed(-15,speedLevel);
	                break;
	            default:
	                break;
	        }
	}

	//转直角过程处理(左右)
	switch(AngleDirection)
	{
		case -1:
				cnt1 = 0;
				while(cnt1<50)
				{
					if(!bit_change(P7DR) ) break;//忽略白线(16ms)
				}
				var = cnt1;
				getData(P7DR,-35); 				//debug-35(经过白线)

				//step5:经过黑区(摆头初期)
				if(var<17){ degree_ = -43; speedLevel_ = 45; }
				else if(var<25){ degree_ = -42; speedLevel_ = 48; }
				else if(var<35){ degree_ = -41; speedLevel_ = 51; }
				else if(var<45){ degree_ = -40; speedLevel_ = 54; }
				else { degree_ = -40; speedLevel_ = 57; }

				cnt1 = 0;
				degree = degree_;
				speedLevel = speedLevel_;
				while(!bit_change(P7DR) )					//根据白线时间var决定黑区偏转度数
				{
					handle(degree);
					speed(0,speedLevel);
				}
				getData(P7DR,-36); 				//debug-36

				//step6:摆头过程
				speedLevel = 60;
				cnt1 = 0;
				reachMax = 0;
				max = 0;
				curSt = 0;
				b = bit_change(P7DR);
				while((b&0xe7)!=0x60 && (b&0xe7)!=0x20)
				{
					if(!reachMax)				//最大状态前
					{
						if(degree>-40)
							degree = -40;
						else degree = degree_;

						switch(b&0xe7)
						{
							case 0x00: if(curSt!=0){ curSt = 0; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(0,75);break;
							case 0x20: if(curSt!=1){ curSt = 1; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(0,75);break;
							case 0x60: if(curSt!=2){ curSt = 2; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(0,75);break;
							case 0xe0: if(curSt!=3){ curSt = 3; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(0,75);break;
							case 0xc0: if(curSt!=4){ curSt = 4; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(0,75);break;
							case 0x80: if(curSt!=5){ curSt = 5; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(0,75);break;
							default:
								if((b&0x0f)==0x0f){  curSt = 9; if(degree>-44)degree = -44; max= 9; speed(0,52); }
								else if((b&0x07)==0x07)
								{
									if(curSt!=8)
									{
										curSt=8;
										if(degree>-43)
											degree=-43;
										if(curSt>=max)
											max = curSt;
										else
										{
											reachMax = 1;
											cnt1 = 0;
										}
									}
									speed(0,51);
								}
								else if((b&0x03)==0x03)
								{
									if(curSt!=7)
									{
										curSt=7;
										if(degree>-42)
											degree=-42;
										if(curSt>=max)
											max = curSt;
										else
										{
											reachMax = 1;
											cnt1 = 0;
										}
									}
									speed(0,50);
								}
								else if(b&0x01)
								{
								  	if(curSt!=6)
									{
										curSt=6;
										if(degree>-41)
											degree=-41;
										if(curSt>=max)
											max = curSt;
										else
										{
											reachMax = 1;
											cnt1 = 0;
										}
									}
									speed(0,45);
								}
						}
						handle(degree);

				}else
					{
						if(degree>-40)
							degree = -40;								//最大状态后  (86ms)
						while(bit_change(P7DR)&0x07)
						{
							handle(degree);
							if(cnt1>85)do_speed(-38,95);
							else if(cnt1>65)do_speed(-38,93);
							else if(cnt1>50)do_speed(-38,91);
							else if(cnt1>35)speed(get_inner_speed(-38,89)-5,89);
							else if(cnt1>20)speed(get_inner_speed(-38,87)-10,87);
							else speed(0,85);
						}
						handle(-38);
						do_speed(-38,90);
					}

					b = bit_change(P7DR);
				}//end while

				//step7:直角偏转结束,调整车身出直角
				getData(P7DR,-37);
				speedLevel = 90;
				cnt1 = 0;
				curSt = -2;
				do{
						switch(get_near_std(degree))
						{
							case std_4: std_st = -3;break; //-27
							case std_3: std_st = -2;break; //-15
							case std_2: std_st = -1;break; //-10
							case std_1: std_st = 0;break; //-5
							case std0:  				  //0
									led_out( 0x0 );
									pattern = 10;
				//step8:结束函数
									getData(P7DR,-38); //debug-38
									return;
						}
						switch(sensor_inp(MASK3_3))
						{
							case 0xc0:  if(curSt!=-4) { curSt=-4;    degree+=(curSt-std_st)*4;   }break;
							case 0xe0:	if(curSt!=-3) { curSt=-3;    degree+=(curSt-std_st)*4;   }break;
							case 0x60:	if(curSt!=-2) { curSt=-2;    degree+=(curSt-std_st)*4;   }break;
							case 0x20:	if(curSt!=-1) { curSt=-1;    degree+=(curSt-std_st)*4;   }break;
							case 0x00:	if(curSt!=0)  { curSt=0;    degree+=(curSt-std_st)*4;   }break;
							case 0x04:	if(curSt!=1)  { curSt=1;   degree+=(curSt-std_st)*4;   }break;
							case 0x06:  if(curSt!=2)  { curSt=2;   degree+=(curSt-std_st)*4;   }break;
							case 0x07:  if(curSt!=3)  { curSt=3;   degree+=(curSt-std_st)*4;   }break;
						}
						handle(degree);
						do_speed(degree,speedLevel);

				}while(1);


		case 1:
				cnt1 = 0;
				while(cnt1<50)
				{
					if(!bit_change(P7DR) ) break;//忽略白线(16ms)
				}
				var = cnt1;
				getData(P7DR,35); 				//debug35(经过白线)

				//step5:经过黑区(摆头初期)
				if(var<17){ degree_ = 43; speedLevel_ = 45; }
				else if(var<25){ degree_ = 42; speedLevel_ = 48; }
				else if(var<35){ degree_ = 41; speedLevel_ = 51; }
				else if(var<45){ degree_ = 40; speedLevel_ = 54; }
				else { degree_ = 40; speedLevel_ = 57; }

				cnt1 = 0;
				degree = degree_;
				speedLevel = speedLevel_;
				while(!bit_change(P7DR) )					//根据白线时间var决定黑区偏转度数
				{
					handle(degree);
					speed(speedLevel,0);
				}
				getData(P7DR,36); 				//debug36

				//step6:摆头过程
				speedLevel = 60;
				cnt1 = 0;
				reachMax = 0;
				max = 0;
				curSt = 0;
				b = bit_change(P7DR);
				while((b&0xe7)!=0x06 && (b&0xe7)!=0x04)
				{
					if(!reachMax)				//最大状态前
					{
						if(degree<40)
							degree = 40;
						else degree = degree_;

						switch(b&0xe7)
						{
							case 0x00: if(curSt!=0){ curSt = 0; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(75,0);break;
							case 0x04: if(curSt!=1){ curSt = 1; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(75,0);break;
							case 0x06: if(curSt!=2){ curSt = 2; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(75,0);break;
							case 0x07: if(curSt!=3){ curSt = 3; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(75,0);break;
							case 0x03: if(curSt!=4){ curSt = 4; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(75,0);break;
							case 0x01: if(curSt!=5){ curSt = 5; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(75,0);break;
							default:
								if((b&0xf0)==0xf0){  curSt = 9; if(degree<44)degree = 44; max= 9; speed(52,0); }
								else if((b&0xe0)==0xe0)
								{
									if(curSt!=8)
									{
										curSt=8;
										if(degree<43)
											degree=43;
										if(curSt>=max)
											max = curSt;
										else
										{
											reachMax = 1;
											cnt1 = 0;
										}
									}
									speed(51,0);
								}
								else if((b&0xc0)==0xc0)
								{
									if(curSt!=7)
									{
										curSt=7;
										if(degree<42)
											degree=42;
										if(curSt>=max)
											max = curSt;
										else
										{
											reachMax = 1;
											cnt1 = 0;
										}
									}
									speed(50,0);
								}
								else if(b&0x80)
								{
								  	if(curSt!=6)
									{
										curSt=6;
										if(degree<41)
											degree=41;
										if(curSt>=max)
											max = curSt;
										else
										{
											reachMax = 1;
											cnt1 = 0;
										}
									}
									speed(45,0);
								}
						}
						handle(degree);

					}else
					{
						if(degree<40)
							degree = 40;								//最大状态后  (86ms)
						while(bit_change(P7DR)&0xe0)
						{
							handle(degree);
							if(cnt1>85)do_speed(38,95);
							else if(cnt1>65)do_speed(38,93);
							else if(cnt1>50)do_speed(38,91);
							else if(cnt1>35)speed(get_inner_speed(38,89)-5,89);
							else if(cnt1>20)speed(get_inner_speed(38,87)-10,87);
							else speed(85,0);
						}
						handle(38);
						do_speed(38,90);
					}

					b = bit_change(P7DR);
				}//end while

				//step7:直角偏转结束,调整车身出直角
				getData(P7DR,37);							//debug37
				speedLevel = 90;
				cnt1 = 0;
				curSt = 2;
				do{
						switch(get_near_std(degree))
						{
							case std4: std_st = 3;break;
							case std3: std_st = 2;break;/* [-25,-15) */
							case std2: std_st = 1;break;/* [-15,-10) */
							case std1: std_st = 0;break;/* [-10,-5) */
							case std0:  /* [-5,+~) */
									led_out( 0x0 );
									pattern = 10;
									getData(P7DR,39); //debug39
									return;
						}
						switch(sensor_inp(MASK3_3))
						{
							case 0xe0:	if(curSt!=-3) { curSt=-3;   degree+=(curSt-std_st)*4;  }break;
							case 0x60:	if(curSt!=-2) { curSt=-2;   degree+=(curSt-std_st)*4;  }break;
							case 0x20:	if(curSt!=-1) { curSt=-1;   degree+=(curSt-std_st)*4;  }break;
							case 0x00:	if(curSt!=0)  { curSt=0;    degree+=(curSt-std_st)*4;  }break;
							case 0x04:	if(curSt!=1)  { curSt=1;    degree+=(curSt-std_st)*4;  }break;
							case 0x06:  if(curSt!=2)  { curSt=2;    degree+=(curSt-std_st)*4;  }break;
							case 0x07:  if(curSt!=3)  { curSt=3;    degree+=(curSt-std_st)*4;  }break;
							case 0x03:  if(curSt!=4)  { curSt=4;    degree+=(curSt-std_st)*4;  }break;
						}
						handle(degree);
						do_speed(degree,speedLevel);

				}while(1);

	}
}

//===========================================================================================

//盲区
void left_blackArea()
{
	handle(0);
	getData(bit_change(P7DR),80); //debug80
	while(sensor_inp(MASK4_4))
	{
		if(check_crossline())		//是直线，错误检测为盲区
		{
			pattern = 30;
			getData(bit_change(P7DR),85); //debug85
			return;
		}
		handle(0);
		speed(60,60);
		switch(sensor_inp(MASK3_3))
		{
			case 0x00:
                handle( 0 );
                speed( speedLevel ,speedLevel );
                break;
            case 0x04:
				handle(4);
				do_speed(4,speedLevel);
				break;
            case 0x06:
				handle(8);
				do_speed(8,speedLevel);
				break;
			case 0x07:
				handle(12);
				do_speed(12,speedLevel);
				break;
            case 0x03:
                handle(20);
                do_speed(20,speedLevel);
                break;
            case 0x20:
				handle(-4);
				do_speed(-4,speedLevel);
				break;
            case 0x60:
				handle(-8);
				do_speed(-8,speedLevel);
				break;
			case 0xe0:
				handle(-12);
				do_speed(-12,speedLevel);
				break;
            case 0xc0:
                handle( -20);
                do_speed(-20,speedLevel);
                break;
			default:
				do_speed(speedLevel,speedLevel);
		}
	}
	
	speed(0,55);
	degree=-1;
	while(degree>=-40)
	{
		handle(degree);
		degree-=4;                   //xxxxxxx
	}
	while(degree<-23)
	{
		handle(degree);
		degree++;
	}
	handle(-23);
	timer(5);
	while(!(bit_change(P7DR)&0x0c))
	{
		handle(-23);
		do_speed(-23,60);
	}
	speed(0,0);
	handle(0);
	
	for(degree=1;degree<=23;degree++)
	{
		handle(degree);
		do_speed(degree,45);
	}
	while(sensor_inp(MASK3_3)!=0x20 && sensor_inp(MASK3_3)!=0x60 && sensor_inp(MASK3_3)!=0xc0)
	{
		handle(15);
		do_speed(15,60);	
	}
	speed(0,0);
	handle(0);
	for(speedLevel=40;speedLevel<90;speedLevel+=10)
	{
		switch(sensor_inp(MASK3_3))
		{
			case 0x00:
					handle(0);
					speed(speedLevel,speedLevel);
					break;
			case 0x04:
					handle(5);
					do_speed(5,speedLevel);
					break;
			case 0x06:
					handle(10);
					do_speed(-10,speedLevel);
					break;
			case 0x07:
					handle(15);
					do_speed(15,speedLevel);
			case 0x03:
					handle(20);
					do_speed(20,speedLevel);
					break;
					
			//left
			case 0x20:
					handle(-5);
					do_speed(-5,speedLevel);
					break;
			case 0x60:
					handle(-10);
					do_speed(-10,speedLevel);
					break;
			case 0xe0:
					handle(-15);
					do_speed(-15,speedLevel);
					break;
			case 0xc0:
					handle(-20);
					do_speed(-20,speedLevel);
					break;				
		}	
	}
	pattern = 10;
}


void right_blackArea()
{
	handle(0);
	getData(bit_change(P7DR),80); //debug80
	speedLevel = 70;
	while(sensor_inp(MASK4_4))
	{
		if(check_crossline())		//是直线，错误检测为盲区
		{
			pattern = 30;
			getData(bit_change(P7DR),85); //debug85
			return;
		}
		
		switch(sensor_inp(MASK3_3))
		{
			case 0x00:
                handle( 0 );
                speed( speedLevel ,speedLevel );
                break;
            case 0x04:
				handle(4);
				do_speed(4,speedLevel);
				break;
            case 0x06:
				handle(8);
				do_speed(8,speedLevel);
				break;
			case 0x07:
				handle(12);
				do_speed(12,speedLevel);
				break;
            case 0x03:
                handle( 20 );
                do_speed(20,speedLevel);
                break;
            case 0x20:
				handle(-4);
				do_speed(-4,speedLevel);
				break;
            case 0x60:
				handle(-8);
				do_speed(-8,speedLevel);
				break;
			case 0xe0:
				handle(-12);
				do_speed(-12,speedLevel);
				break;
            case 0xc0:
                handle( -20 );
                do_speed(-20,speedLevel);
                break;
			default:
				do_speed(speedLevel,speedLevel);
		}
	}
	
	speed(40,0);
	degree=1;
	while(degree<=23)	//转一个比较大的弯
	{
		handle(degree);
		degree++;
	}
	while(degree>17)  //修正转的大弯为较小的弯道
	{
		handle(degree);
		degree--;
	}
	handle(17);
	while(!(bit_change(P7DR)&0x30)) //轨道交接后开始
	{
		handle(17);
		do_speed(17,60);
	}
	speed(0,0);	//将车身弄直，并速度清零准备转负角度
	handle(0);
	
	for(degree=-1;degree>=-17;degree--) //转负角度
	{
		handle(degree);
		do_speed(degree,45);
	}
	while(sensor_inp(MASK3_3)!=0x04 && sensor_inp(MASK3_3)!=0x06 && sensor_inp(MASK3_3)!=0x07)
	{		//负角度结束判断
		handle(-15);
		do_speed(-15,60);	
	}
	speed(0,0);
	handle(0);
	for(speedLevel=40;speedLevel<90;speedLevel+=5) //出盲区并速度提速
	{
		switch(sensor_inp(MASK3_3))
		{
			case 0x00:
					handle(0);
					speed(speedLevel,speedLevel);
					break;
			case 0x04:
					handle(5);
					do_speed(5,speedLevel);
					break;
			case 0x06:
					handle(10);
					do_speed(-10,speedLevel);
					break;
			case 0x07:
					handle(15);
					do_speed(15,speedLevel);
			case 0x03:
					handle(20);
					do_speed(20,speedLevel);
					break;
					
			//left
			case 0x20:
					handle(-5);
					do_speed(-5,speedLevel);
					break;
			case 0x60:
					handle(-10);
					do_speed(-10,speedLevel);
					break;
			case 0xe0:
					handle(-15);
					do_speed(-15,speedLevel);
					break;
			case 0xc0:
					handle(-20);
					do_speed(-20,speedLevel);
					break;				
		}	
	}
	pattern = 10;
	
}

int get_inner_speed(int degree,int speed_outer)
{
	speedLevel = speed_outer;
	
	if(degree>50)
		degree = 50;
	else if(degree<-50)
		degree = -50;
	else if(degree<0)
		degree = -1*degree;

	if(speed_outer>100)
		speed_outer = 100;
	
	return (int)speed_outer*((2*length-tan[degree]*width)/(2*length+tan[degree]*width));	
	
}

int get_speedLevel(int curSt)
{
	switch(curSt)
	{
		case st0:	return speedLevel_st0;
		case st1:
		case st_1:	return speedLevel_st1;
		case st2:
		case st_2:	return speedLevel_st2;
		case st3:
		case st_3:	return speedLevel_st3;
		case st4:
		case st_4:	return speedLevel_st4;
		case st5:
		case st_5:	return speedLevel_st5;

		default: return 40;//error 
	}
}

int get_midDegree(int curSt)
{
	switch(curSt)
	{
		case st0:	return midDegree_st0;

		case st1:	return midDegree_st1;
		case st_1:	return -1*midDegree_st1;

		case st2:	return midDegree_st2;
		case st_2:	return -1*midDegree_st2;

		case st3:	return midDegree_st3;
		case st_3:	return -1*midDegree_st3;

		case st4:	return midDegree_st4;
		case st_4:	return -1*midDegree_st4;

		case st5:	return midDegree_st5;
		case st_5:	return -1*midDegree_st5;

		default:return 20;
	}
}

void do_speed(int degree,int speed_outer)
{
	if(degree>0)
		speed(speed_outer,get_inner_speed(degree,speed_outer));
	else
		speed(get_inner_speed(degree,speed_outer),speed_outer);

}

int get_near_std(int cur_degree)
{
	if(cur_degree>=0)
	{
		if(cur_degree>std4)
			return std4;
		else if(cur_degree>std3)
			return std3;
		else if(cur_degree>std2)
			return std2;
		else if(cur_degree>std1)
			return std1;
		else return std0;				
	}else
	{
		if(cur_degree<std_4)
			return std_4;
		else if(cur_degree<std_3)
			return std_3;
		else if(cur_degree<std_2)
			return std_2;
		else if(cur_degree<std_1)
			return std_1;
		else return std0;	
	}	
}
void start()
{
	int i;
	isStart = 1;
	cnt0 = 0;
	
	cnt1 = 70;
	while(cnt1<=650)
	{
		//580ms
		if(cnt1<=100)
			speedLevel = (int)3*cnt1/10;
		else if(cnt1<=200)
			speedLevel = (int)(cnt1-100)/4+30;
		else if(cnt1<=325)
			speedLevel = (int)4*(cnt1-200)/25+55;
		else if(cnt1<=475)
			speedLevel = (int)(cnt1-325)/10+75;
		else speedLevel  = (int)2*(cnt1-475)/35+90;
		
		switch(sensor_inp(MASK3_3))
		{
			case 0x00:
					handle(0);
					speed(speedLevel,speedLevel);
					break;
			case 0x04:
					handle(5);
					do_speed(5,speedLevel);
					break;
			case 0x06:
					handle(10);
					do_speed(-10,speedLevel);
					break;
			case 0x07:
					handle(15);
					do_speed(15,speedLevel);
			case 0x03:
					handle(20);
					do_speed(20,speedLevel);
					break;
					
			//left
			case 0x20:
					handle(-5);
					do_speed(-5,speedLevel);
					break;
			case 0x60:
					handle(-10);
					do_speed(-10,speedLevel);
					break;
			case 0xe0:
					handle(-15);
					do_speed(-15,speedLevel);
					break;
			case 0xc0:
					handle(-20);
					do_speed(-20,speedLevel);
					break;				
		}
	}
	pattern = 10;
	getData(P7DR,5); //debug5
	pushSw = 1;	
}


char * getInfo(int id,int dir)
{
	switch(id)
	{
		case 5: return "\n****************   start car over!     ****************\n\n";
		/**************弯道case*******************/
		case 21:
			if(dir<0) return "\n\n*******************************************************\n****************      left-turn        ****************\n*******************************************************\n\n[left-turn]: send out turn left -27 instruction";
			else return "\n\n*******************************************************\n****************      right-turn        ***************\n*******************************************************\n\n[right-turn]: send out turn left -27 instruction";
		case 22:
			if(dir<0) return "[left-turn]: send out turn left -27 instruction over \n\n[left-turn]: enter do-while  to regulate";
			else return "[right-turn]: send out turn right 27 instruction over \n\n[right-turn]: enter do-while  to regulate";
		case 23:
			if(dir<0) return "[left-turn]: left turn regulate over,leave function\n\n*******************************************************\n*****************    straight-line    *****************\n*******************************************************\n\n";
			else return "[right-turn]: right turn regulate over,leave function\n\n*******************************************************\n*****************    straight-line    *****************\n*******************************************************\n\n";

		
		/*****************直角case***************************/
		case 31:
			return "\n\n*******************************************************\n******************      Angle        ******************\n*******************************************************\n\n[Angle]: run 100ms(deaf)";
		case 32:
			return "[Angle]: run 100ms(deaf) over \n\n[Angle]: regulate 300ms";
		case 33:
			return "[Angle]: regulate 300ms over \n\n[Angle]: waite for swagging state";
		case 34:
			if(dir<0) return "[left-angle]: detect left angle,send out speed and handle instruction \n\n[left-angle]: ignore 50ms to wait for all black";
			else return "[right-angle]: detect right angle,send out speed and handle instruction \n\n[right-angle]: ignore 50ms to wait for all black";
		case 35:
			if(dir<0) return "[left-angle]: ignore 50ms and all black come \n\n[left-angle]: wait for all black over";
			else return "[right-angle]: ignore 50ms and all black come \n\n[right-angle]: wait for all black over";
		case 36:
			if(dir<0) return "[left-angle]: all black over \n\n[left-angle]: swagging process,handle and speed adjust,wait for swagging over state";
			else return "[right-angle]: all black over \n\n[right-angle]: swagging process,handle and speed adjust,wait for swagging over state";
		case 37:
			if(dir<0) return "[left-angle]: swagging process over(state come) \n\n[left-angle]: regulate car body";
			else return "[right-angle]: swagging process over(state come) \n\n[right-angle]: regulate car body";
		case 38:
			if(dir<0) return "[left-angle]: regulate car body over,\n\n[left-angle]:increase speed to leave function\n\n*******************************************************\n*****************    straight-line    *****************\n*******************************************************\n\n";
			else return "[right-angle]: regulate car body over,\n\n[right-angle]:increase speed to leave function\n\n*******************************************************\n*****************    straight-line    *****************\n*******************************************************\n\n";
		case 39:
			if(dir<0) return "[left-angle]:leave function\n";
			else return "[right-angle]:leave function\n";
		case 80:
			if(dir<0)	return "[left-storage]: find end line\n\n";
			else return "[right-storage]: find end line\n\n";
		/********************盲区***************************/
		case 41:
			if(dir<0) return "\n\n*******************************************************\n****************    left-blackArea     ****************\n*******************************************************\n\n[left-blackArea]:cross two while line";
			else return "\n\n*******************************************************\n****************   right-blackArea     ***************\n*******************************************************\n\n[right-blackArea]:cross two while line";
		case 42:
			if(dir<0) return "[left-blackArea]: two white line over\n\n[left-blackArea]: speed-down till all black";
			else return "[right-blackArea]: two white line over\n\n[right-blackArea]: speed-down till all black";	
		case 43:
			if(dir<0) return "[left-blackArea]: all-black come \n\n[left-blackArea]: cross till left-sensor touch left road";
			else return "[right-blackArea]: all-black come \n\n[right-blackArea]: cross till right-sensor touch right road";		
		case 44:
			if(dir<0) return "[left-blackArea]: left-sensor have touched left road \n\n[left-blackArea]: wait for first regulate state come";
			else return "[right-blackArea]: right-sensor have touched right road \n\n[right-blackArea]: wait for first regulate state come";		
		case 45:
			if(dir<0)return "[left-blackArea]: first regulate state come\n\n[left-blackArea]: begin first regualte";
			else return "[right-blackArea]: first regulate state come\n\n[right-blackArea]: begin first regualte";		
		case 46:
			if(dir<0) return "[left-blackArea]: first regulate over\n\n[left-blackArea]:steady and speed-up";
			else return "[right-blackArea]: first regulate over\n\n[right-blackArea]:steady and speed-up";		
		case 47:
			if(dir<0) return "[left-blackArea]: leave function\n\n*******************************************************\n*****************    straight-line    *****************\n*******************************************************\n\n";
			else return "[right-blackArea]: leave function\n\n*******************************************************\n*****************    straight-line    *****************\n*******************************************************\n\n";

			
		case 50: return "\n\ndetect end line\n\n";
		case 51: return "\n\nstart timer for  right turn\n\n";
		case 52: return "\n\ntime of turning right come,next turn 26\n\n";
		case 53: return "\n\nturn 26 finish,wait for increase degree and speed\n\n";
		case 54: return "\n\nincrease instruct send out,wait for turn right finish\n\n";
		case 55: return "\n\nturn right finish,next left turn\n\n";
		case 56: return "\n\nturn left -40 and delay 500ms finish,wait for end state\n\n";
		case 57: return "\n\nleft turn end state come,next regulate car body\n\n";
		case 58: return "\n\nregulate car body finish\n\n";
		case 59: return "finish ,wait for get data.\n\n";
	}
	return 0;
}

void stop()
{
	speed(0,0);
	while(1);
}
short int getDegree_(int degree)
{
	if(degree>=50)
		return -1*(degree-50);
	else return degree;
}
int getSpeed_(int speed)
{
	if(speed>=150)
		return -1*(speed-150);
	else return speed;
}
void getLightLed(unsigned char led)
{
	int i;	
	for(i=0;i<8;i++)
	{
		if(led&0x80)printf("%d",1);
		else printf(".");
		led <<=1;
	}
}

int getLightLedSum()
{
	int i;
	int sum = 0;
	unsigned char led = bit_change(P7DR);
	for(i=0;i<8;i++)
	{
		if(led&0x80)sum++;
		led <<=1;
	}
	return sum;
}

/************************************************************************/
/* end of file                                                          */
/************************************************************************/