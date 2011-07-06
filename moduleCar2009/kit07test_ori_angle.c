//?¡À??
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
	
	//step1:1y??¡ã¡Á??(31ms 34ms 34ms)
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

	//step2:???¨´¦Ì¡Â?¨²(¨®?1y??¡ã¡Á??¦Ì?¨º¡À??¨®D1?)
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

	//step3:¦Ì¨¨¡äy?¡À??¦Ì?¨¤¡ä(175ms)
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
					//step4:o????¡À??¡ã¡Á??
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
					//step4:o????¡À??¡ã¡Á??
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
		 	if(!sensor_inp(MASK4_4)) //¨º??¡è???¨®?D?a?¡À??
			{
				pattern = 40;
				return;
			}
	}	
	///////encode ///////////////
	
	crankFlag=0;
	//¡Áa?¡À??1y3¨¬¡ä|¨¤¨ª(¡Á¨®¨®¨°)
	switch(AngleDirection)
	{
		case -1:
				cnt1 = 0;
				while(cnt1<50)
				{
					if(!bit_change(P7DR) ) break;//o???¡ã¡Á??(16ms)
				}
				var = cnt1;
				getData(P7DR,-35); 				//debug-35(?-1y¡ã¡Á??)
				
				//step5:?-1yo¨²??(¡ã¨²¨ª¡¤3??¨²)
				if(var<17){ degree_ = -43; speedLevel_ = 45; }
				else if(var<25){ degree_ = -42; speedLevel_ = 48; }
				else if(var<35){ degree_ = -41; speedLevel_ = 51; }
				else if(var<45){ degree_ = -40; speedLevel_ = 54; }
				else { degree_ = -40; speedLevel_ = 57; }
				
				cnt1 = 0;
				degree = degree_;
				speedLevel = speedLevel_;
				while(!bit_change(P7DR) )					//?¨´?Y¡ã¡Á??¨º¡À??var???¡§o¨²????¡Áa?¨¨¨ºy
				{
					handle(degree);
					speed(0,speedLevel);	
				}				
				getData(P7DR,-36); 				//debug-36
				
				//step6:¡ã¨²¨ª¡¤1y3¨¬				
				speedLevel = 60;
				cnt1 = 0;
				reachMax = 0;
				max = 0;
				curSt = 0;
				b = bit_change(P7DR);
				while((b&0xe7)!=0x60 && (b&0xe7)!=0x20)
				{
					if(!reachMax)				//¡Á?¡ä¨®¡Á¡ä¨¬??¡ã
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
							degree = -40;								//¡Á?¡ä¨®¡Á¡ä¨¬?o¨®  (86ms)
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
				
				//step7:?¡À????¡Áa?¨¢¨º?,¦Ì¡Â??3¦Ì¨¦¨ª3??¡À??
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
				//step8:?¨¢¨º?o¡¥¨ºy    
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
					if(!bit_change(P7DR) ) break;//o???¡ã¡Á??(16ms)
				}
				var = cnt1;
				getData(P7DR,35); 				//debug35(?-1y¡ã¡Á??)
				
				//step5:?-1yo¨²??(¡ã¨²¨ª¡¤3??¨²)
				if(var<17){ degree_ = 43; speedLevel_ = 45; }
				else if(var<25){ degree_ = 42; speedLevel_ = 48; }
				else if(var<35){ degree_ = 41; speedLevel_ = 51; }
				else if(var<45){ degree_ = 40; speedLevel_ = 54; }
				else { degree_ = 40; speedLevel_ = 57; }
				
				cnt1 = 0;
				degree = degree_;
				speedLevel = speedLevel_;
				while(!bit_change(P7DR) )					//?¨´?Y¡ã¡Á??¨º¡À??var???¡§o¨²????¡Áa?¨¨¨ºy
				{
					handle(degree);
					speed(speedLevel,0);	
				}				
				getData(P7DR,36); 				//debug36
				
				//step6:¡ã¨²¨ª¡¤1y3¨¬				
				speedLevel = 60;
				cnt1 = 0;
				reachMax = 0;
				max = 0;
				curSt = 0;
				b = bit_change(P7DR);
				while((b&0xe7)!=0x06 && (b&0xe7)!=0x04)
				{
					if(!reachMax)				//¡Á?¡ä¨®¡Á¡ä¨¬??¡ã
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
							degree = 40;								//¡Á?¡ä¨®¡Á¡ä¨¬?o¨®  (86ms)
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
				
				//step7:?¡À????¡Áa?¨¢¨º?,¦Ì¡Â??3¦Ì¨¦¨ª3??¡À??
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
