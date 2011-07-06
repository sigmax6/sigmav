#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

int main (int argc, char const* argv[])
{
    DIR * dp;                    //Dir pointer
    struct dirent * dirp;        //Dir struct pointer

    if(argc != 2)
    {
        printf("a argument (dir name) is required!\n");
        return -1;
    }
    if((dp = opendir(argv[1])) == NULL)
    {
        printf("can't open %s!\n",argv[1]);
        return -1;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        printf("%s\n",dirp->d_name);
    }

    closedir(dp);

    return 0;
}

