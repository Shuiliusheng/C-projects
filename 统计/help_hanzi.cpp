#include<stdio.h>
int main()
{
	char str[10]="了";
	int choices=0;
	int h_num=1;
	printf("input the choices:(0:to_num/1:to_str)\n");
	scanf("%d",&choices);
	if(choices==0)
		printf("input 0 over\n");
	else
		printf("input -1 over\n");





	while(str[0]!='0'&&choices==0)
	{
		int a=(unsigned char)str[0]-0x8e;
		int b=(unsigned char)str[1]-0x40;
		int a1=(unsigned char)str[0];
		int b1=(unsigned char)str[1];
		printf("gbk十六进制: %x%x\n",a1,b1);
		printf("gbk十进制: %d\n",a1*256+b1);
		b=a*1000+b;
		printf("%s:%d\n",str,b);
		scanf("%s",str);
	}




	str[2]='\0';
	while(h_num!=-1&&choices==1)
	{
		scanf("%d",&h_num);
		int a=h_num/1000;
		int b=h_num%1000;
		str[0]=(char)a+0x8e;
		str[1]=(char)b+0x40;
		printf("%d: %s\n",h_num,str);
	}
	return 0;
}