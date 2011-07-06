//setTerm.c

//Set the terminal for spcial use
//Sun Oct 10 14:17:16 CST 2010

#include <termios.h>
#include <stdio.h>

char * speedStr[15]=
{
    "B50",
    "B75",
    "B110",
    "B134",
    "B150",
    "B200",
    "B300",
    "B600",
    "B1200",
    "B1800",
    "B2400",
    "B4800",
    "B9600",
    "B19200",
    "B38400"
};

speed_t speedVal[15]=
{
    B50,
    B75,
    B110,
    B134,
    B150,
    B200,
    B300,
    B600,
    B1200,
    B1800,
    B2400,
    B9600,
    B19200,
    B38400
};

char * csizeStr[4]=
{
    "5 bits/Byte",
    "6 bits/Byte",
    "7 bits/Byte",
    "8 bits/Byte"
};

int csizeVal[4]=
{
    CS5,
    CS6,
    CS7,
    CS8
};

char * stopBitStr[2]=
{
    "1 bit",
    "2 bit"
};

char * oddEvenStr[3]=
{
    "NONE",
    "ODD",
    "EVEN"
};

int setTerm(int fd,struct termios * save_term)
{
    int counter,select;
    struct termios term;
    if(tcgetattr(fd,&term)<0)
    {
        printf("the fd is not a terminal!\n");
        return -1;
    }

    *save_term = term;

    //Set the baud rate
    printf("Select the input baud rate:\n");
    for(counter=0;counter<15;counter++)
    {
        printf("%d:%s\n",counter,speedStr[counter]);
    }
    scanf("%d",&select);

    if(select<0 || select>14)
    {
        printf("wrong select!\n");
        return -1;
    }

    if(cfsetispeed(&term,speedVal[select])==-1)
    {
        printf("set input baud rate error!\n");
        return -1;
    }

    printf("Select the output baud rate:\n");
    for(counter=0;counter<15;counter++)
    {
        printf("%d:%s\n",counter,speedStr[counter]);
    }
    scanf("%d",&select);

    if(select<0 || select>14)
    {
        printf("wrong select!\n");
        return -1;
    }

    if(cfsetospeed(&term,speedVal[select])==-1)
    {
        printf("set output baud rate error!\n");
        return -1;
    }

    //Set the CSIZE(bits/Byte)
    printf("select the CSIZE (bits/Byte)\n");
    for(counter=0;counter<4;counter++)
    {
        printf("%d:%s\n",counter,csizeStr[counter]);
    }
    scanf("%d",&select);

    if(select<0 || select>3)
    {
        printf("wrong select!\n");
        return -1;
    }

    term.c_cflag &= ~CSIZE;    //zero out the origenal CSIZE bit;
    term.c_cflag |= csizeVal[select];  //set the new szie;

    //Set the Stop Bit
    printf("slect the stop bit\n");
    for(counter = 0 ; counter < 2 ; counter ++)
    {
        printf("%d:%s\n",counter,stopBitStr[counter]);
    }

    scanf("%d",&select);

    if(select < 0 || select > 1)
    {
        printf("wrong select!\n");
        return -1;
    }

    if(select == 0)
    {
        term.c_cflag &= ~CSTOPB;
    }
    else
    {
        term.c_cflag |= CSTOPB;
    }

    //Set the odd-even check bit
    printf("select the odd-even check mode\n");
    for (counter = 0 ; counter < 3 ; counter ++)
    {
        printf("%d:%s\n",counter,oddEvenStr[counter]);
    }
    scanf("%d",&select);
    if(select < 0 || select > 2)
    {
        printf("wrong select!\n");
        return -1;
    }

    if(select == 0)
    {
        term.c_cflag &= ~PARENB;
    }
    else
    {
        term.c_cflag |= PARENB;
        if(select == 1 )
        {
            term.c_cflag |= PARODD;
        }
        else
        {
            term.c_cflag &= ~PARODD;
        }
    }

    //Apply the change  to the fd terminal
    if (tcsetattr(fd,TCSAFLUSH,&term)<0)
    {
        printf("Apply the change to the fd failed!\n");
        return -1;
    }

}
//Last Change :Sun Oct 10 15:04:28 CST 2010

