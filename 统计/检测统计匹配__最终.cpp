#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int num1=0,num2=0;//num1��Ҫ���ַ��ĸ���   num2 ģ��ĸ���

double **detect;//�洢��ģ������ȡ���ַ���Ƶ��
double **data;//�洢�ļ��е��ַ���Ƶ��
char **d_file;//�洢ģ�������
double **result;//�洢��Ӧ��ÿ��ģ��Ľ������ͬ�ַ��ĸ�����Ƶ��

int useless[300];
int num_u=0;///��¼�����ַ��ĸ���
int add_flag=1;//�Ƿ������ģ��ı�־λ


int read_useless()//��ȡ��Ӧ���ַ�
{
	FILE *p=NULL;
	char str[70];
	int a=0,b=0;
	scanf("%s",str);
	p=fopen(str,"rb");
	if(p==NULL)
	{
		printf("Wrong name or the file is not exit\n");
		return 0;
	}
	while(!feof(p)&&num_u<300)
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
	return 1;
}

///��ȡ�ı����ݣ�ͳ�ƺ���,d_name�ı�����c�洢���ֵ�����,c1�洢���ظ����ַ�
//flag���ڼ�¼���ظ��ַ��ĸ�����n_h�ַ�����
int read_data(char d_name[],int c[][191],int c1[],int &flag,double &n_h)
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

///���򣬽�����д���ļ�na�У�d1���ڼ�¼Ƶ���ܺͣ� naΪfilename��num3��¼��ȥ�����ַ���ʵ�ʶ�ȡ�ĸ���
void sort_write(int c[][191],int c1[],double n_h,int flag,double &d1,char na[],int &num3)
{
	char str[10];
	str[2]='\0';
	int e=0,a=0,b=0,n=0;
	FILE *q;			
	q=fopen(na,"wb");
	double temp=0;
	fprintf(q,"%lf    \n",n_h);
	int i=0,num2=num1;
	for(i=0,e=0;e<num2;e++)
	{
		if(num2>=flag)
			break;
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

		for(n=0;n<num_u;n++)///�ж��Ƿ���������
			if(c1[e]==useless[n])
				break;

		if(n<num_u&&num_u!=0)//�ǵģ�ֱ�ӽ�����һ��
		{
			num2++;
			continue;
		}
		a1=c1[e]/1000;
	    b1=c1[e]%1000;
		str[0]=(char)a1+0x8e;
		str[1]=(char)b1+0x40;

		temp=(double)c[a1][b1]/n_h;//n_h������
		fprintf(q,"%s %lf \n",str,temp);
		data[i][0]=c1[e];//ʲô��
		data[i][1]=temp;//��ռƵ��
		i++;
		d1+=temp;
	}
	num3=i;
	printf("num3: %d\n",num3);
	fclose(q);
}

//��ȡģ���ļ���Ϣ�����ҽ��м�¼
//nameģ�������,d2��¼ģ���ܵ�Ƶ������
void read(char name[],double &d2)
{
	int i=0,a=0,b=0;
	double temp=0;
	FILE *p=NULL;
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
	fclose(p);
	
}

///���бȽϼ�⣬�õ������ÿ��ģ�������
//p�����Ӧ�ڵڼ���ģ��ļ����

void detect_file(int p,double d1,double d2,int num3)
{
	double f=0;
	int n=0;
	for(int i=0;i<num3;i++)
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
	result[p][0]=n,result[p][1]=f/n;

}

int autodetect()///����ģ�����ݵıȽϵĵ��������ͬ�ֵĸ����ͷ����ռһ��Ȩ��
{
	int i=0;
	int t=0;
	double temp=100,n=0,e=0;
	for(i=0;i<num2;i++)
		n+=result[i][0],e+=result[i][1];

	n=num2*num1-n;//������ͬ�ĸ�����ʹ�ò���ͬ�ĸ���ռ����ı�ֵ����Ϊ����ʹ�ã��뷽��ѡ����Сֵ���Ӧ

	double c1=0.5,c2=0.5;//Ȩ��ϵ����

	for(i=0;i<num2;i++)
	{
		double c=c1*(num1-result[i][0])/n+c2*(result[i][1]/e);//�õ�����ϵ��
		if(c<temp)
		{
			temp=c;
			t=i;
		}
	}
	printf("\nis more like     c=%lf,       %s\n",temp,d_file[t]);
	return t;
}

