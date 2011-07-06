#include<stdio.h>

int main()
{
    printf("you are now in fuction %s\n",__FUNCTION__);
    foo();
    return 0;
}

int foo()
{
    printf("you are now in function %s\n",__FUNCTION__);
    return 0;
}
