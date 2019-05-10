#include<iostream>
#include<stdio.h>
#include<conio.h>
#define Pname 1
#define Var 2
using namespace std;
typedef struct{
	char var[100];
	int type;
	int value;
}VAR1;

bool ProgDef();
bool SubProg();

//变量定义
bool VarDef();
bool VarDefList();
bool VarDefList1();
bool VarDefState();
bool VarList();
bool Variable();
int Type();

//程序主体
bool CompState();
bool StateList();
bool Statement();

//赋值语句
bool AsignState();
int Expr();
int term();
int factor();
int Variable1();

//if - else
bool ISE();
bool IBT();

//while
bool WBD();

//bool表达式
bool BoolExpr();
int RelationOp();

//标识符
bool Iden();
bool Iden1();

//常量
bool Const();
bool Int();
bool Float();

bool Num();
bool Char();

//辅助函数
void copystr(char des[],char in[]);
bool judge1(char *des);
bool judge(char *des);
bool judgeiden();
void print();
void output(char first[],char second[],char third[],char fourth[]);


char constnum[1000][1000]={0};//常量
int conum=0;

char iden[1000][100]={0};//标识符
int idnum=0;

VAR1 VAR[1000];//变量
int numvar=0;

int value=0;//用于识别赋值语句语义时用到
int tempvar=0;
char strvar[100]={0};
int label=0;

char buf[2000]={0};//字符输入缓存区
int s,e=0;
int sentence=1;

FILE *save=NULL;//四元式保存位置
char savename[100]={0};

int clear_tvar=0;

void readfile()
{
	char name[100]={0};
	cout <<"input the file name:"<<endl;
	cin >>name;

	FILE *p=fopen(name,"rb");
	if(p!=NULL)
	{
		while(!feof(p)&&s<2000)
		{
			char t=fgetc(p);
			if(t==' '||t=='\n'||t=='\r'||t=='\t')
				continue;
			buf[s++]=t;
		}
		buf[s]='\0';
		e=s-1;
		s=0;
		cout <<"input the file name to save 四元式:"<<endl;
		cin >>savename;
		save=fopen(savename,"wb");
	}
	else
	{
		cout <<"the file is not exist!"<<endl;
	}
}


int main()
{
	readfile();
	ProgDef();
	fclose(save);
	return 0;
}






/////////////////////////////////////////////////////辅助函数////////////////////
void copystr(char des[],char in[])
{
	int l=strlen(in);
	for(int i=0;i<l;i++)
		des[i]=in[i];
	des[l]='\0';
}


bool judgeiden()//检测标识符
{
	char IDen[1000][100]={"While","If","Then","Do","Begin"};
	int num=4;
	for(int i=0;i<num;i++)
		if(judge1(IDen[i]))
			return true;
	return false;
}


bool judge(char * des)//匹配字符串，相同则前进
{
	int s1=s;
	int i=0;
	int l=strlen(des);
	for(i=0;i<l;i++,s1++)
	{
		if(s1>e)
			return false;
		if(buf[s1]!=des[i])
			break;
	}
//	print();
	if(i==l)
	{
		s=s1;
		return true;
	}
	else
		return false;
}

bool judge1(char * des)//匹配字符串，相同不前进
{
	int s1=s;
	int i=0;
	int l=strlen(des);
	for(i=0;i<l;i++,s1++)
	{
		if(s1>e)
			return false;
		if(buf[s1]!=des[i])
			break;
	}
	if(i==l)
	{
		return true;
	}
	else
		return false;
}
void print()//打印已经识别的字符
{
	for(int i=0;i<s;i++)
		cout <<buf[i];
	cout <<endl;
}
void output(char first[],char second[],char third[],char fourth[])//输出四元式
{
	cout <<sentence<<":( "<<first<<","<<second<<","<<third<<","<<fourth<<" )"<<endl;
	fprintf(save," %s %s %s %s \n",first,second,third,fourth);
	sentence++;
}
/********************************************************************************/


/*****************************程序大体识别***************************************/

