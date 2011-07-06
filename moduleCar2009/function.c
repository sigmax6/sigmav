//*************************************************************************
//*************************************************************************
//*************************************************************************
//**<程序名>：瑞萨模型车          						 	             **
//**<版本说明>：第2版，第一版修改                                        **
//**<运行环境>：瑞萨H8-3048                                              **
//**<作者>：MCU5								    	            	 **
//**<开始修改时间>：2010年3月								             **
//*************************************************************************
//*************************************************************************
																						                        *
//*    **********************功能函数程序*************************        *
//*	文件说明：功能性质函数																							                        *
//*************************************************************************
#include "includes.h"

unsigned long cnt0 =0;
unsigned long cnt1 =0;
unsigned long cnt2 =0;

int degree = 0;
int lastDegree = 0;
 
int speedLevel =0;
int blackArea_direction =0;	 
int turn_direction = 0;

int pattern ;	//直线
int curSt =0;
int lastSt =0;
int turnSpeed=0;

//内外轮匹配计算
#define length	16	
#define width	16

const double tan[46] ={ 0, 0.017455,0.034921,0.052408,0.069927,0.087489,
						   0.105104,0.122785,0.140541,0.158384,0.176327,
						   0.194380,0.212557,0.230868,0.249328,0.267949,
						   0.286745,0.305731,0.324920,0.344328,0.363970,
						   0.383864,0.404026,0.424475,0.445229,0.466308,
						   0.487733,0.509525,0.531709,0.554309,0.577350,
						   0.600861,0.624869,0.649408,0.674509,0.700208,
						   0.726543,0.753554,0.781286,0.809784,0.839100,
						   0.869287,0.900404,0.932515,0.965689,1.000000};


//#define get_inner_speed(degree_def,speed_outdef) ((int)(speed_outdef*((2*length-tan[degree_def]*width)/(2*length+tan[degree_def]*width))))
												  //此处宏定义存在范围问题(degree:0-45)
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

void timer(unsigned long timer_set)	//定时，延迟
{
	cnt0 = RESET;
	while(cnt0<timer_set);	
}

unsigned char startbar_get(void)
{
	unsigned char b;
	
	b = ~P7DR;
	b &= 0x10;
	b >>=4;

	return b;
}


unsigned char bit_change(unsigned char in)
{
	unsigned sensor;
	sensor  = ~in;
	sensor  &=0xef;

	if(sensor &0x08)sensor|=0x10;

	return sensor;
}
unsigned char sensor_inp( unsigned char mask )
{
    unsigned char sensor;
 
	sensor = ~P7DR;
	sensor &= 0xef;
	if( sensor & 0x08 ) sensor |= 0x10;
	
	sensor &=mask;
	
    return sensor;
}

int check_blackArea(void)	//优化
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

 
 
int check_crossline( void )
{
    unsigned char b;
    int ret=0;
 
	b = bit_change(P7DR);
	if((b&0xc0) && (b&0x3c) && (b&0x03))
		ret = 1;
    return ret;
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


void stop(void)
{
	speed(0,0);
	handle(0);
	printf("stop\n");
	while(1);
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
//速度函数
void speed( int accele_l, int accele_r )
{
	unsigned char sw_data;
	unsigned long speed_max;
    sw_data= dipsw_get()+5;
	speed_max=(unsigned long)(PWM_CYCLE-1) * sw_data / 20;
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
void do_speed(int degree,int speed_outer)	//内外圈匹配速度
{
	if(degree>0)
		speed(speed_outer,get_inner_speed(degree,speed_outer));
	else
		speed(get_inner_speed(degree,speed_outer),speed_outer);

}

//车辆自动控制使用
int getLightLedSum()	//灯亮数目，双排线:是否可以替换
{
	int i,sum=0;
	unsigned led = bit_change(P7DR);
	for(i=0;i<8;i++)
	{
		if(led&0x80)sum++;
		led<<=1;
	}
	return sum;
}
 
unsigned char dipsw_get( void )
{
    unsigned char sw;

    sw  = ~P6DR;                        /* Reading of DIP switch */
    sw &= 0x0f;

    return  sw;
}




