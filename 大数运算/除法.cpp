#include<iostream>
#include<string.h>
using namespace std;
int *s[10];
int *r;
int a=2;
void cals(int str[],int l)
{
	for(int i=0;i<10;i++)
	{
		int t=0;
		for(int c=l-1;c>=0;c--)
		{
			s[i][c+1]=str[c]*i+t;
			t=s[i][c+1]/10;
			s[i][c+1]%=10;
		}
		s[i][0]=t;
	}
}
int charge(int str[],int s,int str1[],int l)
{
	for(int i=0;i<=l;i++)
		if(str[s+i]>str1[i])
			return 1;
		else if(str[s+i]<str1[i])
			return 0;
		else
			continue;
	return 1;
}
void cut(int str[],int s,int str1[],int l)
{
	for(int i=0;i<=l;i++)
		str[s+i]-=str1[i];
	int t=0;
	for(int j=l;j>=0;j--)
	{
		str[s+j]-=t;
		if(str[j+s]<0)
		{
			if(str[j+s]%10!=0)
				t=str[j+s]*(-1)/10+1;
			else
				t=str[j+s]*(-1)/10;
			str[j+s]+=t*10;
		}
		else
			t=0;
	}
}
void end(int r[],int n)
{
	int i=0,j=0,t=0;
	if(r[n-1]>4)
		t=1;
	else
		t=0;
	for(i=n-2;i>=0;i--)
	{
		r[i]+=t;
		t=r[i]/10;
		r[i]%=10;
	}
	for(j=0;j<n-1;j++)
		if(r[j]!=0)
			break;
	if(n-1-j<=a)
	{
		cout <<"0.";
		for(i=0;i<a-n+1+j;i++)
			cout <<0;
		for(i=j;i<n-1;i++)
			cout <<r[i];
		cout <<endl;
	}
	else
	{
		for(i=j;i<n-1;i++)
		{
			if(n-i-1==a)			
				cout <<".";
			cout <<r[i];
		}
		cout <<endl;
	}
}
void divi(int str[],int str1[],int s1,int s2)
{
	int i=0,j=0;
	int n=0;
	for(i=s2,j=0;i<s1;i++,j++)
		for(int c=9;c>=0;c--)
			if(charge(str,j,s[c],s2))
			{
				cut(str,j,s[c],s2);
				r[n++]=c;	
				break;
			}
	end(r,n);
}
int main()
{
	char str1[10000],str2[10000];
	int i=0,j=0;
	cin >>str1;
	cin >>str2;
	cout <<"¾«¶È\n";
	cin >>a;
	int s1=strlen(str1);
	int s2=strlen(str2);
	int max=s1;
	if(max<s2)
		max=s2;
	max=max+a+1;

	int *l1=(int *)malloc(sizeof(int)*(s1+2+a));
	int *l2=(int *)malloc(sizeof(int)*(s2+1));
	r=(int *)malloc(sizeof(int)*(max));
	for(int c=0;c<max;c++)
		r[c]=0;
	for(i=0;i<10;i++)
		s[i]=(int *)malloc(sizeof(int)*(s2+1));
	for(i=1;i<=s1;i++)
		l1[i]=str1[i-1]-'0';
	for(i=s1+1;i<s1+2+a;i++)
		l1[i]=0;
	l1[0]=0;
	s1=s1+2+a;
	for(i=0;i<s2;i++)
		l2[i]=str2[i]-'0';
	cals(l2,s2);
	divi(l1,l2,s1,s2);
	return 0;
}