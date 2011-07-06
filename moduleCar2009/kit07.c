//*************************************************************************
//*************************************************************************
//**<程序名>：瑞萨模型车          									     **
//**<版本说明>：第1版，整体设计(参考：瑞萨官方，HT代码)                  **
//**<运行环境>：瑞萨H8-3048                                              **
//**<作者>：tianweidut											     	 **
//**<完成时间>：2009年10月19日										     **
//**<联系方式>：E-mail:liutianweidlut@gmail.com;QQ:416774905。		     **
//*************************************************************************
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

/*======================================*/
/* Include                              */
/*======================================*/
#include "includes.h"

/*======================================*/
/* var declarations               */
/*======================================*/

#define enSpeedMax	287	//最大速度控制：可以改成按键选项的方式

/*======================================*/
/* Prototype declarations               */
/*======================================*/
void myInit();

void main(void)
{
	vInitialize();
	myInit();
	
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
							case LEFT:
									handle(-1*midDegree_st4);
									left_turn();
									break;
							case RIGHT:
									handle(midDegree_st4);
									right_turn();
									break;
							default:break;
						}
						break;


			case 30:	//直角

						rightAngle();						
						break;


			case 40:	//盲区
						switch(blackArea_direction)
						{
							case LEFT:
									left_blackArea();
									break;
							case RIGHT:
									right_blackArea();
									break;
							default:break;									
						}
						break;

			default:break;

		}//end switch

	}//end while
	
}
 
void myInit(void)
{
	if((~P6DR)&0x08) isDebug = 1;	
	else isDebug = 0;
 	
	lastDegree = 0;
	pushSw = 0;
	turnSpeed = 95;
//	turnSpeed = 100 - 2*(dipsw_get()&0x07);
}
#pragma interrupt( interrupt_timer0 )
void interrupt_timer0( void )
{
    ITU0_TSR &= 0xfe;                   /* Flag clear                   */
   
    cnt0++;
    cnt1++;
	cnt2++;
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


/*
	if(!isDebug)
	{
		if(1==angleFlag)
		{
			if((int)eCurSpeed>150)
			{
				ITU4_BRA=(unsigned long)(PWM_CYCLE-1)*(-10)/100;
				ITU3_BRB=(unsigned long)(PWM_CYCLE-1)*(-10)/100;
				
				//brake time
				for(speedBrake_var=200;speedBrake_var!=0;speedBrake_var--);
			}
			else if((int)eCurSpeed<145)
			{
				ITU4_BRA=(unsigned long)(PWM_CYCLE-1)*40/100;
				ITU3_BRB=(unsigned long)(PWM_CYCLE-1)*40/100;
				for(speedBrake_var=200;speedBrake_var!=0;speedBrake_var--);
			}
		}
	}

	
*/	
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

#pragma interrupt( interrupt_capture )
void interrupt_capture( void )////////////////////////////////////////////
{
	ISR &=	0xfe;		//清楚标志寄存器				
	iPulesNumAll++;		//脉冲计数:
	//此处需要考虑数目越界问题
		//最高速限制
	if((int)eCurSpeed>enSpeedMax)
	{
 
		ITU4_BRA=(unsigned long)(PWM_CYCLE-1)*(3)/100;
		ITU3_BRB=(unsigned long)(PWM_CYCLE-1)*(3)/100;
		for(speedBrake_var=100;speedBrake_var!=0;speedBrake_var--);
		
	}
}