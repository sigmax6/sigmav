#include <unistd.h>
#include <termios.h>

static struct termios save_termios;                    //Common termios struct
static int            ttysavefd=-1;                    //Commom fd
static enum {RESET,RAW,CBREAK} ttystate=RESET;

int tty_cbreak(int fd) //Set the terminal to cbreak mode
{
   struct termios buf;

   if((tcgetattr(fd,&save_termios))<0)                 //get and save the termios struct
   {
       return (-1);
   }

   buf=save_termios;

   buf.c_lflag &= ~(ECHO|ICANON) ;                     //Trun off the ECHO and disable ICANON mode

   buf.c_cc[VMIN]=1;
   buf.c_cc[VTIME]=0;                                  //1 byte at a time ,no timer ,B mode

   if((tcsetattr(fd,TCSAFLUSH,&buf))<0)
   {
       return (-1);                                    //Set the termios struct changed
   }

   ttystate=CBREAK;
   ttysavefd=fd;
   return(0);
}

int tty_raw(int fd)
{
    struct termios buf;

    if((tcgetattr(fd,&save_termios))<0)
    {
        return (-1);
    }

    buf=save_termios;

    buf.c_lflag &= ~(ECHO|ICANON|IEXTEN|ISIG);          //echo off ,canonical mode off,extened input processing off,
                                                        //signal chars off
    buf.c_iflag &= ~(BRKINT|ICRNL|INPCK|ISTRIP|IXON);   //no SIGINT on BREAK, CR_TO_NL off, input parity check off,
                                                        //do not strip 8th bit on input
                                                        //output flow control off
    buf.c_cflag &= ~(CSIZE|PARENB);                     //clear size bits, parity checking off
    buf.c_cflag |= CS8;                                 //set 8 bits/char

    buf.c_oflag &= ~(OPOST);                            //out put processing off

    buf.c_cc[VMIN]=1;
    buf.c_cc[VTIME]=0;                                  //1 Byte at a time

    if((tcsetattr(fd,TCSAFLUSH,&buf))<0)
    {
        return (-1);
    }

    ttystate =RAW;
    ttysavefd=fd;
    return (0);
}

int tty_reset(int fd)
{
    if(ttystate !=CBREAK && ttystate != RAW)
    {
        return (0);
    }

    if((tcsetattr(fd,TCSAFLUSH,&save_termios))<0)
    {
        return (-1);
    }

    ttystate = RESET;
    return (0);
}

void tty_atexit(void)
{
    if(ttysavefd > 0)
    {
        ttyreset(ttysavefd);
    }
}

struct termios * tty_termios(void)
{
    return (&save_termios);
}

