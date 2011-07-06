#include <dirent.h>
#include <stdio.h>

int main (int argc, char const* argv[])
{
    DIR * dir;
    struct dirent * dirp;

    if(argc != 2)
    {
        printf("Usage %s <path>\n",argv[0]);
        return -1;
    }
    if((dir=opendir(argv[1])) != NULL)
    {
        while ( (dirp=readdir(dir)) != NULL )
        {
            printf("%s ",dirp->d_name);
        }
        puts("\n");
        return 0;
    }
    else
    {
        printf("error opening %s\n",argv[1]);
        return -1;
    }
}

