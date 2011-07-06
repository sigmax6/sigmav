#include <stdio.h>
int main (int argc, char const* argv[])
{
    int input;
    printf("Input the number to detect\n");
    scanf("%d",&input);
    if(input & (input-1))
    {
        printf("Is not 2^n \n");
    }
    else
    {
        printf("Is 2 ^n \n");
    }
    return 0;
}
