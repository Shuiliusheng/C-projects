#include<iostream>
#include<math.h>
using namespace std;
double f(double x)
{
	double y=x*x*x+4*x*x-10;
	return y;
}
void show(double a)
{
	int i=0;
	cout <<"a附近的值\n";
	for(i=0;i<5;i++)
		cout <<a+i*0.1<<":"<<f(a+i*0.1)<<endl;
	for(i=1;i<=5;i++)
		cout <<a+i<<":"<<f(a+i)<<endl;
	cout <<endl<<endl;
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
double cal(double a,double b,double min,double &x)
{
	while((b-a)>=min)
	{
		double x1=(a+b)/2;
		cout <<"["<<a<<","<<b<<"]"<<"  "<<x1<<endl;
		if(f(x1)==0)
		{
			x=x1;
			return 0;
		}
		else if(f(x1)*f(a)<0)
			b=x1;
		else
			a=x1;
	}
	x=(b+a)/2;
	return (b-a)/2; 

}
int main()
{
	double a,b;
	cout <<"输入上下限"<<endl<<"下线:";
	cin >>a;
	show(a);
	cout <<"上线:";
	cin >>b;
	double min=0;
	int temp=charge(a,b);
	if(temp==1)
	{
		cout <<"输入精度\n:";
		cin >>min;
		double min1=0,x=0;
		min1=cal(a,b,min,x);
		cout <<"解为:"<<x<<endl;
		cout <<"精度:"<<min1<<endl;
	}
	else if(temp==0)
		cout <<"区间错误\n";
	else 
		return 0;
	return 0;
}