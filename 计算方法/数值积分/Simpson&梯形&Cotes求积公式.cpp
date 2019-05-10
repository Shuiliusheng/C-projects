#include<iostream>
#include<math.h>
using namespace std;
double a,b;
double f(double x)
{
	return sin(x);
}

double T(double a,double b)
{
	return (b-a)*(f(a)+f(b))/2;
}
double simpson(double a,double b)
{
	return ((b-a)*(f(a)+4*f((a+b)/2)+f(b))/6);
}
double Cotes(double a,double b)
{
	double t=(b-a)*(7*f(a)+32*f((3*a+b)/4)+12*f((a+b)/2)+32*f((a+3*b)/4)+7*f(b))/90;
	return t;
}

int main()
{
	cout <<"输入下限"<<endl;
	cin >>a;
	cout <<"输入上限"<<endl;
	cin >>b;
	double h;
	cout <<"输入步长"<<endl;
	cin >>h;
	if(b<a)
		return 0;
	double r=0,r1=0,r2=0;
	for(int i=0;a<b;i++)
	{
		if(b-a<=h)
		{
			r+=T(a,b);
			r1+=simpson(a,b);
			r2+=Cotes(a,b);
		}
		else
		{
			r+=T(a,a+h);
			r1+=simpson(a,a+h);
			r2+=Cotes(a,a+h);
		}
		a+=h;
	}
	cout <<"T: "<<r<<endl;
	cout <<"Simpson: "<<r1<<endl;
	cout <<"Cotes: "<<r2<<endl;
	return 0;
}