#include<iostream>
#include<math.h>
using namespace std;
double f(double x)
{
	return sin(x)*cos(x);
}
int main()
{
	double h;
	double x0;
	cout <<"输入步长"<<endl;
	cin >>h;
	cout <<"输入x的初始值"<<endl;
	cin >>x0;
	for(int n=0;n<15;n++)
	{
		double t=x0+n*h;
		double t1=f(t+h),t2=f(t-h);
		cout <<t<<": "<<"一阶  ";
		cout <<((t1-t2)/2/h)<<"   二阶  ";
		cout <<((t1+t2-2*f(t))/h/h)<<endl;
	}
	return 0;
}