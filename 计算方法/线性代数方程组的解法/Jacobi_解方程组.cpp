#include<iostream>//coefficient
#include<math.h>
using namespace std;
double **str;
int n;
void calcoe()
{
	for(int i=0;i<n;i++)
	{
		double t=str[i][i];
		for(int j=0;j<=n;j++)
		{
			if(i==j)
				str[i][j]=0;
			else if(j!=n)
				str[i][j]=str[i][j]*-1/t;
			else
				str[i][j]/=t;
		}
	}
}
double getmax(double x[],double x1[])
{
	double t=fabs(x1[0]-x[0]);
	for(int i=1;i<n;i++)
	{
		double t1=fabs(x1[i]-x[i]);
		if(t<t1)
			t=t1;
	}
	return t;
}
void calresult()
{
	double *x=new double[n];
	double *x1=new double[n];
	int i=0,j=0;
	cout <<"输入初值"<<endl;
	for(i=0;i<n;i++)
		cin >>x[i];

	double temp=0;
	cout <<"输入精度"<<endl;
	cin >>temp;
	int e=0;
	double t=temp+1;
	while(t>temp)
	{
		e++;
		for(j=0;j<n;j++)
		{
			x1[j]=0;
			for(i=0;i<n;i++)
				x1[j]+=x[i]*str[j][i];
			x1[j]+=str[j][i];
		}
		t=getmax(x,x1);

		for(i=0;i<n;i++)
			x[i]=x1[i];
	}
	cout <<"迭代次数为:"<<e<<endl;
	cout <<"方程的解为"<<endl;
	for(i=0;i<n;i++)
		cout <<x[i]<<" ";
	cout <<endl;
	cout <<"方程的误差为"<<endl;
	cout <<t<<endl;
}
int main()
{
	int i=0,j=0;
	cout <<"输入未知数的个数"<<endl;
	cin >>n;
	str=new double*[n];
	for(i=0;i<n;i++)
		str[i]=new double[n+2];
	cout <<"输入每个方程的系数和常数"<<endl;
	for(i=0;i<n;i++)
		for(j=0;j<=n;j++)
			cin >>str[i][j];
	calcoe();
	calresult();
	return 0;
}
/*
5 -1 1 10
1 -10 -2 27
-1 2 10 13
*/