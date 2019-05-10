#include<iostream>
#include<stdlib.h>
#include<string.h>
using namespace std;
int *s[10];
int *s0=NULL;
int max=0;
void cal1(int s1[],int l)
{
	for(int i=0;i<10;i++)
		for(int j=0;j<l;j++)
			s[i][j]=s1[j]*i;
}
void cal2(int s2[],int l1,int l2)
{
	int n=max;
	int i=0,j=0;
	int t=0;
	for(i=l2-1,j=0;i>=0;i--,j++)
	{
		n=max-j-1;
		for(int c=l1-1;c>=0;c--,n--)
			s0[n]=s[s2[i]][c]+s0[n];
	}
	for(i=max-1;i>=0;i--)
	{
		s0[i]+=t;
		t=s0[i]/10;
		s0[i]%=10;
	}
	for(i=0;i<max;i++)
		if(s0[i]!=0)
			break;
	if(i==max)
		cout <<0;
	for(j=i;j<max;j++)
		cout <<s0[j];
	cout <<endl;
}
int main()
{
	char str[10000],str1[10000];
	cin >>str;
	cin >>str1;
	int l1=strlen(str);
	int l2=strlen(str1);
	max=l1;
	if(max<l2)
		max=l2;
	max=max*2+1;
	int *s1=(int *)malloc(sizeof(int)*l1);
	int *s2=(int *)malloc(sizeof(int)*l2);
	s0=(int *)malloc(sizeof(int)*max);
	int i=0;
	for(i=0;i<10;i++)
		s[i]=(int *)malloc(sizeof(int)*l1);
	for(i=0;i<max;i++)
		s0[i]=0;
	for(i=0;i<l1;i++)
		s1[i]=str[i]-'0';
	for(i=0;i<l2;i++)
		s2[i]=str1[i]-'0';
	cal1(s1,l1);
	cal2(s2,l1,l2);
	return 0;
}