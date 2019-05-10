#include<iostream>
#include<string.h>
using namespace std;

void calcut(char str[],char str1[],int flag)
{
	int l=strlen(str);
	int l1=strlen(str1);
	int i=0,j=0,t=0;
	for(i=0;i<l;i++)
		str[i]=str[i]-'0';
	for(i=0;i<l1;i++)
		str1[i]=str1[i]-'0';
	for(i=l-1,j=l1-1;j>=0;j--,i--)
		str[i]=str[i]-str1[j];
	for(j=0;j<l;j++)
		if(str[j]!=0)
		    break;
	if(l==j)
	{
		cout <<0<<endl;
		return ;
	}
	if(str[j]>0)
		for(i=l-1;i>=0;i--)
		{
			str[i]=str[i]-t;
			if(str[i]<0)
			{
				if(str[i]%10!=0)
					t=str[i]*(-1)/10+1;
				else
					t=str[i]*(-1)/10;
				str[i]+=t*10;
			}
			else
				t=0;
		}
	else
		for(i=l-1;i>=0;i--)
		{
			str[i]=str[i]+t;
			if(str[i]>0)
			{		
				if(str[i]%10!=0)
					t=str[i]/10+1;
				else
					t=str[i]/10;
				str[i]-=t*10;
			}
			else
				t=0;
		}
	for(i=j+1;i<l;i++)
		if(str[i]<0)
			str[i]*=-1;
	if(flag==2)
		cout <<"-";
	for(i=j;i<l;i++)
		cout <<(int)str[i];
	cout <<endl;
}
int main()
{
	char str[10000],str1[10000];
	cin >>str;
	cin >>str1;
	int s=strlen(str);
	int s1=strlen(str1);
	if(s>=s1)
		calcut(str,str1,1);	
	else
		calcut(str1,str,2);	
	return 0;
}