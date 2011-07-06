#include<stdio.h>
int main (int argc, char const* argv[])
{
    char * ptr=/*/**/"*/"/*"/**/;
    int res=/*/*/0*/**/1;
    printf("%s\n",ptr);
    printf("%d\n",res);
    if(res)
    {
       printf("Nesting Disabled\n");
    }
    else
    {
        printf("Nesting Enabled\n");
    }
    return 0;
}
