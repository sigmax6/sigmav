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
#include "includes.h"

//print

int angleFlag=0;
int suf[sufNum]={0};
int time[sufNum]={0};
int cntflag=0;
int cntflagTime=0;

//通过等级获取速度，角度情况：此方法可以应用到光电编码测速的速度调整上
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

 	//弯道，直角使用
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

void straight_run()
{
	//直线代码
	switch(sensor_inp(MASK3_3))
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
	handle(0);
	speed(-45,-45); //before speed(60,60);
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
	speed(-47,-47);
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
	speedLevel = 48;
	cnt1 = 0;
	while(waitForAngle)
	{
		if(cnt1>100) speedLevel = 57;
		else if(cnt1>50) speedLevel = 56;
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
					handle(45);
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



//直角
void rightAngle3()
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


	//step1:过双白线
	cnt1 = 0;
	var = 0;
	while(cnt1<50)
	{
		if(var==0 && getLightLedSum()<=2)var++;
		if(var==1 && getLightLedSum()>2)var++;
		if(var==2 && getLightLedSum()<=2)var++;
		if(var==3)break;
	}

	cnt1 =0;
	while(cnt1<150)
	{

		if(eCurSpeed>150)
		{
			speed(-25,-25);		//数据调整
		}

		suf[cntflag++]=(int)eCurSpeed;
		if(eCurSpeed<145)
		{
			speed(40,40);	//数据调整
		}
		suf[cntflag++]=(int)eCurSpeed;

		if((eCurSpeed-150<3)&&(eCurSpeed-150>-3))	//此时速度已经达到了标准速度
		{
			cnt1=1000;
			//此处应该根据转化关系，进行速度转化
		}

		//角度调整
		switch(sensor_inp(MASK3_3))
		{
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
				cnt1= 1000;	//出来了
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
			default:break;
		}
		handle(degree);
	}


	speed(50,50);
	for(var=0;var<200;var++);


	//step2:等待直角到来(175ms)
	speedLevel = 55;
	waitForAngle = 1;
	cnt1 = 0;
	while(waitForAngle)
	{
		if(cnt1>100) speedLevel = 53;
		else if(cnt1>50) speedLevel = 51;
		switch( sensor_inp(MASK3_3) ) {
				case 0xa0:
	            case 0xe0:
				case 0xe4:
				case 0xe6:
					speed(0,0);
					for(degree=-20;degree>=-44;degree-=1) handle(degree);  //before -40
					AngleDirection = -1;
					waitForAngle = 0;
					//step4:忽略直角白线
					getData(P7DR,-34);				//debug-34
	                break;
				case 0x05:
	            case 0x07:
				case 0x27:
					speed(0,0);
					for(degree=20;degree<=44;degree+=1)	handle( degree );   //before 4
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
				speedLevel = 55;
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
							case 0x00: if(curSt!=0){ curSt = 0; if(curSt>=max) max = curSt;else{ reachMax = 1;cnt1 = 0;} }speed(0, 75);break;
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
				speedLevel = 55;
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
void left_blackArea()
{
	//step0:过双白线
	int var ;
	var = 0;
	handle(0);
	cnt1=0;
	while(cnt1<100)
	{
        if(check_crossline()){    pattern = 30;return;  }
        if(var==0 && getLightLedSum()<=2)var++;
        if(var==1 && getLightLedSum()>2)var++;
        if(var==2 && getLightLedSum()<=2)var++;
        if(var==3)break;
	}
	getData((~P7DR),-47);                //debug-47

	speedLevel = 70;
	//step1:检测，稳定区
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
				break;
		}
	}

	//step2:盲区动作
	speed(0,0);
	handle(-22);
	speed(0,40);
	while(!(bit_change(P7DR)&0xf0))
	{
		handle(-22);
		do_speed(-22,70);
	}

	do_speed(25,80);
	handle(25);
	timer(15);

	while((sensor_inp(MASK3_3)!=0x20) && (sensor_inp(MASK3_3)!=0x60 )&& (sensor_inp(MASK3_3)!=0xc0))
	{
		handle(15);
		do_speed(15,60);
	}
	handle(0);

	//step3:出盲区调整过程
	for(speedLevel=60;speedLevel<90;speedLevel+=5)
	{
		if(check_crossline()){	pattern = 30;return;	}
		if(check_blackArea()){	pattern = 40;return;	}

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

	if(check_crossline()){	pattern = 30;return;	}
	if(check_blackArea()){	pattern = 40;return;	}
	pattern = 10;

}


void right_blackArea()
{
	int var;
	var = 0;
	handle(0);
	cnt1=0;
	while(cnt1<100)
	{
        if(check_crossline()){    pattern = 30;return;  }
        if(var==0 && getLightLedSum()<=2)var++;
        if(var==1 && getLightLedSum()>2)var++;
        if(var==2 && getLightLedSum()<=2)var++;
        if(var==3)break;
	}
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

	speed(0,0);
	handle(22);
	speed(40,0);
	while(!(bit_change(P7DR)&0xf0))
	{
		handle(22);
		do_speed(22,70);
	}
	handle(-25);
	do_speed(-25,80);
	timer(40);
	while(sensor_inp(MASK3_3)!=0x04 && sensor_inp(MASK3_3)!=0x06 && sensor_inp(MASK3_3)!=0x07)
	{		//负角度结束判断
		handle(-15);
		do_speed(-15,60);
	}
	handle(0);
	for(speedLevel=60;speedLevel<90;speedLevel+=5) //出盲区并速度提速
	{
		if(check_crossline()){	pattern = 30;return;}
		if(check_blackArea()){	pattern = 40;return;}

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

	if(check_crossline()){	pattern = 30;return;}
	if(check_blackArea()){	pattern = 40;return;}
	pattern = 10;



}

/*
void left_blackArea1()
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

	speed(0,40);
	degree=-1;
	while(degree>=-23)
	{
		handle(degree);
		degree--;
	}
	while(degree<-17)
	{
		handle(degree);
		degree++;
	}
	handle(-17);
	while(!(bit_change(P7DR)&0x0c))
	{
		handle(-17);
		do_speed(-17,60);
	}
	speed(0,0);
	handle(0);

	for(degree=1;degree<=17;degree++)
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
	for(speedLevel=40;speedLevel<90;speedLevel+=5)
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
*/

/*void right_blackArea1()
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

*/
/*修改前的代码
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

	speed(0,40);   //40

	handle(-17);
	while(!(bit_change(P7DR)&0x0c))
	{
		handle(-20);
		do_speed(-20,60);
	}
	speed(0,0);
	handle(0);


	while(sensor_inp(MASK3_3)!=0x20 && sensor_inp(MASK3_3)!=0x60 && sensor_inp(MASK3_3)!=0xc0)
	{
		handle(15);
		do_speed(15,60);
	}
	speed(0,0);
	handle(0);
	for(speedLevel=40;speedLevel<90;speedLevel+=5)
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

	handle(17);
	while(!(bit_change(P7DR)&0x30)) //轨道交接后开始
	{
		handle(20);
		do_speed(20,60);
	}
	speed(0,0);	//将车身弄直，并速度清零准备转负角度
	handle(0);


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

}*/



void speedAdjust(int acc_left,int acc_right)	//直接从PWM定时器上进行速度变化
{
		ITU4_BRA=ITU4_BRA+(unsigned long)(PWM_CYCLE-1)*(acc_left)/100;	//left
		ITU3_BRB=ITU3_BRB+(unsigned long)(PWM_CYCLE-1)*(acc_right)/100;	//right
}

