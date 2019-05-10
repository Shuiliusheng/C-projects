/* �������,win-tc��Dev-c++�µ���ͨ�� */ 
#include<stdio.h> 
#include<string.h> 
#include<stdlib.h>
/* �������� */ 
void calc1(char* str1,int len1,int* tmp,int m); 
void accumulate(int cnt,int* res,int res_len,int* tmp,int tmp_len); 
char* bignum_multi(char* str1,int len1,char* str2,int len2,char* result,int len); 

int main() 
{ 
int i,j; 
/* ��ȡ��������(���Դ��ļ��ж�ȡ) */ 
char str1[10000]="123456789987"; 
char str2[10000]="987456123"; 
char* result; 

gets(str1);
gets(str2);


/* ���������ַ����ĳ���,���洢�������Ҫ�Ŀռ� */ 
int len1=strlen(str1),len2=strlen(str2); 
int len=len1+len2; 

/* ���䲢��ʼ���ַ������� */ 
result=(char*)malloc(len*sizeof(char)); 
for(i=0;i<len;i++) 
*(result+i)='0'; 

/* ���㲢��������� */ 
printf("The result is: %s",bignum_multi(str1,len1,str2,len2,result,len)); 
free(result); 
system("pause"); 
return 0; 
} 

/*=============================================================== 
����calc1��accumulate�������������� 
===============================================================*/ 
char* bignum_multi(char* str1,int len1,char* str2,int len2,char* result,int len) 
{ 
int i,j,m=0,cnt=0,*tmp,*res; 

/* ������ʱ����Ĵ�ſռ� */ 
tmp=(int*)malloc((len1+1)*sizeof(int)); 
res=(int*)malloc(len*sizeof(int)); 

/* ��ʼ���������� */ 
for(i=0;i<len1;i++) 
tmp[i]=0; 
for(j=0;j<len;j++) 
res[j]=0; 

for(i=len2-1;i>=0;i--) 
{ 
/* ��ȡ�����е�iλ��ֵ */ 
m=str2[i]-'0'; 
/* ���㱻�������iλ�ĳ˻�,���������tmp���������� */ 
calc1(str1,len1,tmp,m); 
/* ��tmp�����е�ֵ�ӵ�res������ */ 
cnt++; 
accumulate(cnt,res,len,tmp,len1+1); 
} 

/* ����������res�е�ֵת�����ַ�������result�� */ 
i=0;j=0; 
/* ȥ��res�е�һ����������ǰ���� */ 
while(res[i++]==0); 

for(m=i-1;m<len;m++,j++) 
result[j]=res[m]+0x30; 
result[j]='\0'; 

free(tmp); 
free(res); 
return result; 
} 

/*=============================================================== 
���㱻�����������ĳһλ�ĳ˻� 
===============================================================*/ 
void calc1(char* str1,int len1,int* tmp,int m) 
{ 
/* d����λ�ĳ˻����,remainder����,carry��λ */ 
int i,d=0,remainder=0,carry=0; 
/* �ӱ������ַ���'\0'��ǰһλ���� */ 
for(i=len1-1;i>=0;i--) 
{ 
d=str1[i]-'0'; 
d*=m; 
remainder=(d+carry)%10; 
carry=(d+carry)/10; 
tmp[i+1]=remainder; 
} 
if(carry) 
tmp[0]=carry; 
else 
tmp[0]=0; 
} 
/*=============================================================== 
���������������һλ���ֵĳ˻��������res������ 
===============================================================*/ 
void accumulate(int cnt,int* res,int len,int* tmp,int len1) 
{ 
int m=0,n=0,i,k,remainder=0; 
static int carry=0; 
for(k=len1-1,i=0;k>=0;k--,i++) 
{ 
m=tmp[k]; 
n=res[len-cnt-i]; 
if(m+n+carry>=10) 
{ 
remainder=(m+n+carry)%10; 
carry=1; 
} 
else 
{ 
remainder=m+n+carry; 
carry=0; 
} 
res[len-cnt-i]=remainder; 
} 
}