// ProgDef:     Program Iden ';' SubProg '.'
// 最外层的程序定义，包括程序名称
bool ProgDef()
{
	if(judge("Program"))
	{
		if(Iden())
		{
			output("ProgramName",iden[idnum-1],"-","-");
			if(judge(";"))
			{
				cout <<"Program name define over"<<endl;
				if(SubProg())
				{
					cout <<"Program recognize over"<<endl;
				}
				else
				{
					cout <<"SubProg wrong!"<<endl;
				}
			}
			else
			{
				cout <<"Program define end short of ;"<<endl;
				return false;
			}
		}
		else
		{
			cout <<"Program iden wrong!"<<endl;
			return false;
		}
	}
	else
	{
		cout <<"Program define wrong!"<<endl;
		return false;
	}
	return true;
}

// SubProg:     VarDef CompState
// 子程序，包括变量定义，程序体
bool SubProg()
{
	if(VarDef())
	{
		for(int i=0;i<numvar;i++)
		{
			if(VAR[i].type==1)
				output("VarDefine",VAR[i].var,"Integer","-");
			else
				output("VarDefine",VAR[i].var,"Real","-");
		}
		cout <<"SubProg VarDef is end!"<<endl;
		print();
		if(CompState()&&judge("."))
		{
		}
		else
		{
			cout <<"CompState is wrong!"<<endl;
			return false;
		}
	}
	else
	{
		cout <<"SubProg VarDef wrong!"<<endl;
		return false;
	}
	return true;
}
/*******************************************************************/

/**********************变量定义部分的翻译**************************/

