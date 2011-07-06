#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char const* argv[])
{
    char * slashArray="\\|/-";
    int slashCount = strlen(slashArray);
    printf("%d\n",slashCount);
    int slashPtr = 0;
    printf ("%c",slashArray[slashPtr]);

    while(1)
    {
        if(slashPtr == slashCount )
        {
            slashPtr = 0;
        }
        /*putchar('\b');*/
        printf("\b");
        printf("%c",slashArray[slashPtr++]);
        usleep(100);
        //usleep(10);

    }
    return 0;
}
