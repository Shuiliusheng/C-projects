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
	cout <<"���벽��"<<endl;
	cin >>h;
	cout <<"����x�ĳ�ʼֵ"<<endl;
	cin >>x0;
	for(int n=0;n<15;n++)
	{
		double t=x0+n*h;
		double t1=f(t+h),t2=f(t-h);
		cout <<t<<": "<<"һ��  ";
		cout <<((t1-t2)/2/h)<<"   ����  ";
		cout <<((t1+t2-2*f(t))/h/h)<<endl;
	}
	return 0;
}