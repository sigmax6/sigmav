#include <errno.h>        //errno perror
#include <stdio.h>
#include <string.h>       //strerror
int main (int argc, char const* argv[])
{
    fprintf(stderr,"EACCES:%s\n",strerror(EACCES));

    errno=ENOENT;
    perror(argv[0]);

    return 0;
}

