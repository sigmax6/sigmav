/*
习题 49：成绩统计★

问题描述：
某高校要统计校内学生的各科成绩总分和各门科的平均分，需要编写一个程序来完成

输入：
多组测试数据，每组测试数据里：第一行是n,k(1<=n<=1e6,1<=k<=1000)，
n是学生数目，k是科目数。接下来就是n*k个数，
每k个就是一个学生所有科目的成绩，是在0-100之间的数

输出：
对于每组测试数据：
第一行输出各学生的成绩总分
第二行输出各科平均分（四舍五入到整数）
最后多输出一个空行表示分隔

样例输入：
3 2
100 100
90 90
80 80
2 3
90 90 90
60 60 60
2 2
1 2 3 4

样例输出：
200 180 160
90 90

270 180
75 75 75

3 7
2 3

*/

#include <stdio.h>
//#define NMAX 1000000
#define KMAX 1000
int main(void)
{
//    char Score[NMAX];
    int Avr[KMAX]={0};
    int n,k,nptr,kptr,input,sum;
    while(scanf("%d%d",&n,&k)!=EOF)
    {
        getchar();
        nptr=n;
        kptr=k;
        while(kptr)
        {
            Avr[(kptr--)-1]=0;
        }
        do
        {
            kptr=k;
            //Score[nptr-1]=0;
            sum=0;
            do
            {
                scanf("%d",&input);
                //Score[nptr-1]+=input;
                sum+=input;
                Avr[kptr-1]+=input;
                kptr--;
            }while(kptr);
            //input=Score[nptr-1][0];
            //Score[nptr-1][1]=input/k+(((input%k)*10/k>=5)?1:0);
            printf("%d",sum);
            nptr--;
            nptr?putchar(' '):putchar('\n');
        }while(nptr);
//       putchar('\n');
//        nptr=n;
//        do
//        {
//            printf("%d ",Score[nptr-1]);
//            nptr--;
//        }while(nptr);
//        putchar('\n');
        do
        {
            input=Avr[k-1];
            input=(input+n/2)/n;
            printf("%d",input);
            k--;
            k?putchar(' '):putchar('\n');
        }while(k);
        putchar('\n');
    }
    return 0;
}



