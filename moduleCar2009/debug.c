//*************************************************************************
//*************************************************************************
//*************************************************************************
//**<程序名>：瑞萨模型车          						 	             **
//**<版本说明>：第2版，第一版修改                                        **
//**<运行环境>：瑞萨H8-3048                                              **
//**<作者>：MCU5								    	            	 **
//**<开始修改时间>：2010年3月								             **
//*************************************************************************
//*************************************************************************																      *
//*        **********************调试打印函数程序**************************
//*	文件说明：调试																							                        *
//*************************************************************************
#include "includes.h"

#define arrayLength  300

unsigned char pattern_[arrayLength] = { 0 };
unsigned char ledValue_[arrayLength]= {0};
short int duration_[arrayLength] = { 0 }; // 负数时表示调试信息
unsigned char degree_[arrayLength] = {0};	//+0~49  -:50~
unsigned char leftSpeed_[arrayLength] = { 0 };
unsigned char rightSpeed_[arrayLength]={ 0 };

unsigned char curLedValue;
unsigned char lastLedValue;

int isDebug = 0;
int curSuffix = 1;
int lastSuffix = 0;
int printNow = 0;

int pushSw = 0;	//调试用:取数据中断，打印数据开关
int isStop = 0;
int isStart = 0;

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
			if(lastSuffix+1 ==curSuffix)//不等说明    上一次加入了调试信息，不用修改lastSuffix下标对于的值，该下标在上一次调试时已经统计完毕
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

	int i,j ;
	int sum;
	isStart = 0;
	isDebug = 0;
	printNow = 0;
	isStop = 0;

	cnt1 = RESET;

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
			printf("] %4d     ||     %3d (%3d,%3d)                \n",duration_[i],getDegree_(degree_[i]),getSpeed_(leftSpeed_[i]),getSpeed_(rightSpeed_[i]));

			sum = 0;
		}else
		{
			sum +=duration_[i];

			printf("%3d>:  %2d (%02X,%02X)[",i,pattern_[i],bit_change(0xe7&ledValue_[i]),bit_change(ledValue_[i]));
			getLightLed(bit_change(ledValue_[i]));
			printf("] %4d     ||     %3d (%3d,%3d)                \n",duration_[i],getDegree_(degree_[i]),getSpeed_(leftSpeed_[i]),getSpeed_(rightSpeed_[i]));
		}
	}
	printf("sum time:%d\n\n",sum);
	printf("==============================================================\n");
	for(j=0;j<cntflagTime;j++)
	{
		printf("the %d ,now print the time is %d\n",time[j]);
	}
	for(j=0;j<cntflag;j++)
	{
		printf("the %d ,now print the ecurspeed is %d\n",suf[j]);
	}
	printf("==============================================================\n");
	while(!pushsw_get()){ if(cnt1<500) led_out(0x01); else if(cnt1<1000) led_out(0x02); else cnt1 = 0; }
	stop();
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

void getLightLed(unsigned char led)	//打印灯的信息
{
	 int i;
	 for (i=0;i<8;i++ )
	 {
		 if (led&0x80)
		 {
			 printf("%d",1);
		 }
		 else
		 {
			printf(".");
		 }

		 led <<= 1;
	 }
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
