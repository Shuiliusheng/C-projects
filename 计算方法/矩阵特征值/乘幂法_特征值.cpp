#include<iostream>
#include<math.h>
using namespace std;
double **str;
double *v,*v1;
int n=0;
void calv()
{
	for(int i=0;i<n;i++)
	{
		double t=0;
		for(int j=0;j<n;j++)
			t+=str[i][j]*v[j];
		v1[i]=t;
	}
}
void hist(double v[])
{
	int i=0;
	double t=v[0];
	for(i=0;i<n;i++)
		if(v[i]>t)
			t=v[i];
	for(i=0;i<n;i++)
		v[i]/=t;
}
int main()
{
	cout <<"�������"<<endl;
	cin >>n;
	int i=0,j=0;
	str=new double *[n];
	cout <<"�������"<<endl;
	for(i=0;i<n;i++)
	{
		str[i]=new double[n+1];
		for(j=0;j<n;j++)
			cin >>str[i][j];
	}
	cout <<"�����ʼ����"<<endl;
	v=new double[n+1];
	v1=new double[n+1];
	for(i=0;i<n;i++)
	{
		cin >>v[i];
		v1[i]=v[i];
	}
	cout <<"���뾫��"<<endl;
	double z=0,r=0,r1=-1000;
	cin >>z;
	double t=z+1;
	while(t>z)
	{
		hist(v);
		calv();
		r=v1[0]/v[0];
		t=fabs(r-r1);
		r1=r;
		for(i=0;i<n;i++)
			v[i]=v1[i];
	}
	cout <<"���Ϊ: " <<t<<endl<<endl;
	cout <<"����ֵ�������ֵΪ: "<<r<<endl<<endl;
	cout <<"��������Ϊ:"<<endl;
	for(i=0;i<n;i++)
		cout <<v[i]<<" ";
	cout <<endl;
	return 0;
}