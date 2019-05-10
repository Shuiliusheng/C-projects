#include<iostream>
#include<stdlib.h>
#include<math.h>
using namespace std;
int n;
double **str;
double **str1;
double *strx;
int max(int i,int d)
{
	int j=0;
	double max=-100000;
	for(int c=d;c<n;c++)
		if(str[c][i]>max)
		{
			max=str[c][i];
			j=c;
		}
	return j;
}
void exchange(int i,int j)
{
	double t;
	for(int c=0;c<=n;c++)
	{
		t=str[i][c];
		str[i][c]=str[j][c];
		str[j][c]=t;
	}
}
void calwucha()
{
	double max=-1000;
	for(int i=0;i<n;i++)
	{
		double t=0;
		for(int j=0;j<n;j++)
			t+=str1[i][j]*strx[j];
		t=fabs(str1[i][j]-t);
		if(t>max)
			max=t;
	}
	cout <<"误差为:"<<max<<endl;
}
void calx()
{
	int i=0;
	for(i=0;i<=n;i++)
		strx[i]=0;
	for(i=n-1;i>=0;i--)
	{
		double t=0;
		for(int j=n-1;j>i;j--)
			t=strx[j]*str[i][j]+t;
		if(str[i][i]==0)
		{
			cout <<"无解\n";
			return ;
		}
		strx[i]=(str[i][n]-t)/str[i][i];
	}
	cout <<"结果为:\n";
	for(i=0;i<n;i++)
		cout <<strx[i]<<" ";
	cout <<endl;
	calwucha();
}
void show()
{
	int i=0,j=0;
	for(i=0;i<n;i++)
	{
		for(j=0;j<=n;j++)
			printf("%0.3lf ",str[i][j]);
		cout <<endl;
	}
}
void calmartrix()
{
	int i=0,j=0;
	int t=0;
	double temp=0;
	for(i=0;i<n;i++)
	{
		t=max(i,i);
		cout <<"t:"<<t<<endl;
		exchange(i,t);
		for(j=i+1;j<n;j++)
		{
			if(str[i][i]==0)
			{
				cout <<"Wrong"<<endl;
				return ;
			}
			double t1=str[j][i]/str[i][i];
			for(int c=i;c<=n;c++)
			{
				//if(i==0)
				//	cout <<t1<<" "<<str[j][c]<<" "<<str[i][c]*t1<<endl;
				str[j][c]=str[j][c]-str[i][c]*t1;
			}
		}
	}
	cout <<"消元后的矩阵\n";
	show();
	calx();
}
/*
4 3 2 1
3 2 1 4
1 2 3 4
*/
int main()
{
	cout <<"输入未知数个数\n:";
	cin >>n;
	int i=0,j=0;
	strx=(double *)malloc(sizeof(double)*(n+1));
	str=(double **)malloc(sizeof(double *)*n);
	str1=(double **)malloc(sizeof(double *)*n);
	cout <<"输入各个系数"<<endl;
	for(i=0;i<n;i++)
	{
		str[i]=(double *)malloc(sizeof(double)*(n+2));
		str1[i]=(double *)malloc(sizeof(double)*(n+1));
		for(j=0;j<=n;j++)
		{
			cin >>str[i][j];
			str1[i][j]=str[i][j];
		}
	}
	calmartrix();
	return 0;
}