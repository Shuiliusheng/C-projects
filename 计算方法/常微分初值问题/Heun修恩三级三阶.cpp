#include<iostream>
#include<math.h>
using namespace std;
double h;
double f(double x,double y)
{
	return sin(x*y);
}
double Heun(double x,double y)
{
	double k1,k2,k3;
	k1=f(x,y);
	k2=f(x+h/3,y+h*k1/3);
	k3=f(x+2*h/3,y+2*k2*h/3);
	return y+h*(k1+3*k3)/4;
}
int main()
{
	cout <<"���벽��"<<endl;
	cin >>h;
	cout <<"����x��y��ʼֵ"<<endl;
	double x,y;
	cin >>x>>y;
	cout <<"�������Ӷ��ٲ���"<<endl;
	int n,i=0;
	cin >>n;
	cout <<endl<<"��������Heun��ʽ"<<endl;
	for(i=0;i<=n;i++)
	{
		cout <<x<<": "<<y<<endl;
		y=Heun(x,y);
		x=x+h;
	}
	return 0;
}
