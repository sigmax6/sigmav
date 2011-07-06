#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

int main (int argc, char const* argv[])
{
   struct termios term;
   long vdisable;

   //Check if the input is a terminal device or not;
   if(!isatty(STDIN_FILENO))
   {
       printf("stardard input is not a terminal device!\n");
       exit(-1);
   }
   //Set the vdisable by fpathconf;
   if((vdisable=fpathconf(STDIN_FILENO,_PC_VDISABLE))<0)
   {
       printf("fpathconf error or _PC_VDISABLE is not effected!\n");
       exit(-1);
   }
   //Fetch tty state
   if(tcgetattr(STDIN_FILENO,&term)<0)
   {
       printf("tcgetattr error!\n");
       exit(-1);
   }
   //Set the term struct just read~
   term.c_cc[VINTR]=vdisable;    //disable the INTR character
   term.c_cc[VEOF]=2;            //set EOF to Control-B
   //Set tty state
   if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&term)<0)
   {
       printf("tcsetattr error!\n");
       exit(-1);
   }
   exit(0);
}