//VarDef:	     Var VarDefList ';'
//变量定义 开始符号为Var，然后是变量定义列表
bool VarDef()
{
	if(judge("Var"))
	{
		if(VarDefList())
		{
			judge(";");
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}


//VarDefList:  VarDefList';' VarDefState| VarDefState
//VarDefList:  VarDefState VarDefList1
//VarDefList1: ;VarDefState VarDefList1
//变量定义列表 包含多个变量定义式
bool VarDefList()
{
	if(VarDefState())
	{
		if(!VarDefList1())
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool VarDefList1()
{
	if(judge(";"))
	{
		if(judge1("Begin"))//遇到Begin，程序体开始符号，退出
			return false;
		if(VarDefState())
		{
			VarDefList1();
		}
		else
		{
			return false;
		}
	}
	return true;
}

//VarDefState: VarList':'Type
//变量定义式 包含变量列表和变量类型
bool VarDefState()
{
	int sv=numvar,ev=0;
	if(VarList())
	{
		ev=numvar;
		if(judge(":"))
		{
			int t=Type();
			if(t)
			{
				for(int i=sv;i<ev;i++)
					VAR[i].type=t;
			}
			else
			{
				return false;
			}
		}
		else
		{
			cout <<"short of : before var type"<<endl;
			return false;
		}
	}
	return true;
}

// Type: Integer  |  Real
// 变量种类，整型和浮点型
int Type()
{
	if(judge("Integer"))
	{
		return 1;
	}
	else if(judge("Real"))
	{
		return 2;
	}
	else
	{
		cout <<"wrong var type !"<<endl;
		return 0;
	}
}

// VarList: Variable ',' VarList  |  Variable
// 变量列表 包含多个变量，逗号隔开
bool VarList()
{
	if(Variable())
	{
		if(judge(","))
		{
			if(!VarList())
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

//Variable: Iden
//定义阶段，变量就是标识符即可
bool Variable()
{
	if(Iden())
	{
		copystr(VAR[numvar].var,iden[idnum-1]);
		numvar++;
	}
	else
	{
		cout <<"Var define var name is wrong !"<<endl;
		return false;
	}

	return true;
}
/************************************************************************/

/*******************程序主体******************/
//Begin StateList End
//以begin开始和end结束 中间为具体的语句列表
bool CompState()
{
	if(judge("Begin"))
	{
		if(StateList())
		{
			if(judge("End"))
			{

			}
			else
			{
				cout <<"CompState end is wrong!"<<endl;
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{	
		return false;
	}
	return true;
}

// StateList:   Statement  ';' StateList|  Statement 
// 语句列表 为多个语句组合在一起，以;隔开。这个;是所有语句的最后一个;
bool StateList()
{
	if(judge1("End"))
	{
		return true;
	}
	if(Statement())
	{
		if(judge(";"))
		{
			if(!StateList())
			{
				return false;
			}
		}
		else
		{
			cout <<"StateList is short of ;"<<endl;
		}
	}
	else
	{
		return false;
	}
	return true;
}

// Statement:   AsignState|ISE |WBD |CompStat
// 语句的组成，由4种基本语句够成
bool Statement()
{
	if(CompState())
	{
	}
	else if(AsignState())
	{

	}
	else if(ISE())
	{

	}
	else if(WBD())
	{

	}
	else
	{
		return false;
	}
	return true;
}
/**************************************************/

/*********************赋值语句**********************/

// AsignState:  Variable':''='Expr
// 赋值包含左边的变量和赋值符号以及右边的表达式
bool AsignState()
{	
	if(judgeiden())
	{
		return false;
	}
	int t=Variable1();
	if(t!=-1)
	{
		if(judge(":="))
		{
			int t1=Expr();
			if(!t1)
			{
				return false;
			}
			else
			{
				if(t1!=-2)
				{
					VAR[value].type=VAR[t].type;
					output(":=",VAR[t].var,VAR[value].var,"-");
				}
				else
					output(":=",VAR[t].var,strvar,"-");
			}
		}
		else
		{
			cout <<"AsignState := is wrong"<<endl;
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}

// Expr : term { +|- term}
// 表达式包含+-*/以及（）运算，通过所在位置来表达出优先级
int Expr()
{
	int n=numvar,n1=tempvar;
	char temp[100];
	int t=term();
	if(t)
	{
		if(t!=-2)
			copystr(temp,VAR[t].var);
		else
			copystr(temp,strvar);

		if(!(judge1("+")||judge1("-")))
		{
			if(t!=-2)
				copystr(strvar,VAR[t].var);
			value=numvar-1;
			if(clear_tvar==0)
			{
				numvar=n;
				tempvar=n1;
			}
			return -2;
		}
		while(judge1("+")||judge1("-"))
		{
			if(judge("+"))
			{
				int t1=term();
				if(!t1)
				{
					value=numvar-1;
					if(clear_tvar==0)
					{
						numvar=n;
						tempvar=n1;
					}
					return 0;
				}
				sprintf(VAR[numvar++].var,"VALUE%d",tempvar++);
				if(t1!=-2)
					output("+",VAR[numvar-1].var,temp,VAR[t1].var);
				else
					output("+",VAR[numvar-1].var,temp,strvar);
			}
			else if(judge("-"))
			{
				int t1=term();
				if(!t1)
				{
					value=numvar-1;
					if(clear_tvar==0)
					{
						numvar=n;
						tempvar=n1;
					}
					return 0;
				}
				sprintf(VAR[numvar++].var,"VALUE%d",tempvar++);
				if(t1!=-2)
					output("-",VAR[numvar-1].var,temp,VAR[t1].var);
				else
					output("-",VAR[numvar-1].var,temp,strvar);
			}
		}
	}
	else
	{
		value=numvar-1;
		if(clear_tvar==0)
		{
			numvar=n;
			tempvar=n1;
		}
		return 0;
	}
	value=numvar-1;
	if(clear_tvar==0)
	{
		numvar=n;
		tempvar=n1;
	}
	return 1;
}
//term : factor { *|/ factor}
//乘除项
int term()
{
	int t=factor();
	if(t)
	{
		char temp[100]={0};
		if(t==3||t==2)
			copystr(temp,strvar);
		else
			copystr(temp,VAR[value].var);

		if(!(judge1("*")||judge1("/")))
		{
			copystr(strvar,temp);
			return -2;
		}
		while(judge1("*")||judge1("/"))
		{
			if(judge("*"))
			{
				t=factor();
				if(!t)
				{
					return 0;
				}
				sprintf(VAR[numvar++].var,"VALUE%d",tempvar++);
				if(t==1)
					output("*",VAR[numvar-1].var,temp,VAR[value].var);
				else
					output("*",VAR[numvar-1].var,temp,strvar);
				copystr(temp,VAR[numvar-1].var);
			}
			else if(judge("/"))
			{
				t=factor();
				if(!t)
				{
					return 0;
				}
				sprintf(VAR[numvar++].var,"VALUE%d",tempvar++);
				if(t==1)
					output("/",VAR[numvar-1].var,temp,VAR[value].var);
				else
					output("/",VAR[numvar-1].var,temp,strvar);
				copystr(temp,VAR[numvar-1].var);
			}
		}
	}
	else
	{
		return 0;
	}
	return numvar-1;
}


// factor:  (exp)|Const|Variable
// 最基本的项或者是括号表达式
int factor()
{
	if(judge("("))
	{
		if(!Expr())
		{
			return 0;
		}
		if(!judge(")"))
		{
			return 0;
		}
		return 1;
	}
	else if(Const())
	{
		sprintf(VAR[numvar++].var,"VALUE%d",tempvar++);
		output(":=",VAR[numvar-1].var,constnum[conum-1],"-");
		copystr(strvar,VAR[numvar-1].var);
		return 2;
	}
	else if(Variable1()!=-1)
	{
		sprintf(VAR[numvar++].var,"VALUE%d",tempvar++);
		output(":=",VAR[numvar-1].var,iden[idnum-1],"-");
		copystr(strvar,VAR[numvar-1].var);
		return 3;
	}
	else
	{
		return 0;
	}
}
// Variable:    Iden
// 用于程序体中变量的检测，返回值为变量已经保存下来的位置，没有返回-1 报错
int Variable1()
{
	if(Iden())
	{
		for(int i=0;i<numvar;i++)
		{
			if(strcmp(iden[idnum-1],VAR[i].var)==0)
				return i;
		}
		cout <<"var is not define!"<<endl;
		return -1;
	}
	else
	{
		cout <<"var name is wrong !"<<endl;
		return -1;
	}
}

/*****************************************************/


/****************** if-else ****************************/
//ISE: IBT Statement Else Statement 
//if语句包含IBT前缀和语句体，以及可选的else和对应的程序体
bool ISE()
{
	if(IBT())
	{
		char temp1[100],temp2[100];
		sprintf(temp1,"label%d",label++);
		output("if",temp1,"-","-");
		if(Statement())
		{
			if(judge1(";Else"))
			{
				sprintf(temp2,"label%d",label++);
				output("Jump",temp2,"-","-");
			}
			output("label",temp1,"-","-");
			if(judge(";Else"))
			{
				if(!Statement())
				{
					return false;
				}
				output("label",temp2,"-","-");
			}		
		}
		else
		{
			cout <<"IBT statement is wrong"<<endl;
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}

// If BoolExpr Then
// if前缀包含if 判别表达式 Then 
bool IBT()
{
	if(judge("If"))
	{
		if(BoolExpr())
		{
			if(!judge("Then"))
			{
				cout <<"IBT is then wrong "<<endl;
				return false;
			}
		}
		else
		{
			cout <<"IBT bool expr is wrong"<<endl;
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}
/*****************************************************/


/******************* while ***************************/

// WBD:  While BoolExpr Do Statement 
// while语句包含while起始符 bool表达式 Do 以及子程序体
bool WBD()
{
	if(judge("While"))
	{
		char temp1[100],temp2[100];
		sprintf(temp1,"label%d",label++);
		sprintf(temp2,"label%d",label++);
		output("label",temp1,"-","-");
		if(BoolExpr())
		{
			output("if",temp2,"-","-");
			if(judge("Do"))
			{
				if(!Statement())
				{
					return false;
				}
				output("Jump",temp1,"-","-");
				output("label",temp2,"-","-");
			}
			else
			{
				cout <<"WBD do wrong"<<endl;
				return false;
			}
		}
		else
		{
			cout <<"WBD bool expr is wrong!"<<endl;
			return false;
		}
	}
	else
	{
		cout <<"WBD while is wrong"<<endl;
		return false;
	}
	return true;
}

/*****************************************************/

/***********************其余项目**************************/

//BoolExpr:  Expr RelationOp Expr
//bool表达式只包含了比较运算符
bool BoolExpr()
{
	clear_tvar=1;
	int n=numvar,n1=tempvar;	
	int t=Expr();
	char temp[100];
	if(t!=0)
	{
		if(t==-2)
			copystr(temp,strvar);
		else
			copystr(temp,VAR[value].var);
		int op=RelationOp();
		char temp1[2]="0";
		temp1[0]=op+'0';
		if(op)
		{
			clear_tvar=0;
			int t1=Expr();
			if(!t1)
			{
				clear_tvar=0;
				numvar=n,tempvar=n1;
				return false;
			}
			if(t1==-2)
				output(temp1,temp,strvar,"-");
			else
				output(temp1,temp,VAR[value].var,"-");
		}
		else
		{
			clear_tvar=0;
			numvar=n,tempvar=n1;
			return false;
		}
	}
	else
	{
		clear_tvar=0;
		numvar=n,tempvar=n1;
		return false;
	}
	clear_tvar=0;
	numvar=n,tempvar=n1;
	return true;
}

// RelationOp:  '<'  |  '>'  |  '=='|'<>'|'>='|'<='
// 关系操作符
int RelationOp()
{
	if(judge("<="))
	{
		return 1;
	}
	else if(judge(">="))
	{
		return 2;
	}
	else if(judge("=="))
	{
		return 3;
	}
	else if(judge("<>"))
	{
		return 4;
	}
	else if(judge(">"))
	{
		return 5;
	}
	else if(judge("<"))
	{
		return 6;
	}
	else
	{
		cout <<"RelationOP is wrong! "<<endl;
		return 0;
	}

}

//Const: Int  |  Float
//常量有两种，整型，浮点型
bool Const()
{
	int st=s;
	int se=0;
	if(Int())
	{
		se=s;
		for(int i=st;i<se;i++)
			constnum[conum][i-st]=buf[i];
		conum++;
	}
	else if(Float())
	{
		se=s;
		for(int i=st;i<se;i++)
			constnum[conum][i-st]=buf[i];
		conum++;
	}
	else
	{
		return false;
	}
	return true;
}

//Int: num|Int
//整型，只有数字
bool Int()
{
	if(Num())
	{
		if(!Int())
		{
			return true;
		}
	}
	else
	{
		return false;
	}
	return true;
}

//数字
bool Num()
{
	if(buf[s]>='0'&&buf[s]<='9')
	{
		s=s+1;
	}
	else
	{
		return false;
	}
	return true;
}

//float: .Int|Int.Int
bool Float()
{
	if(judge("."))
	{
		if(!Int())
		{
			return false;
		}
	}
	else
	{
		if(!Int())
		{
			return false;
		}
		if(judge("."))
		{
			if(!Int())
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}


//Iden : Char Iden1
//Iden1: Num Iden1|Char Iden1
//标识符首字符只能为字符,不能为数字
bool Iden()
{
	if(judgeiden())
	{
		return true;
	}
	int chars=0,chare=0;
	if(Char())
	{
		chars=s-1;
		Iden1();
		chare=s;
		for(int i=chars;i<chare;i++)
			iden[idnum][i-chars]=buf[i];
		idnum++;
	}
	else
	{
		return false;
	}
	return true;
}
bool Iden1()
{
	if(judgeiden())
	{
		return true;
	}
	if(Num())
	{
		Iden1();
	}
	else if(Char())
	{
		Iden1();
	}
	else
	{
		return false;
	}
	return true;
}

//字母
bool Char()
{
	if(buf[s]>='a'&&buf[s]<='z'||buf[s]>='A'&&buf[s]<='Z'||buf[s]=='_')
	{
		s=s+1;
	}
	else
	{
		return false;
	}
	return true;
}
/********************************************************************************/