#include <stdio.h>
#include <string.h>

#define MAX 10000000

int DelSpace(char * str)
{
    char * ptr = str;
    char * dtr = str;
    char tmp = 0;
    while((tmp=*ptr) != '\0')
    {
        if(tmp ==' ')
        {
            ptr++;
        }
        else
        {
            if(ptr!=dtr)
            {
                *dtr=tmp;
            }
            ptr++;
            dtr++;
        }
    }
    *dtr='\0';
    return 1;
}

int main (int argc, char const* argv[])
{

    char test[20] ="this a test";
//    char * test1=test;
    DelSpace(test);
    /*DelSpace(test2);*/
    /*DelSpace(test3);*/
    printf("%s\n",test);
    return 0;
}

