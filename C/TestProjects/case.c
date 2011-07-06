#include<stdio.h>
int main()
{
    int t=3;
    switch(t)
    {
        case 0 ... 2:
        printf("0/1/2\n");
        break;
        case 3 ... 5:
        printf("3/4/5\n");
        break;
    }
    return 0;
}
