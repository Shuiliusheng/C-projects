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
	cout <<"���벽��"<<endl;
	cin >>h;
	cout <<"����x��y��ʼֵ"<<endl;
	double x,y;
	cin >>x>>y;
	cout <<"�������Ӷ��ٲ���"<<endl;
	int n,i=0;
	cin >>n;

	cout <<endl<<"Euler����Ԥ��У����ʽ"<<endl;
	for(i=0;i<=n;i++)
	{
		cout <<x<<": "<<y<<endl;
		y=Euler1(x,y);
		x=x+h;
	}
	return 0;
}
