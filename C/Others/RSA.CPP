#include<iostream>
#include<cmath>
using namespace std;

void main()
{
	int p,q;
	cout<<"请输入两个较大的素数："<<endl;
	cin>>p>>q;
	cout<<"p="<<p<<",q="<<q<<endl;
	int n,o;
	n=p*q;
	o=(p-1)*(q-1);
	cout<<"n="<<n<<",o="<<o<<endl;
	cout<<"请从【0，"<<o-1<<"】中选择一个与"<<o<<"互素的数e："<<endl;
	int e,i;
	float d;
	cin>>e;
	for(i=1;;i++)
	{
		d=(float)(o*i+1)/e;
		if(d-(int)d==0)
			break;
	}
	cout<<"e="<<e<<",d="<<d<<endl;
	cout<<"公开密钥Pk={e,n}={"<<e<<","<<n<<"}"<<endl;
	cout<<"秘密密钥Sk={d,n}={"<<d<<","<<n<<"}"<<endl;
	cout<<endl;
	
	cout<<"请输入要加密的正整数（以-1结束）："<<endl;
	int m1[500],m3[500],m4[500];
	double m2[500];
	int j;
	for(j=0;j<500;j++)
	{
		cin>>m1[j];
		if(m1[j]==-1)
			break;
		m2[j]=pow(m1[j],e);
		m4[j]=m2[j]/n;
		m3[j]=m2[j]-m4[j]*n;
	}
	cout<<"密文为："<<endl;
	int k;
	for(k=0;k<j;k++)
		cout<<m3[k]<<" ";
	cout<<endl;
}