#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int num1=0,num2=0;//num1需要的字符的个数   num2 模板的个数

double **detect;//存储从模板中提取的字符及频率
double **data;//存储文件中的字符及频率
char **d_file;//存储模板的名字
double **result;//存储对应与每个模板的结果，相同字符的个数及频率

int useless[300];
int num_u=0;///记录无用字符的个数
int add_flag=1;//是否添加入模板的标志位


int read_useless()//读取相应的字符
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

///读取文本数据，统计汉字,d_name文本名，c存储汉字的数组,c1存储不重复的字符
//flag用于记录不重复字符的个数，n_h字符总数
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

///排序，将数据写入文件na中，d1用于记录频率总和， na为filename，num3记录除去无用字符后实际读取的个数
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

		for(n=0;n<num_u;n++)///判断是否是无用字
			if(c1[e]==useless[n])
				break;

		if(n<num_u&&num_u!=0)//是的，直接进行下一步
		{
			num2++;
			continue;
		}
		a1=c1[e]/1000;
	    b1=c1[e]%1000;
		str[0]=(char)a1+0x8e;
		str[1]=(char)b1+0x40;

		temp=(double)c[a1][b1]/n_h;//n_h总字数
		fprintf(q,"%s %lf \n",str,temp);
		data[i][0]=c1[e];//什么字
		data[i][1]=temp;//所占频率
		i++;
		d1+=temp;
	}
	num3=i;
	printf("num3: %d\n",num3);
	fclose(q);
}

//读取模板文件信息，并且进行记录
//name模板的名字,d2记录模板总的频率总数
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

///进行比较检测，得到相对于每个模板的数据
//p代表对应于第几个模板的检测结果

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

int autodetect()///进行模板数据的比较的到结果，相同字的个数和方差各占一半权重
{
	int i=0;
	int t=0;
	double temp=100,n=0,e=0;
	for(i=0;i<num2;i++)
		n+=result[i][0],e+=result[i][1];

	n=num2*num1-n;//对于相同的个数，使用不相同的个数占总体的比值，作为参数使用，与方差选择最小值相对应

	double c1=0.5,c2=0.5;//权重系数，

	for(i=0;i<num2;i++)
	{
		double c=c1*(num1-result[i][0])/n+c2*(result[i][1]/e);//得到最后的系数
		if(c<temp)
		{
			temp=c;
			t=i;
		}
	}
	printf("\nis more like     c=%lf,       %s\n",temp,d_file[t]);
	return t;
}

//读取模板的数据至c数组中，然后进行从新的排序检测，存储
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

//选择使用哪个模板
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


	for(i=0;i<num1;i++)//释放申请的内存
		free(detect[i]),free(data[i]);
	for(i=0;i<num2;i++)
		free(d_file[i]),free(result[i]);
	free(data),free(detect),free(d_file),free(result);
	system("pause");
	return 0;
}