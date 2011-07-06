#include <stdio.h>
#include <termios.h>

int  setTerm(int fd,struct termios * save_term);

int main (int argc, char const* argv[])
{
    FILE * fp;
    struct termios save_term;

    if((fp=fopen(ctermid(NULL),"r+"))==NULL)
    {
        printf("open device error!\n");
        return -1;
    }

    if(setTerm(fileno(fp),&save_term)!=-1)
    {
        printf("set terminal succeed!\n");
        return 0;
    }
    else
    {
        return -1;
    }
}
