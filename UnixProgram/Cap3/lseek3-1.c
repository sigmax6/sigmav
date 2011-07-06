#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char const* argv[])
{
    off_t offset;

    if((offset = lseek(STDIN_FILENO,0,SEEK_CUR)) == -1)
    {
        printf("cannot seek\n");
    }
    else
    {
        printf("can seek\n");
    }
    return 0;
}

