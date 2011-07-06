#include<stdio.h>

int main (int argc, char const* argv[])
{
    char a='\0';
    printf("%d\n",isspace(a));
    char * tr1="tt  ";
    char * tr2="tt ";
    printf("%d\n",strcmp(tr1,tr2));
    printf("%d-%d\n",'a','Z');
    char * tt3="abcddddddddddddddddddd";
    char * tty="zzz";
    tt3=strcpy(tt3,"ZZZ");
    printf("%s",tr1);
    return 0;
}
