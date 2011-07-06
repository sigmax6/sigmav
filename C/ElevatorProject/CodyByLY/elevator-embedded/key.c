/*********************************************************************
* File£º	test-key.c
* Author£º	Embest
* Desc£º		
* History£º	
*			Original version, 2005.4 Embest
*			Program modify, 2005.06.05 Embest R.X.Huang <hungrx@embedinfo.com> 
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/fcntl.h>
#include "key.h"


/* control code */
#define I2C_SET_DATA_ADDR	0x0601
#define I2C_SET_BUS_CLOCK	0x0602


/*********************************************************************************************
* name:		key_set
* func:		keyboard setting
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
char key_set(char ucChar)
{
	switch(ucChar)
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
				ucChar-=1; break;
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
				ucChar-=4; break;
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
				ucChar-=7; break;
		case 25: ucChar = 0xF; break;
		case 26: ucChar = '+'; break;
		case 27: ucChar = '-'; break;
		case 28: ucChar = '*'; break;
		case 29: ucChar = 0xFF; break;
		default: ucChar = 0xFE; // error
	}
	return ucChar;
}

int get_key()
{
	int i,cx, cy, fd;
	char key;
	static char *driver = "/dev/i2c/0";

	//printf(" 5x4 Keyboard Test Example base on Linux.\n");
	//printf(" Please Press a key...\n");
	/* open device */
	fd = open(driver, O_RDWR);
	if(fd < 0)
	{
		printf("device open fail\n");
		return -1;
	}
	/* set zlg7290 slave address */
	ioctl(fd, I2C_SLAVE_FORCE, ZLG_SLAVE_ADDR);
	/* set i2c bus clock 250KHz */
	ioctl(fd, I2C_SET_BUS_CLOCK, 250*1000);

	/* display all segment */
	key = 0xff;
	for(i=0; i<8; i++)
	{
		/* zlg7290 inner register access address */
		ioctl(fd, I2C_SET_DATA_ADDR, REG_Dis7-i);
		write(fd, &key, 1);
	}
	sleep(2);

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
					{
						//printf("(row %d, col %d) Key: %c\n", cy, cx,key);
						return key;
					}
					//if(key == 0xFF) printf("(row %d, col %d) Key: FUN\n", cy, cx);
				}
				if(cy == 4 && cx == 5)// 'FUN' to exit..
				{
					//break;
					return -1;
				}
			}
		}
	
	//printf(" end.\n");
	close(fd);

	return -1;
}
