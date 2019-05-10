#include<iostream>
#include<math.h>
using namespace std;
double f(double x)
{
	double y=x-cos(x);
	return y;
}
double f1(double x)
{
	double y=1+sin(x);
	return y;
}
double f2(double x)
{
	double y=cos(x);
	return y;
}
void cal(double x0,double min)
{
	double x1=x0+2*min;
	while(fabs(x0-x1)>min)
	{
		x1=x0;
		if(f1(x0)==0)
		{
			cout <<"区间错误，导数为零\n";
			return ;
		}
		else
			x0=x0-f(x0)/f1(x0);
	}
	cout <<"解为:"<<x0<<endl;
	cout <<"精度:"<<fabs(x0-x1)<<endl;

}
int charge(double a,double b)
{
	double t=f(a)*f(b);
	if(t<0&&a<=b)
		return 1;
	else if(t==0)
	{
		if(f(a)==0)
			cout <<"解为:"<<a<<endl;
		else
			cout <<"解为:"<<b<<endl;
		return 2;
	}
	else
		return 0;
}
double findx0(double a,double b)
{
	double t=(b-a)/200;
	for(int i=0;i<=200;i++)
	{
		double c=a+i*t;
		if(f(c)*f2(c)>0)
			return c;
	}
	return a-1;
}
int main()
{
	double a,b;
	cout <<"输入上下限"<<endl<<"下线:";
	cin >>a;
	cout <<"上线:";
	cin >>b;
	double min=0;
	int temp=charge(a,b);
	double x0=findx0(a,b);
	if(x0<a)
	{
		cout <<"区间错误,不满足初始值\n";
		return 0;
	}
	if(temp==1)
	{
		cout <<"输入精度\n";
		cin >>min;
		cal(x0,min);
		cout <<"初值为:"<<x0<<endl;
	}
	else if(temp==0)
		cout <<"区间错误，不满足有解\n";
	else 
	{
		cout <<"over\n";
		return 0;
	}
	return 0;
}