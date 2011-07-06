/*********************************************************************
* File：	test-key.c
* Author：	Embest
* Desc：
* History：
*			Original version, 2005.4 Embest
*			Program modify, 2005.06.05 Embest R.X.Huang <hungrx@embedinfo.com>
*********************************************************************/
/*********************************************************************
* File：	test-key.c
* Author：	sigmax6
* Desc：    继承自：Original version, 2005.4 Embest
* History：	嵌入式操作系统上机作业 用线程实现电梯功能
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/fcntl.h>
#include "test-key.h"


/* control code */
#define I2C_SET_DATA_ADDR	0x0601
#define I2C_SET_BUS_CLOCK	0x0602


/*********************************************************************************************
* name:		key_set
* func:		keyboard setting
* para:		none
* ret:		none
* modify:
* comment: 将试验板上的按键信号转换为真实表示的数值
*********************************************************************************************/
char key_set(char ucChar)
{
	switch(ucChar)
	{
		case 1:                  //0
		case 2:                  //1
		case 3:                  //2
		case 4:                  //3
		case 5: //4
				ucChar-=1; break;
		case 9: //5
		case 10://6
		case 11://7
		case 12://8
		case 13://9
				ucChar-=4; break;
		case 17://10
		case 18://11
		case 19://12
		case 20://13
		case 21://14
				ucChar-=7; break;
		case 25: ucChar = 0xF; break;   //15
		case 26: ucChar = '+'; break;   //+
		case 27: ucChar = '-'; break;  //-
		case 28: ucChar = '*'; break;  //×
		case 29: ucChar = 0xFF; break;  //FF
		default: ucChar = 0xFE; // error
	}
	return ucChar;
}

int main(int argc, char** argv)
{
	int i,cx, cy, fd;
	char key;
	static char *driver = "/dev/i2c/0";              //按键设备在linux中的设备位置

	printf(" Welcome to the elevator.\n");
	printf(" Press to select the floor [1-16]\n");
	/* open device */
	fd = open(driver, O_RDWR);                       //打开设备，设置为读写操作
	if(fd < 0)
	{
		printf("elevator device open fail\n");       //无法打开设备，则报错
		return -1;
	}
	/* set zlg7290 slave address */
	ioctl(fd, I2C_SLAVE_FORCE, ZLG_SLAVE_ADDR);
   /*int ioctl(int fd, int cmd, …)；cmd:命令 用于指定从设备其中fd就是用户程序打开设备时使用op    en函数返回的文件标示符，cmd就是用户程序对设备的控制命令*/
	/* set i2c bus clock 250KHz */
	ioctl(fd, I2C_SET_BUS_CLOCK, 250*1000);          //原理同上

	/* display all segment */
	key = 0xff;
	for(i=0; i<8; i++)
	{
		/* zlg7290 inner register access address */
		ioctl(fd, I2C_SET_DATA_ADDR, REG_Dis7-i);
		write(fd, &key, 1);
	}
	sleep(2);

	for(;;)
	{
		/* zlg7290 inner register access address */
		ioctl(fd, I2C_SET_DATA_ADDR, REG_Sys);
		read(fd, &key, 1);

		/* check key pressed */
		if(key & 0x01)// if REG_Sys set as 1, a valid key pressed
		{
			/* zlg7290 inner register access address */
			ioctl(fd, I2C_SET_DATA_ADDR, REG_Key);
			/* read key value */
			read(fd, &key, 1);
			if(key > 0)
			{
				/* adjust base zero */
				cx = ((key-1) % 8) + 1; cy = ((key-1) / 8) + 1;
				key = key_set(key);
				if(key != 0xFE)// key map for EduKitII
				{
					if(key<16)
					{
						if(key<10)	key += 0x30;
						else 		key += 0x37;
					}
					if(key < 255)
						printf("(row %d, col %d) Key: %c\n", cy, cx,key);

					if(key == 0xFF) printf("(row %d, col %d) Key: FUN\n", cy, cx);
				}
				if(cy == 4 && cx == 5)// 'FUN' to exit..
					break;
			}
		}
	}
	printf(" end.\n");
	close(fd);

	return 0;
}
