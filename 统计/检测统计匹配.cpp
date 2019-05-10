#include<string.h>
#include<stdio.h>
#include<stdlib.h>
int c[113][191]={0};
int c1[20000];
int flag=0;
double n_h=0;
int num1=0;
double **detect;
double **data;
double d1=0,d2=0;

int useless[100];
int num_u=0;

void read_useless()//读取字符相应的数字
{
	FILE *p=NULL;
	char str[70];
	printf("input the useless char text name\n");
	scanf("%s",str);
	p=fopen(str,"rb");
	if(p==NULL)
	{
		printf("Wrong name or the file is not exit\n");
		return ;
	}
	while(!feof(p)&&num_u<100)
	{
		fscanf(p,"%d",&useless[num_u++]);
	}
	fclose(p);
	printf("the useless char number: %d \n",num_u);
	printf("read successfully!\n\n");

}
void read_useless1()//读取相应的字符
{
	FILE *p=NULL;
	char str[70];
	int a=0,b=0;
	printf("input the useless char text name\n");
	scanf("%s",str);
	p=fopen(str,"rb");
	if(p==NULL)
	{
		printf("Wrong name or the file is not exit\n");
		return ;
	}
	while(!feof(p)&&num_u<100)
	{
		fscanf(p,"%s",str);
		a=(unsigned char)str[0]-142;
		b=(unsigned char)str[1]-64;
		a=a*1000+b;
		useless[num_u++]=a;
	}
	fclose(p);
	printf("the useless char number: %d \n",num_u);
	printf("read successfully!\n\n");

}

int read_data(char d_name[])///读取文本数据，统计汉字
{
	FILE *p=NULL;
	int a=0,b=0;
	unsigned char s;
	char str[3];
	str[2]='\0';
	p=fopen(d_name,"rb");
	if(p==NULL){
		printf("read wrong,%s is not exit\n",d_name);
		return 0;
	}
	while(!feof(p))
	{	
		fscanf(p,"%c",str);
		s=str[0];
		if(s>=0x8e&&s<=0xff)
		{
			a=(int)s-142;
			fscanf(p,"%c",str+1);
			s=str[1];
			if(s>0x40&&s<0xff)
			{
				b=(int)s-64;
				c[a][b]++;
				n_h++;
				if(c[a][b]==1)
					c1[flag++]=a*1000+b;
			}
		}
		
	}
	fclose(p);
	return 1;
}

void sort_write()///排序，将数据写入文件2中
{
	char str[10];
	str[2]='\0';
	int e=0,a=0,b=0,n=0;
	FILE *q;			
	q=fopen("detectlrc.txt","wb");
	

	printf("the total:%lf\n",n_h);
	printf("the kind: %d\n\n",flag); 

	double temp=0;
	fprintf(q,"%lf    \n",n_h);
	int i=0,num2=num1;
	for(i=0,e=0;e<num2;e++)
	{
		int a1=0;
	    int b1=0;
		for(n=e;n<flag;n++)
		{
			a=c1[n]/1000;
			b=c1[n]%1000;
			a1=c1[e]/1000;
			b1=c1[e]%1000;
			if(c[a][b]>c[a1][b1])
			{
				int t=c1[e];
		        c1[e]=c1[n],c1[n]=t;
			}
		}
		for(n=0;n<num_u;n++)
			if(c1[e]==useless[n])
				break;
		if(n<num_u&&num_u!=0)
		{
			num2++;
			continue;
		}
		a1=c1[e]/1000;
	    b1=c1[e]%1000;
		str[0]=(char)a1+0x8e;
		str[1]=(char)b1+0x40;
		temp=(double)c[a1][b1]/n_h;
		printf("%s: %lf\n",str,temp);
		fprintf(q,"%s %lf \n",str,temp);
		data[i][0]=c1[e];
		data[i][1]=temp;
		i++;
		d1+=temp;
	}
	fclose(q);
}

void read()
{
	int i=0,a=0,b=0;
	double temp=0;
	FILE *p=NULL;
	char name[70];
	printf("input the name\n");
	scanf("%s",name);
	p=fopen(name,"rb");
	if(p==NULL)
	{
		printf("the detect file read wrong!\n");
		return ;
	}
	fscanf(p,"%lf ",&temp);
	char str[4];
	for(i=0;i<num1;i++)
	{
		fscanf(p,"%s %lf",str,&temp);
		a=(unsigned char)str[0]-142;
		b=(unsigned char)str[1]-64;
		detect[i][0]=a*1000+b;
		detect[i][1]=temp;
		d2+=temp;
	}
	printf("d1:%lf d2:%lf\n",d1,d2);
	fclose(p);
	
}


void detect_file()
{
	double f=0;
	int n=0;
	for(int i=0;i<num1;i++)
	{
		for(int j=0;j<num1;j++)
		{
			if(data[i][0]==detect[j][0])
			{
				n++;
				f+=(detect[j][1]/d2-data[i][1]/d1)*(detect[j][1]/d2-data[i][1]/d1)*1000;
				break;
			}
		}
	}
	printf("相同个数: %d   方差(*1000): %lf    比值(后/前): %lf\n",n,f,f/n);
}
int main()
{
	char str[70];
	int n=0,i=0;

	printf("input the name:\n");
	gets(str);
	printf("input the number:\n");
	scanf("%d",&num1);
	data=(double **)malloc(sizeof(double *)*num1);
	for(i=0;i<num1;i++)
		data[i]=(double *)malloc(sizeof(double)*2);

	read_data(str);
	printf("cut the useless char?  Yes: 1 No: 0\n");
	scanf("%d",&i);
	if(i==1)
		read_useless1();
	sort_write();
	
	printf("input the times:\n");
	scanf("%d",&n);

	detect=(double **)malloc(sizeof(double *)*num1);
	for(i=0;i<num1;i++)
		detect[i]=(double *)malloc(sizeof(double)*2);

	while(n--!=0)
	{
		read();
		detect_file();
		d2=0;
	}

	for(i=0;i<num1;i++)
		free(detect[i]),free(data[i]);
	free(data),free(detect);
	return 0;
}