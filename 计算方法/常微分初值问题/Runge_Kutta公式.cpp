#include<iostream>
#include<math.h>
using namespace std;
double h;
double f(double x,double y)
{
	return sin(x*y);
}
double Runge_Kutta(double x,double y)
{
	double k1,k2,k3,k4;
	k1=f(x,y);
	k2=f(x+h/2,y+h*k1/2);
	k3=f(x+h/2,y+h*k2/2);
	k4=f(x+h,y+h*k3);
	return y+h*(k1+2*k2+2*k3+k4)/6;
}
int main()
{
	cout <<"输入步长"<<endl;
	cin >>h;
	cout <<"输入x，y初始值"<<endl;
	double x,y;
	cin >>x>>y;
	cout <<"输入增加多少步长"<<endl;
	int n,i=0;
	cin >>n;
	
	cout <<endl<<"Runge_Kutta公式"<<endl;
	for(i=0;i<=n;i++)
	{
		cout <<x<<": "<<y<<endl;
		y=Runge_Kutta(x,y);
		x=x+h;
	}
	return 0;
}
