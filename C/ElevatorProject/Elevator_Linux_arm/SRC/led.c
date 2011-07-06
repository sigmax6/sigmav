/*********************************************************************
* File£º	test-led.c
* Author£º	Embest
* Desc£º
* History£º
*			Original version, 2005.4 Embest
*			Program modify, 2005.06.05 Embest R.X.Huang <hungrx@embedinfo.com>
*			Changed by @sigmax6
*			Used by @sigmax6
*********************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/fcntl.h>


#define LED_NUM		4

#define LED_OFF		0
#define LED_ON		1
#define LED_ON_ALL	0xF
#define LED_OFF_ALL	0x0


int led(int led_num)
{
	int i, j, wval, rval, fd;
	static char *driver = "/dev/led/0";

//	printf("LED test example base on Linux. \n");

	/* open led device */
	fd = open(driver, O_RDWR);
	if(fd <0)
	{
		printf("Can't not open %s\n",driver);
		return fd;
	}
    if(led_num<LED_NUM);
    {
       wval=LED_ON<<led_num;
       write(fd,&wval,1);
       read(fd,&rval,1);
    }
    else
    {
       printf("LED_NUM_ERROR!\n");
    }
	/* test single led */
	//for(i=0; i<3; i++)
//	{
//		for(j=0; j<LED_NUM; j++)
//		{
//			wval = LED_ON<<j;
//			write(fd, &wval, 1);
//			read(fd, &rval, 1);
//			printf("Turn on LED%d, readback val = 0x%02X\n", j+1, rval&0xF);
//			/* delay 500ms */
//			usleep(500*1000);
//		}
//	}

	/* test all leds */
//	for(i=0; i<6; i++)
//	{
//		if(i%2)
//		{
//			wval = LED_OFF_ALL;
//			write(fd, &wval, 1);
//			read(fd, &rval, 1);
//			printf("Turn off all LEDs, readback val = 0x%02X\n", rval&0xF);
//		}
//		else
//		{
//			wval = LED_ON_ALL;
//			write(fd, &wval, 1);
//			read(fd, &rval, 1);
//			printf("Turn on all LEDs, readback val = 0x%02X\n", rval&0xF);
//		}
		/* delay 1s */
	sleep(1);


//	printf(" end.\n");
	/* close led device */
	close(fd);

	return 0;
}
