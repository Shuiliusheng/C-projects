#include<stdio.h>
#include<iostream>
#include<math.h>
using namespace std;
typedef struct{
	int x[3];
	double ceo[3];
}Node;
void sort(double *p,double *f,int number)
{
	double temp=0;
	for(int i=0;i<number-1;i++)
		for(int j=i;j<number;j++)
			if(p[i]>p[j])
			{
				temp=p[i];
				p[i]=p[j];
				p[j]=temp;
				temp=f[i];
				f[i]=f[j];
				f[j]=temp;
			}
}
void calceo(Node *node,double *x1,double *f,int nodenum)
{
	for(int i=0;i<nodenum;i++)
	{
		double a1,a2,a3;
		a1=  (x1[node[i].x[0]]-x1[node[i].x[1]])*(x1[node[i].x[0]]-x1[node[i].x[2]]);
		a2= (x1[node[i].x[1]]-x1[node[i].x[0]])*(x1[node[i].x[1]]-x1[node[i].x[2]]) ;
		a3= (x1[node[i].x[2]]-x1[node[i].x[0]])*(x1[node[i].x[2]]-x1[node[i].x[1]]) ;
		if(a1==0)
			a1=0.000001;
		if(a2==0)
			a2=0.000001;
		if(a3==0)
			a3=0.000001;
		node[i].ceo[0]=f[node[i].x[0]]/(a1);
		node[i].ceo[1]=f[node[i].x[1]]/(a2);
		node[i].ceo[2]=f[node[i].x[2]]/(a3);
	}
}

void calx(Node *node,int nodenum,int number)
{
	for(int i=0;i<nodenum-1;i++)
	{
		node[i].x[0]=3*i;
		node[i].x[1]=3*i+1;
		node[i].x[2]=3*i+2;
	}
	node[i].x[0]=number-3;
	node[i].x[1]=number-2;
	node[i].x[2]=number-1;
}

void calf(Node *node,double *x2,int i,double &f,double x1)
{
	double temp[3];
	temp[0]=(x1-x2[node[i].x[0]]);
	temp[1]=(x1-x2[node[i].x[1]]);
	temp[2]=(x1-x2[node[i].x[2]]);
	f=temp[2]*temp[1]*node[i].ceo[0]+temp[2]*temp[0]*node[i].ceo[1]+temp[0]*temp[1]*node[i].ceo[2];
}
int main()
{
	int number=0;int i=0;
	cout << "输入已知数据的个数"<<endl;
	cin >>number;
	double *x2=(double *)malloc(sizeof(double)*(number+1));
	double *f=(double *)malloc(sizeof(double)*(number+1));
	cout <<"输入x，y的值"<<endl;
	for(i=0;i<number;i++)
		scanf("%lf %lf",&x2[i],&f[i]);
	


	sort(x2,f,number);	
	int nodenum=0;
	if(number%3!=0)
		nodenum=number/3+1;
	else
		nodenum=number/3;
	Node *node=(Node *)malloc(sizeof(Node)*nodenum);
	calx(node,nodenum,number);
	calceo(node,x2,f,nodenum);
	
	double x1=0,f1=0;
	for(i=0;i<nodenum;i++)
		cout <<"x1: "<<node[i].x[0]<<"  x2: "<<node[i].x[1]<<"  x3: "<<node[i].x[2]<<endl;

	cout <<"输入要插值的x值: "<<endl;
    while(scanf("%lf",&x1)!=EOF)
	{
		for(i=nodenum-1;i>=0;i--)
		{
			if(i==nodenum-1&&x1>=x2[node[i].x[2]])break;
			if(i==0&&x1<=x2[node[i].x[0]])break;
			if(x1<=x2[node[i].x[2]]&&x1>=x2[node[i].x[0]])break;
		}
		calf(node,x2,i,f1,x1);
		cout <<"result : "<<f1<<endl<<endl;
		cout <<"输入要插值的x值: "<<endl;
	}
	return 0;
	
	
}
/*
5
-1 10
-2 20
0 1
1 3
2 7
*/