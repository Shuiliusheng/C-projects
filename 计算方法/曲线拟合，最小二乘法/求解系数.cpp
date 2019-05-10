#include<iostream>
#include<stdio.h>
#include<math.h>
using namespace std;
int num=2;//the number of the fuction
double **str;
double **s;
double *strx;
/**********����***********************/

void print(int i)
{
	switch(i){
	case 0:cout <<"";break;
	case 1:cout <<"*x1";break;
	case 2:cout <<"*x2";break;
	case 3:cout <<"*x3";break;
	case 4:cout <<"*x4";break;
	case 5:cout <<"*x5";break;
	case 6:cout <<"*x6";break;
	default:cout <<"Wrong"<<endl;
	}
	
}
/*********************************/

void calmartrix(int n)
{
	int i=0,j=0,m=0;
	for(i=0;i<num;i++)
		for(j=0;j<num;j++)
		{
			double t=0;
			for(m=0;m<n;m++)
				t+=str[i][m]*str[j][m];
			s[i][j]=t;
		}
}
void calmartrix1(int n,double y[])
{
	int i,m;
	for(i=0;i<num;i++)
	{
		double t=0;
		for(m=0;m<n;m++)
			t+=str[i][m]*y[m];
		s[i][num]=t;
	}
	cout <<"�������̵�ϵ��Ϊ:"<<endl;
	for(i=0;i<num;i++)
	{
		for(m=0;m<=num;m++)
			cout <<s[i][m]<<" ";
		cout <<endl;
	}
	cout <<"***********************************"<<endl;
}
/**************���㷽����***************/
int max(int i,int d)
{
	int j=0;
	double max=-100000;
	for(int c=d;c<num;c++)
		if(s[c][i]>max)
		{
			max=s[c][i];
			j=c;
		}
	return j;
}
void exchange(int i,int j)
{
	double t;
	for(int c=0;c<num+1;c++)
	{
		t=s[i][c];
		s[i][c]=s[j][c];
		s[j][c]=t;
	}
}
void calx()
{
	int i=0;
	for(i=0;i<=num;i++)
		strx[i]=0;
	for(i=num-1;i>=0;i--)
	{
		double t=0;
		for(int j=num-1;j>i;j--)
			t=strx[j]*s[i][j]+t;
		if(s[i][i]==0)
		{
			cout <<"�޽�\n";
			return ;
		}
		strx[i]=(s[i][num]-t)/s[i][i];
	}
	cout <<"���Ϊ:\ny=";
	for(i=0;i<num;i++)
	{
		if(i==0)
			cout <<strx[i];
		else
			printf("%+lf",strx[i]);
		print(i);
	}
	cout <<endl;
}
void show()
{
	int i=0,j=0;
	cout <<"��Ԫ�����ֵ����:"<<endl;
	for(i=0;i<num;i++)
	{
		for(j=0;j<=num;j++)
			printf("%0.3lf ",s[i][j]);
		cout <<endl;
	}
	cout <<"**************************************"<<endl;
}

void calmartrixs()
{
	int i=0,j=0;
	int t=0;
	double temp=0;
	for(i=0;i<num;i++)
	{
		t=max(i,i);
		exchange(i,t);
		for(j=i+1;j<num;j++)
		{
			if(s[i][i]==0)
			{
				cout <<"Wrong"<<endl;
				return ;
			}
			double t1=s[j][i]/s[i][i];
			for(int c=i;c<=num;c++)
				s[j][c]=s[j][c]-s[i][c]*t1;
		}
	}
	cout <<endl;
	show();
	cout <<endl;
	calx();
}
/**************************************************/


int main()
{

	cout <<"�������"<<endl;
	cin >>num;
	str=new double *[num];
	double **str1=new double *[num];
	s=new double *[num];
	for(int e=0;e<num;e++)
		s[e]=new double[num+1];


	int n;
	cout <<"��������֪��ĸ���:"<<endl;
	cin >>n;
	double *x=new double[n+1];
	double *y=new double[n+1];
	double *y1=new double[n+1];
	strx=new double[num+1];
	int i=0,j=0;
	for(i=0;i<num;i++)
	{
		str[i]=new double[n+1];
		str1[i]=new double[n+1];
	}
	cout <<"��������֪�㣬��ʽx y"<<endl;
	for(i=0;i<num;i++)
	{
		cout <<"num:"<<i<<endl;
		for(j=0;j<n;j++)
		{
			cin >>str[i][j];
			str1[i][j]=str[i][j];
			str[i][j]/=10000;
			
		}
	}
	cout <<"y:"<<endl;
	for(i=0;i<n;i++)
	{
		cin>>y[i];
		y1[i]=y[i];
		y[i]/=10000;
	}
	calmartrix(n);
	calmartrix1(n,y);//��������������ĸ�����

	calmartrixs();//�������Է�����õ�ϵ��
	double c=0;
	for(i=0;i<n;i++)
	{
		double t=0;
		for(j=0;j<num;j++)
			t+=strx[j]*str1[j][i];
		c+=(t-y1[i])*(t-y1[i]);
	}
	c/=num;
	cout <<"����Ϊ: "<<c<<endl;
	for(i=0;i<num;i++)
		delete(str[i]);
	for(i=0;i<num;i++)
		delete(s[i]);
	delete(s);
	delete(str);
	delete(x);
	delete(y);
	delete(strx);
	return 0;
}

/*
0 0
1 1
2 5
5 4
8 1


1.52 150
1.55 160
1.57 170
1.61 180
1.64 190
1.67 200
1.69 210
1.73 220
*/