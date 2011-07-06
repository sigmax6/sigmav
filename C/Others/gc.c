#include<stdio.h>
#include<unistd.h>

int main()
{
    char in[1];
    ssize_t bytesread;

    while(1)
    {
       if((bytesread=read(STDIN_FILENO,in,1))!=-1)
        printf("%c\n",*in);
       else
        printf("Error\n");
    }
    return 0;
}
