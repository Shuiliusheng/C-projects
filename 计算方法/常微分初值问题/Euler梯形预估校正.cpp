#include<iostream>
#include<math.h>
using namespace std;
double h;
double f(double x,double y)
{
	return sin(x*y);
}
double Euler1(double x,double y)
{
	double y1=h*f(x,y),y2=y+y1;
	return y+y1/2+h*0.5*f(x+h,y2);
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

	cout <<endl<<"Euler梯形预估校正公式"<<endl;
	for(i=0;i<=n;i++)
	{
		cout <<x<<": "<<y<<endl;
		y=Euler1(x,y);
		x=x+h;
	}
	return 0;
}
