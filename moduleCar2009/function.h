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
//*        **********************功能函数程序*************************        *
//*	文件说明：功能性质函数																							                        *
//*************************************************************************
#ifndef __FUNCTION_H__
#define __FUNCTION_H__



extern unsigned long cnt0;
extern unsigned long cnt1;
extern unsigned long cnt2;

extern int degree;
extern int lastDegree;
extern int speedLevel;
 
extern int blackArea_direction;	
extern int turn_direction;

extern int turnSpeed;		//弯道时速度控制变量

extern int pattern;
extern int curSt;
extern int lastSt;



#define LEFT	-1	//方向定义：1：左，-1：右
#define RIGHT   1

//车辆几种状态
#define Par_Straight	10
#define Par_Turn		20
#define Par_RightAngle	30
#define Par_BlackArea	40
 

#define RESET	0	//时间，速度，角度等等


void timer(unsigned long timer_set);	//定时，延迟

unsigned  char   sensor_inp(unsigned char mask);	//传感器
unsigned  char   bit_change(unsigned char in);		

unsigned char startbar_get(void);

unsigned char dipsw_get(void);	//按键，button
//#define dipsw_get()	((unsigned char)((~P6DR)&0x0f))
//unsigned char pushsw_get(void);
#define pushsw_get()	((unsigned char)((~PBDR)&0x01))
//unsigned led_out(unsigned char led);	//led
#define led_out(led)	PBDR =  (PBDR&0x3f)|((~led)<<6)

 int check_crossline(void);	//检测特殊赛道：此处改成宏定义
 int  check_blackArea(void);
//#define cur_sensor	((~P7DR)&0xef)	//忽略startbar状态
//#define check_crossline()	((cur_sensor&0xc0)&&(cur_sensor&0x2c)&&(cur_sensor&0x03))?TRUE:FALSE


//速度角度控制，输出
void speed(int accele_l , int accele_r);
void handle(int angle);
void do_speed(int degree ,int speed_outer);	//内外圈匹配速度
int get_inner_speed(int degree,int speed_outer);

void start(void);	//赛车启动
void stop(void);

//车辆自动控制使用
int getLightLedSum();	//灯亮数目，双排线


#endif