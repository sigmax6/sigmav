#include<stdio.h>
#include<math.h>

#define M 3
#define P 101.325  //压力单位Kpa
#define A 0.001    //精度
#define S 1        //步长

double a[M][3],k[M],x[M];
double t0,t1,f=0,df=0;
double temp=0;
int i,j;
long n=0;

void intput()           //输入函数
{
	for(i=0;i<M;i++)
	{
		printf("请输入第%d组分Antoine公式中A,B,C值：\n",i);
		for(j=0;j<3;j++)
        {
			scanf("%lf",&a[i][j]);
        }
	}
	for(i=0;i<M;i++)
	{
		printf("请输入第%d组分摩尔分率：\n",i);
		scanf("%lf",&x[i]);
	}
	printf("请输入温度初值：\n");
	scanf("%lf",&t1);
}

void newton()           //牛顿迭代法函数
{
	printf("牛顿迭代法：\n");
	do
	{
		n++;
		t0=t1;
		for(i=0;i<M;i++)
        {
			k[i]=pow(10.0,(2.303*(a[i][0]-a[i][1]/(t0+a[i][2]))))/P;
        }
		for(i=0;i<M;i++)
		{
			temp=k[i]*x[i];
			f=f+temp;
		}
		f=f-1;
		for(i=0;i<M;i++)
		{
			temp=k[i]*x[i]*2.303*a[i][1]/(t0+a[i][2])/(t0+a[i][2]);
			df=df+temp;
		}
		t1=t0-f/df;
	}
	while(fabs(t1-t0)>A);
	printf("塔釜温度为%.1f\n",t1);  //温度取一位小数，可更改
	printf("迭代次数：%ld",n);
}


void buchang()               //步长法or试差法函数
{
	printf("步长法:\n");
	double sum=0.0;
	do
	{
		n++;
		for(i=0;i<M;i++)
		{
			k[i]=pow(10.0,(2.303*(a[i][0]-a[i][1]/(t1+a[i][2]))))/P;
			sum+=k[i]*x[i];
		}
		if(sum-1>0)
			t1-=S;
		if(sum-1<0)
			t1+=S;

	}
	while(fabs(sum-1)>A);
	printf("塔釜温度：%.1f,循环次数：%ld\n",t1,n);
}


void main()           //主函数
{
	intput();
	int z;
	printf("请选择求解方法：1、newton迭代法，2、步长法，3、两种方法");
	scanf("%d",&z);
	if(z==1)
		newton();
	if(z==2)
		buchang();
	if(z==3)
	{
		newton();
		buchang();
	}
}
