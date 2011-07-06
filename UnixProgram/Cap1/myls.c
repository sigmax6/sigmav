#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

int main (int argc, char const* argv[])
{
    char * dir_name = ".";
    DIR * dp;
    struct dirent *dirp;

    if(argc == 2)
    {
        dir_name = (char *)argv[1];
    }
    else if(argc !=1)
    {
        printf("input error\nUsage : ls <dir name>!\n");
        return -1;
    }

    if((dp = opendir(dir_name)) == NULL )
    {
        printf ("open dir %s error\n",dir_name);
        return -1;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        printf ("%8s",dirp->d_name);
    }

    closedir (dp);

    return 0;
}

