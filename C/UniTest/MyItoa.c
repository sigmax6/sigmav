#include <stdio.h>

int MyItoa(char s[])
{
    int result=0;
    int counter=0;
   // int digit=1;
    for(counter=0;s[counter]!='\0';counter++)
    {
        result=(result*10)+(s[counter]-48);
       // digit*=10;
    }
    return result;
}

int main (int argc, char const* argv[])
{
    //printf("%d\n",a);
    // return 0;
    int res1=MyItoa("1234");
    char testString[]="3456";
    int res2=MyItoa(testString);
    printf("res1:%d,res2:%d",res1,res2);
    return 0;
}

