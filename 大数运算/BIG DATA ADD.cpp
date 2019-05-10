#include<iostream>
#include<string.h>
using namespace std;
void caladd(char str[],char str1[])
{
	int i=0,j=0;
	int t=0;
	int s1=strlen(str);
	int s2=strlen(str1);
	for(i=0;i<s1;i++)
		str[i]=str[i]-'0';
	for(i=0;i<s2;i++)
		str1[i]=str1[i]-'0';
	for(i=s2-1,j=s1-1;i>=0;i--,j--)
			str[j]=str[j]+str1[i];	
	for(i=s1-1;i>=0;i--)
	{
		str[i]+=t;
		t=str[i]/10;
		str[i]%=10;
	}
	if(t!=0)
	cout <<t;
	for(i=0;i<s1;i++)
		cout <<(int)str[i];
	cout <<endl;
}
int main()
{
	char str[10000],str1[10000];
	cin >>str;
	cin >>str1;
	int s1=strlen(str);
	int s2=strlen(str1);
	if(s1>=s2)
		caladd(str,str1);
	else 
		caladd(str1,str);
	return 0;
}