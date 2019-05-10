#include<stdio.h>
#include<iostream>
using namespace std;

typedef struct{
	int index;
	char name[20];
	char type[20];
}VAR;

VAR Var[100];
int numvar=0;

char savename[100];
int reg=10;

int baseplace=0;


void copystr(char des[],char in[])//复制字符串
{
	int l=strlen(in);
	for(int i=0;i<l;i++)
		des[i]=in[i];
	des[l]='\0';
}


int judge(char t[])//判断变量为什么类型，如果为临时变量，返回-1，变量返回存储的位置，否则返回-2
{
	char temp[20]={0};
	copystr(temp,t);
	temp[strlen(t)-1]='\0';
	if(strcmp(temp,"VALUE")==0)
	{
		return -1;
	}
	else
	{
		for(int i=0;i<numvar;i++)
		{
			if(strcmp(t,Var[i].name)==0)
				return i;
		}
	}
	return -2;
}

void translate()
{
	char name[100];
	cout <<"输入四元式的文件名:"<<endl;
	cin >>savename;
	FILE *r=fopen(savename,"rb");
	FILE *w=NULL;
	while(!feof(r))
	{
		char t1[20]={0},t2[20]={0},t3[20]={0},t4[20]={0};
		fscanf(r," %s %s %s %s ",t1,t2,t3,t4);
		if(strcmp("ProgramName",t1)==0)//程序的名称
		{
			sprintf(name,"%s.asm",t2);
			w=fopen(name,"wb");
			continue;
		}
		else if(strcmp("FunctionOver",t1)==0)
		{
			printf("jr $31\n");
			fprintf(w,"jr $31\n");
		}
		else if(strcmp("PUSH",t1)==0)
		{
			int num=0;
			sscanf(t2,"%d",&num);
			for(int i=0;i<num;i++)
			{
				char temp[20]={0},reg1[20];
				sprintf(temp,"%d($%d)",(numvar+i)*4,baseplace);
				int treg=i;
				treg+=reg;
				sprintf(reg1,"$%d",treg);
				printf("sw %s,%s\n",reg1,temp);
				fprintf(w,"sw %s,%s\n",reg1,temp);
			}		
		}
		else if(strcmp("POP",t1)==0)
		{
			int num=0;
			sscanf(t2,"%d",&num);
			for(int i=0;i<num;i++)
			{
				char temp[20]={0},reg1[20];
				sprintf(temp,"%d($%d)",(numvar+i)*4,baseplace);
				int treg=i;
				treg+=reg;
				sprintf(reg1,"$%d",treg);
				printf("lw %s,%s\n",reg1,temp);
				fprintf(w,"lw %s,%s\n",reg1,temp);
			}
		}
		else if(strcmp("JFuc",t1)==0)
		{
			printf("jal %s\n",t2);
			fprintf(w,"jal %s\n",t2);
		}
		else if(strcmp("&=",t1)==0)
		{
			int n1=judge(t2);
			int n2=judge(t3);
			Var[n1].index=Var[n2].index;	
		}
		else if(strcmp("in",t1)==0||strcmp("out",t1)==0)//输入输出语句
		{
			char reg1[10];
			int treg=0;
			sscanf(t2,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg1,"$%d",treg);
			printf("%s %s,%s\n",t1,reg1,t3);
			fprintf(w,"%s %s,%s\n",t1,reg1,t3);
		}
		else if(strcmp("VarDefine",t1)==0)//变量定义的翻译，只是用于保存变量，确定变量的存储器中的偏移位置
		{
			Var[numvar].index=numvar*4;
			copystr(Var[numvar].name,t2);
			copystr(Var[numvar].type,t3);
		//	cout <<Var[numvar].name<<" :"<<Var[numvar].type<<":"<<Var[numvar].index<<endl;
			numvar++;
			continue;
		}
		else if((strcmp("*",t1)==0))//乘法
		{
			char reg1[10],reg2[10],reg3[10];
			int treg=0;
			sscanf(t2,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg1,"$%d",treg);
			
			sscanf(t3,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg2,"$%d",treg);

			sscanf(t4,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg3,"$%d",treg);
			printf("mult %s,%s\n",reg2,reg3);
			printf("mflo %s\n",reg1);/////取乘法之后的低32位
			fprintf(w,"mult %s,%s\n",reg2,reg3);
			fprintf(w,"mflo %s\n",reg1);
		}
		else if((strcmp("/",t1)==0))//除法
		{
			char reg1[10],reg2[10],reg3[10];
			int treg=0;
			sscanf(t2,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg1,"$%d",treg);
			
			sscanf(t3,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg2,"$%d",treg);

			sscanf(t4,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg3,"$%d",treg);
			printf("div %s,%s\n",reg2,reg3);
			printf("mflo %s\n",reg1);/////////取除法之后的结果，
			fprintf(w,"div %s,%s\n",reg2,reg3);
			fprintf(w,"mflo %s\n",reg1);
		}
		else if((strcmp("+",t1)==0))//加法
		{
			char reg1[10],reg2[10],reg3[10];
			int treg=0;
			sscanf(t2,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg1,"$%d",treg);
			
			sscanf(t3,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg2,"$%d",treg);

			sscanf(t4,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg3,"$%d",treg);
			printf("add %s,%s,%s\n",reg1,reg2,reg3);
			fprintf(w,"add %s,%s,%s\n",reg1,reg2,reg3);
		}
		else if((strcmp("-",t1)==0))//减法
		{
			char reg1[10],reg2[10],reg3[10];
			int treg=0;
			sscanf(t2,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg1,"$%d",treg);
			
			sscanf(t3,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg2,"$%d",treg);

			sscanf(t4,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg3,"$%d",treg);
			printf("sub %s,%s,%s\n",reg1,reg2,reg3);
			fprintf(w,"sub %s,%s,%s\n",reg1,reg2,reg3);
		}
		else if(strcmp(":=",t1)==0)//赋值表达式的翻译
		{
			int result=judge(t2);
			//cout <<t2<<" :"<<result<<endl;
			if(result==-1)//赋值左式为临时变量
			{
				char reg1[20];
				int treg=0;
				sscanf(t2,"VALUE%d",&treg);
				treg+=reg;
				sprintf(reg1,"$%d",treg);
				
				if(t3[0]=='-'||(t3[0]<='9'&&t3[0]>='0'))//赋值为给临时变量const
				{
					printf("sub %s,%s,%s\n",reg1,reg1,reg1);
					printf("addi %s,%s,%s\n",reg1,reg1,t3);
					fprintf(w,"sub %s,%s,%s\n",reg1,reg1,reg1);
					fprintf(w,"addi %s,%s,%s\n",reg1,reg1,t3);
				}
				else//赋值为加载变量至寄存器中，load
				{
					char temp[20]={0};
					int t=judge(t3);
					if(t>=0)
					{
						sprintf(temp,"%d($%d)",Var[t].index,baseplace);
						printf("lw %s,%s\n",reg1,temp);
						fprintf(w,"lw %s,%s\n",reg1,temp);
					}
					else
						cout <<"wrong!!"<<endl;
				}
			}
			else if(result>=0)//赋值为保存变量，由临时变量回到变量中
			{
				char temp[20]={0};
				sprintf(temp,"%d($%d)",Var[result].index,baseplace);
				char reg1[20];
				int treg=0;
				sscanf(t3,"VALUE%d",&treg);
				treg+=reg;
				sprintf(reg1,"$%d",treg);
				printf("sw %s,%s\n",reg1,temp);
				fprintf(w,"sw %s,%s\n",reg1,temp);
			}
			else
				cout <<"wrong!"<<endl;
		}
		else if(strcmp("label",t1)==0)//设置标签的翻译
		{
			printf("%s:\n",t2);
			fprintf(w,"%s:\n",t2);
		}
		else if(strcmp("Jump",t1)==0)//跳转翻译
		{
			printf("j %s\n",t2);
			fprintf(w,"j %s\n",t2);
		}
		else if(t1[0]<='9'&&t1[0]>='0')//布尔表达式的翻译，后面的if跳转也会直接翻译
		{
			char t5[20]={0},t6[20]={0},t7[20]={0},t8[20]={0};
			fscanf(r," %s %s %s %s ",t5,t6,t7,t8);

			char reg1[10],reg2[10];
			int treg=0;
			sscanf(t2,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg1,"$%d",treg);
			sscanf(t3,"VALUE%d",&treg);
			treg+=reg;
			sprintf(reg2,"$%d",treg);

			int op=0;
			sscanf(t1,"%d",&op);
			
			if(op==3)//==  beq equal to jump
			{
				printf("bne %s,%s,%s\n",reg1,reg2,t6);
				fprintf(w,"bne %s,%s,%s\n",reg1,reg2,t6);
			}
			else if(op==4)//<>
			{
				printf("beq %s,%s,%s\n",reg1,reg2,t6);
				fprintf(w,"beq %s,%s,%s\n",reg1,reg2,t6);
			}
			else
			{
				printf("sub %s,%s,%s\n",reg1,reg1,reg2);
				fprintf(w,"sub %s,%s,%s\n",reg1,reg1,reg2);
				
				if(op==1)//<=  bgtz 大于时跳转
				{
					printf("bgtz %s,%s\n",reg1,t6);
					fprintf(w,"bgtz %s,%s\n",reg1,t6);
				}
				else if(op==2)//>= bltz 小于时跳转
				{
					printf("bltz %s,%s\n",reg1,t6);
					fprintf(w,"bltz %s,%s\n",reg1,t6);
				}
				else if(op==5)//> 
				{
					printf("blez %s,%s\n",reg1,t6);
					fprintf(w,"blez %s,%s\n",reg1,t6);
				}
				else if(op==6)//<
				{
					printf("bgez %s,%s\n",reg1,t6);
					fprintf(w,"bgez %s,%s\n",reg1,t6);
				}
				else
				{
					cout <<"wrong!"<<endl;
				}
			}
		}
		else
			continue;
	}
	cout <<"输出文件已经被保存在"<<name<<endl;
	fclose(r);
	fclose(w);
}

int main()
{
	translate();
	return 0;
}