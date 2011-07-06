#include <stdio.h>
#include <stdlib.h>

char * getpass(const char * prompt);

int main (int argc, char const* argv[])
{
    char * ptr;
    if((ptr=getpass("Please input the password!"))==NULL)
    {
        printf("Get pass error!\n");
        exit(1);
    }

    printf("The password you just entered is :%s",ptr);

    while(*ptr!=0)
    {
        *ptr++=0;              //zero out the password
    }

    return 0;
}
