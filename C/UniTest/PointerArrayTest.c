#include<stdio.h>
int main (int argc, char const* argv[])
{
    char * p1,p2;
    char ch[12];
    char **pp;
    p1=ch;
    pp=&ch;
    p2=*pp;

    printf ("%d\n",pp);
    printf ("%d\n%d\n",p1,p2);
    return 0;
}

