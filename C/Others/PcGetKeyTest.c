#include<stdio.h>
int main (int argc, char const* argv[])
{
    char key;
    for(;;)
    {
        PC_GetKey(&key);
        putchar(key);
    }
    return 0;
}
