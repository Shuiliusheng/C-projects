#include<iostream>
#include<stdlib.h>
using namespace std;
double *x=NULL;//�����Ա���
double *f=NULL;//����ϵ��
double *t=NULL;//�洢��ʱ�����������ظ�����
void calfirst(int n)//����ϵ��
{
	for(int i=1;i<n;i++)
	{
		double t=f[i-1];
		for(int j=i;j<n;j++)
		{
			double c=f[j]-t;
			t=f[j];
			f[j]=c/(x[j]-x[j-i]);
		}
	}
}
void result(double temp,int n)//�Ա���Ϊtemp�ǵĽ��
{
	double a=1;
	for(int i=0;i<n-1;i++)
	{
		t[i]=(temp-x[i])*a;
		a=t[i];
	}
	double r=0;
	a=1;
	for(int j=0;j<n;j++)
	{
		r=r+a*f[j];
		a=t[j];
	}
	cout <<"���\n";
	cout <<r<<endl;
}
void cal(int c)//����
{
	int n;
	cout <<endl<<"����������ݵĸ���\n";
	cin >>n;
	while(n--!=0)
	{
		double temp=0;
		cout <<"input:"<<endl;
		cin >>temp;
		result(temp,c);
	}
}
int main()
{
	int n;
	int i=0,j=0;
	cout <<"������֪�ڵ����\n";
	cin >>n;
	x=new double[n+2];
	f=new double[n+2];
	t=new double[n+2];
	cout <<"������Ӧ��xֵ��yֵ���ո����\n";
	for(i=0;i<n;i++)
		cin >>x[i]>>f[i];
	calfirst(n);//����ϵ��
	cal(n);//�����ֵ���
	delete x;
	delete f;
	delete t;
	return 0;
}