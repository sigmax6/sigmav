#include<time.h>
#include<stdlib.h>
#define MAX 10
int main()
{
    int total=1890;
    int temp[MAX];
    int length=4;
    int i;
    int sigma=0;
    srand((int)time(0));
    for(i=0;i<length;i++)
    {
        temp[i]=1+(int)(100.0*rand()/(RAND_MAX+1.0));
        sigma+=temp[i];
    }
    for(i=0;i<length;i++)
    {
        temp[i]=temp[i]*total/sigma;
        printf("%d ",temp[i]);
    }
    return 0;
}
