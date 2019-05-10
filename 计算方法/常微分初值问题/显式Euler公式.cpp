#include<iostream>
#include<math.h>
using namespace std;
double h;
double f(double x,double y)
{
	return y-2*x/y;
}
double Euler(double x,double y)
{
	return y+h*f(x,y);
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

	for(i=0;i<=n;i++)
	{
		cout <<x<<": "<<y<<endl;
		y=Euler(x,y);
		x=x+h;
	}
	return 0;
}
