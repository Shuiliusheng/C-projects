#include<string.h>
#include<stdio.h>
int c[113][191]={0};
int c1[20000];
int flag=0;
double n_h=0;
int num1=0;
int useless[100];
int num_u=0;

void read_useless()
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


void copy3()//将文件2中的数据覆盖3
{
	FILE *p,*q;
	p=fopen("2.txt","rb");
	char str1[70];
	printf("input the name to reserve\n");
	scanf("%s",str1);
	q=fopen(str1,"wb");
	double n=0;
	double m=0;
	char str[10];
	fscanf(p,"%lf ",&n);
	fprintf(q,"%lf ",n);
	while(!feof(p))
	{
		fscanf(p,"%s %lf ",str,&m);
		fprintf(q,"%s %lf ",str,m);
	}
	fclose(p);
	fclose(q);
	printf("Success\n");
	remove("2.txt");
}

void add_old_data()//读取以前的数据，即记录在3中的
{
	FILE *w=NULL;
	char str1[70];
	printf("input the name to read\n");
	scanf("%s",str1);
	w=fopen(str1,"rb");
	if(w==NULL)
		return ;

	int a=0,b=0,m1=0;
	double m=0;
	char str[10];
	double num=0;
	fscanf(w,"%lf   ",&num);
	
	while(!feof(w))
	{	
		fscanf(w,"%s %lf",str,&m);
		a=(unsigned char)str[0]-142;
		b=(unsigned char)str[1]-64;
		m1=m*num;
		if(c[a][b]==0)
		{
			c1[flag++]=a*1000+b;
		}
	    c[a][b]=c[a][b]+m1;
	}
	n_h+=num;
	fclose(w);
	printf("read successfully\n");
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
	a=0;
	printf("%d read successfully\n",++a);
	return 1;
}

void sort_write()///排序，将数据写入文件2中
{
	char str[10];
	str[2]='\0';
	int e=0,a=0,b=0,n=0;
	FILE *q;			
	q=fopen("2.txt","wb");
	

	printf("the total:%lf\n",n_h);
	printf("the kind: %d\n\n",flag); 

	double temp=0;
	fprintf(q,"%lf    \n",n_h);
	for(e=0;e<num1;e++)
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
			num1++;
			continue;
		}
		a1=c1[e]/1000;
	    b1=c1[e]%1000;
		str[0]=(char)a1+0x8e;
		str[1]=(char)b1+0x40;
		temp=(double)c[a1][b1]/n_h;
		printf("%s: %lf\n",str,temp);
		fprintf(q,"%s %lf \n",str,temp);
	}
	fclose(q);
}

int main()
{	

	FILE *p=NULL;
	char str1[70];
	char str[70];
	printf("input the name to read\n");
	scanf("%s",str1);
	p=fopen(str1,"rb");

	if(p==NULL){
		printf("Wrong!,the File is not exit\n");
		return 0;
	}

	printf("input the num:\n");
	scanf("%d",&num1);
	while(!feof(p))
	{
		fscanf(p,"%s",str);
		read_data(str);	
	}

	printf("add the old file?  Yes: 1 No: 0\n");
	int i=0;
	scanf("%d",&i);
	if(i==1)
		add_old_data();
	printf("cut the useless char?  Yes: 2 No: 0\n");
	scanf("%d",&i);
	if(i==2)
		read_useless();
	sort_write();
	copy3();
	fclose(p);
	return 0;
}