//��ȡģ���������c�����У�Ȼ����д��µ������⣬�洢
void add_data(int r,int c[][191],int c1[],double n_h,int flag,double d1)
{
	FILE *w=NULL;
	w=fopen(d_file[r],"rb");
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
	sort_write(c,c1,n_h,flag,d1,d_file[r],a);
}

//ѡ��ʹ���ĸ�ģ��
void add(int flag,int r,int c[][191],int c1[],double n_h,int flag1,double d1)
{
	if(add_flag==0)
		return ;
	if(flag==0)
	{
label3: printf("choose the more similar file,(1,2...)\n");
		printf("if not,input 0\n");
		scanf("%d",&r);
		if(r==0)
			return ;
		r-=1;
		if(r>=num2)
		{
			printf("wrong input\n");
			goto label3;
		}
	}
	add_data(r,c,c1,n_h,flag1,d1);
	printf("successfully!\n");
}


int main()
{
	int i=0,c=0,n=0,flag=0,flag_auto=0;
	char file[70];
	FILE *p=NULL;

	printf("choose read from the text(1) or read directly(2)\n");
	scanf("%d",&flag);
	if(flag!=1&&flag!=2)
	{
		printf("wrong input\n");
		return 0;
	}

	printf("input the file name\n");
	scanf("%s",file);
	p=fopen(file,"rb");
	if(p==NULL)
	{
		printf("the file is not exit!\n");
		return 0;
	}


	printf("input the number of char to use\n");
	scanf("%d",&num1);
	data=(double **)malloc(sizeof(double *)*num1);
	for(i=0;i<num1;i++)
		data[i]=(double *)malloc(sizeof(double)*2);
		detect=(double **)malloc(sizeof(double *)*num1);
	for(i=0;i<num1;i++)
		detect[i]=(double *)malloc(sizeof(double)*2);

	printf("input the number of the model file\n");
	scanf("%d",&num2);
	d_file=(char **)malloc(sizeof(char *)*num2);
	result=(double **)malloc(sizeof(double)*num2);
	for(i=0;i<num2;i++)
	{
		d_file[i]=(char *)malloc(sizeof(char)*70);
		result[i]=(double *)malloc(sizeof(double)*2);
		printf("input No %d:",i+1);
		scanf("%s",d_file[i]);
	}
	
	printf("the useless char file name\n");
	if(!read_useless())
		return 0;
	
	printf("auto detect:(Y 1,N 0)\n");
	scanf("%d",&flag_auto);
	
	printf("add into the moban ?(Y 1,N 0)\n");
	scanf("%d",&add_flag);

	if(flag==1)//read from the text
	{
		char file1[70];
		while(!feof(p))
		{
			FILE *q=NULL;
			fscanf(p,"%s",file1);
			q=fopen(file1,"rb");
			if(q==NULL)
			{
				printf("the %s is not exit\n",file1);
				continue;
			}
			else
			{
				int r=0;
				double n_h=0;
				int num3=0;
				double d1=0,d2=0;
				int flag=0;
				int c[113][191]={0};
				int c1[20000];
				char str[100];
				sprintf(str,"detect_%s.txt",file1);

				read_data(file1,c,c1,flag,n_h);
				
				sort_write(c,c1,n_h,flag,d1,str,num3);
				
				for(i=0;i<num2;i++)
				{
					read(d_file[i],d2);
					
					detect_file(i,d1,d2,num3);
					
					d2=0;
				}
				printf("%s",file1);
				r=autodetect();
				add(flag_auto,r,c,c1,n_h,flag,d1);
				
			}
		}
		fclose(p);
	}
	else//read directly
	{
		fclose(p);
		int r=0,num3=0;
		double n_h=0;
		double d1=0,d2=0;
		int flag=0;
		int c[113][191]={0};
		int c1[20000];
		read_data(file,c,c1,flag,n_h);
		char str[100];
		sprintf(str,"detect_%s.txt",file);
		sort_write(c,c1,n_h,flag,d1,str,num3);

		for(i=0;i<num2;i++)
		{
			read(d_file[i],d2);
			detect_file(i,d1,d2,num3);
			d2=0;
		}
		printf("%s",file);
		r=autodetect();
		add(flag_auto,r,c,c1,n_h,flag,d1);
	}


	for(i=0;i<num1;i++)//�ͷ�������ڴ�
		free(detect[i]),free(data[i]);
	for(i=0;i<num2;i++)
		free(d_file[i]),free(result[i]);
	free(data),free(detect),free(d_file),free(result);
	system("pause");
	return 0;
}