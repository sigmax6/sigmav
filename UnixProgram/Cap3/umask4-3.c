#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main (int argc, char const* argv[])
{
   // mode_t ori_mode;
   // ori_mode=
   umask(0);
   // printf("orimode is %o",mode_t);
    if(creat("foo",S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) < 0)
    {
        printf ("create foo error!\n");
        return -1;
    }
    umask(S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    if(creat("bar",S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) < 0)
    {
        printf ("create bar error!\n");
        return -1;
    }
    return 0;
}

