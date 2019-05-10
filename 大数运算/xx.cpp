/* 大数相乘,win-tc和Dev-c++下调试通过 */ 
#include<stdio.h> 
#include<string.h> 
#include<stdlib.h>
/* 函数声明 */ 
void calc1(char* str1,int len1,int* tmp,int m); 
void accumulate(int cnt,int* res,int res_len,int* tmp,int tmp_len); 
char* bignum_multi(char* str1,int len1,char* str2,int len2,char* result,int len); 

int main() 
{ 
int i,j; 
/* 获取计算数据(可以从文件中读取) */ 
char str1[10000]="123456789987"; 
char str2[10000]="987456123"; 
char* result; 

gets(str1);
gets(str2);


/* 计算两个字符串的长度,及存储结果所需要的空间 */ 
int len1=strlen(str1),len2=strlen(str2); 
int len=len1+len2; 

/* 分配并初始化字符串数组 */ 
result=(char*)malloc(len*sizeof(char)); 
for(i=0;i<len;i++) 
*(result+i)='0'; 

/* 计算并输出计算结果 */ 
printf("The result is: %s",bignum_multi(str1,len1,str2,len2,result,len)); 
free(result); 
system("pause"); 
return 0; 
} 

/*=============================================================== 
调用calc1和accumulate函数计算大数相乘 
===============================================================*/ 
char* bignum_multi(char* str1,int len1,char* str2,int len2,char* result,int len) 
{ 
int i,j,m=0,cnt=0,*tmp,*res; 

/* 分配临时结果的存放空间 */ 
tmp=(int*)malloc((len1+1)*sizeof(int)); 
res=(int*)malloc(len*sizeof(int)); 

/* 初始化两个数组 */ 
for(i=0;i<len1;i++) 
tmp[i]=0; 
for(j=0;j<len;j++) 
res[j]=0; 

for(i=len2-1;i>=0;i--) 
{ 
/* 获取乘数中第i位的值 */ 
m=str2[i]-'0'; 
/* 计算被乘数与第i位的乘积,结果保存在tmp整型数组中 */ 
calc1(str1,len1,tmp,m); 
/* 将tmp数组中的值加到res数组中 */ 
cnt++; 
accumulate(cnt,res,len,tmp,len1+1); 
} 

/* 将整形数组res中的值转化成字符串存入result中 */ 
i=0;j=0; 
/* 去掉res中第一个非零数字前的零 */ 
while(res[i++]==0); 

for(m=i-1;m<len;m++,j++) 
result[j]=res[m]+0x30; 
result[j]='\0'; 

free(tmp); 
free(res); 
return result; 
} 

/*=============================================================== 
计算被乘数与乘数的某一位的乘积 
===============================================================*/ 
void calc1(char* str1,int len1,int* tmp,int m) 
{ 
/* d两个位的乘积结果,remainder余数,carry进位 */ 
int i,d=0,remainder=0,carry=0; 
/* 从被乘数字符串'\0'的前一位算起 */ 
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
将被乘数与乘数中一位数字的乘积结果计入res数组中 
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