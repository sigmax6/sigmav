#include <termios.h>

int isatty(int fd)
{
    struct termios term;
    return (tcgetattr(fd,&term)!=-1);  //TURE if not error
}
