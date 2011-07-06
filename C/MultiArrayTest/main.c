#include<stdio.h>
int main(void)
{
    int mutiArray[2][3];
    int i,j;
    for(i=0;i<2;i++)
    {
        for(j=0;j<3;j++)
        {
            mutiArray[i][j]=i+j;
        }
    }
    i=1;
    j=2;
    (i>0&&j>0)?mutiArray[i][j]=0:0;
    for(i=0;i<2;i++)
    {
        for(j=0;j<3;j++)
        {
            printf("%d ",mutiArray[i][j]);
        }
        printf("\n");
    }
    return 0;

}
