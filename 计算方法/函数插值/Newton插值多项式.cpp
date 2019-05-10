#include<iostream>
#include<stdlib.h>
using namespace std;
double *x=NULL;//储存自变量
double *f=NULL;//储存系数
double *t=NULL;//存储临时变量，减少重复计算
void calfirst(int n)//计算系数
{
	for(int i=1;i<n;i++)
	{
		double t=f[i-1];
		for(int j=i;j<n;j++)
		{
			double c=f[j]-t;
			t=f[j];
			f[j]=c/(x[j]-x[j-i]);
		}
	}
}
void result(double temp,int n)//自变量为temp是的结果
{
	double a=1;
	for(int i=0;i<n-1;i++)
	{
		t[i]=(temp-x[i])*a;
		a=t[i];
	}
	double r=0;
	a=1;
	for(int j=0;j<n;j++)
	{
		r=r+a*f[j];
		a=t[j];
	}
	cout <<"结果\n";
	cout <<r<<endl;
}
void cal(int c)//计算
{
	int n;
	cout <<endl<<"输入测试数据的个数\n";
	cin >>n;
	while(n--!=0)
	{
		double temp=0;
		cout <<"input:"<<endl;
		cin >>temp;
		result(temp,c);
	}
}
int main()
{
	int n;
	int i=0,j=0;
	cout <<"输入已知节点个数\n";
	cin >>n;
	x=new double[n+2];
	f=new double[n+2];
	t=new double[n+2];
	cout <<"输入相应的x值和y值，空格隔开\n";
	for(i=0;i<n;i++)
		cin >>x[i]>>f[i];
	calfirst(n);//计算系数
	cal(n);//计算插值结果
	delete x;
	delete f;
	delete t;
	return 0;
}