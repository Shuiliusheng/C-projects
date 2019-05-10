#include<iostream>
using namespace std;
int main()
{
	double x1,x2,x3,x4,x5,x6;
	int i=0;
	double s=0;
	cout <<"input the num:"<<endl;
	cin >>i;
	double *y=new double[i+1];
	double *y1=new double[i+1];
	cout <<"input y:"<<endl;
	for(int n1=0;n1<i;n1++)
		cin >>y[n1];
	cout <<"input x:"<<endl;
	for(int n=0;n<i;n++)
	{
		cin >>x1>>x2>>x3>>x4>>x5>>x6;
		double y2=-0.987428+0.000001*x1-0.000004*x2+0.429816*x3-0.000000*x4+0.570166*x5+0.000001*x6;
		s+=((y[n]-y2)*(y[n]-y2));
		y1[n]=y2;
	}
	for(int n2=0;n2<i;n2++)
	{
		cout <<y[n2]<<"  "<<y1[n2]<<endl;
	}
	s/=i;
	cout <<"·½²îÎª: "<<s<<endl;
	return 0;
}