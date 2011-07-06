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
//*        **********************车辆自动控制程序*************************        *
//*	文件说明：赛道控制（核心代码）																							                        *
//*************************************************************************
#ifndef __CONTROL_H__
#define __CONTROL_H__

//通过等级获取速度，角度情况：此方法可以应用到光电编码测速的速度调整上
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
#define std1	5
#define std2 	10
#define std3 	15
#define std4 	27
#define std_1	-5
#define std_2 	-10
#define std_3 	-15
#define std_4 	-27

extern int angleFlag;	//进入直角标志
//extern int choiceSpeedMax;
//int getChoiceSpeedMax(void);	//以后添加：通过外加button进行最高速限制

int get_speedLevel(int curSt);
int get_midDegree(int curSt);
int get_near_std(int cur_degree);

void straight_run();
void right_turn();
void left_turn();
void rightAngle();
 
void right_blackArea();
void left_blackArea();

void speedAdjust(int acc_left,int acc_right);	//直接从PWM定时器上进行速度变化

#define sufNum	1
extern int suf[sufNum] ;
extern int time[sufNum];
extern int cntflag;
extern int cntflagTime;
#endif