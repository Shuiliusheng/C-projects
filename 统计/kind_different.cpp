#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int length;
int la,lb;
double ta,tb;
double **da=NULL,**db=NULL;


int read(double **d,double &temp)
{
	FILE *p=NULL;
	char str[70];
	int n=0;
	double i=0;
	temp=0;
	printf("input the file name\n");
	scanf("%s",str);
	p=fopen(str,"rb");
	if(p==NULL)
	{
		printf("%s is not exit\n",str);
		return 0;
	}
	fscanf(p,"%lf",&i);
	char str1[4];
	str1[2]='\0';
	while(!feof(p)&&n<length)
	{
		int a=0,b=0;
		fscanf(p,"%s %lf",str1,&i);
		a=(unsigned char)str1[0]-142;
		b=(unsigned char)str1[1]-64;
		d[n][0]=a*1000+b;
		d[n][1]=i;
		temp+=i;
		n++;
	}
	if(n<length)
		printf("the data number is %d, not enough\n",length);
	fclose(p);
	return n;
}

void detect()
{
	FILE *p;
	p=fopen("same_char.txt","wb");
	double bound=0;
	printf("input the low_bound\n");
	scanf("%lf",&bound);
	int i=0,n=0,m=0;
	double t=0;
	char str[3];
	str[2]='\0';
	for(i=0;i<la;i++)
		for(n=0;n<lb;n++)
			if(da[i][0]==db[n][0])
			{
				double a=da[i][1]/ta,b=db[i][1]/tb;
				t+=pow((a-b),2);
				m++;
				str[0]=(char)((int)da[i][0]/1000)+0x8e;
				str[1]=(char)((int)da[i][0]%1000)+0x40;
				printf("No %d: %s\n",m,str);
				printf("a:%lf   b:%lf",a,b);
				printf("  差值 :%lf\n\n",fabs(a-b));
				if(fabs(a-b)<bound)
					fprintf(p,"%s ",str);
			}
	fclose(p);
	printf("\nthe same char number is :: %d\n",m);
	printf("均衡后方差(*1000) : %lf\n",(1000*t)/m);
}

int main()
{
	int i=0;
	printf("input the number of data\n");
	scanf("%d",&length);
	da=(double **)malloc(sizeof(double *)*length);
	db=(double **)malloc(sizeof(double *)*length);
	for(i=0;i<length;i++)
	{
		da[i]=(double*)malloc(sizeof(double)*2);
		db[i]=(double*)malloc(sizeof(double)*2);
	}
	la=read(da,ta);
	lb=read(db,tb);
	printf("占总数据的比值  :%lf  %lf\n\n",ta,tb);
	detect();
	system("pause");
	return 0;
}
