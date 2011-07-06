//*************************************************************************
//*																								                        *
//*        **********************调试打印函数程序*************************        *
//*	文件说明：调试，数组存数																							                        *
//*************************************************************************

#ifndef __DEBUG_H__
#define __DEBUG_H__

//打印调试
/*
#ifdef	DEBUG_SWITCH	// if want to debug, define it while compile this function
#define DEBUG(str, args...)	printf(" " str, ## args)
#else
#define DEBUG(str, args...) ( )
#endif
*/

//调试数组及开关变量
extern unsigned char curLedValue;
extern unsigned char lastLedValue;

extern int isDebug;
extern int curSuffix;
extern int lastSuffix;
extern int printNow;

extern int pushSw;	//调试用:取数据中断，打印数据开关
extern int isStop;
extern int isStart;


void getData(unsigned char ledValue,int debug); 
void print();//该函数中不能调用handle(),因为只有handle()可能调用此函数
char * getInfo(int id,int dir);	//信息打印
void getLightLed(unsigned char led);	//打印灯的信息
short int getDegree_(int degree);
int getSpeed_(int speed);

#endif