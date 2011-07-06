#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>

//#define READ 1
//#define OPEN
#define WRITE 1
#define DELAY 1

unsigned char TxBuf[4] ={0};
unsigned char RxBuf[4] ={0};


int main(void)
{
    int fd = -1;
    int count = 1;
    int ret = 0;

#ifndef OPEN 
   //fd = open("/dev/nrf24l01", 0);
    fd = open("/dev/nrf24l01", O_RDWR);   //打开nrf24l01为可读写文件
    if(fd < 0)
    {
        perror("Can't open /dev/nrf24l01 \n");
        exit(1);
    }
    printf("open /dev/nrf24l01 success \n");
#endif
  	TxBuf[0] = 0x1;

    while(1)
    {
#ifdef WRITE
        write(fd, &TxBuf , sizeof(TxBuf));
        printf("write %d\n",TxBuf[0])  ;
#endif 

#ifdef READ
        ret=read(fd,&RxBuf,sizeof(RxBuf));
        if(ret)
        {
          int i= 0;
          for (i=0;i<sizeof(RxBuf);i++)
          {
             printf("%d ",RxBuf[i]);
          }
          printf("\n");
        }
#endif

#ifdef OPEN
    fd = open("/dev/nrf24l01", O_RDWR);   //打开nrf24l01为可读写文件
    if(fd < 0)
    {
        perror("Can't open /dev/nrf24l01 \n");
        exit(1);
    }
    printf("open /dev/nrf24l01 success \n");
    usleep(100);
    close(fd);
#endif
       // printf("Sending %d time \n", count);
   //     usleep(1000);
            
       // count++;
#ifdef WRITE
        TxBuf[0] <<= 1;
        if(!TxBuf[0])
        	{
        		TxBuf[0] = 0x1;
        	}
#endif
#ifdef DELAY
//	getchar();
           usleep(10000);
#endif
       
    }
    
#ifndef OPEN
    close(fd);
#endif
}
