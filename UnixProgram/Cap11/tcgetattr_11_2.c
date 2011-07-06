#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char const* argv[])
{
    struct termios term;
    int size;

    if(tcgetattr(STDIN_FILENO,&term)<0)  //Get the term stuct by tcgetattr
    {
        printf("Tcgetattr error!\n");
        exit(-1);
    }

    size=term.c_cflag & CSIZE;
    switch(size)                         //Detect the CSIZE
    {
    case CS5:
        printf("5 bits/byte\n");
        break;
    case CS6:
        printf("6 bits/byte\n");
        break;
    case CS7:
        printf("7 bits/byte\n");
        break;
    case CS8:
        printf("8 bits/byte\n");
        break;
    default:
        printf("unknown\n");
        break;
    }

    term.c_cflag &= ~CSIZE;           //Zero out the CSIZE bit
    printf("Set the CSIZE bit you want: 5-8\n");
    scanf("%d",&size);
    switch(size)
    {
    case 5:
      term.c_cflag |= CS5;              //Set the CSIZE bit to CS5
      break;
    case 6:
      term.c_cflag |= CS6;              //Set the CSIZE bit to CS6
      break;
    case 7:
      term.c_cflag |= CS7;              //Set the CSIZE bit to CS7
      break;
    case 8:
    default:
      term.c_cflag |= CS8;              //Set the CSIZE bit to CS8
      size=8;
      break;
    }


    if(tcsetattr(STDIN_FILENO,TCSANOW,&term)<0)
    {
        printf("Tcsetattr error!\n");
        exit(-1);
    }

    printf("Set CSIZE bit to %d succeed!\n",size);
    return 0;

}

