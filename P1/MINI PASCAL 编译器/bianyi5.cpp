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

//��������
bool VarDef();
bool VarDefList();
bool VarDefList1();
bool VarDefState();
bool VarList();
bool Variable();
int Type();

//��������
bool CompState();
bool StateList();
bool Statement();

//��ֵ���
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

//bool���ʽ
bool BoolExpr();
int RelationOp();

//��ʶ��
bool Iden();
bool Iden1();

//����
bool Const();
bool Int();
bool Float();

bool Num();
bool Char();

//��������
void copystr(char des[],char in[]);
bool judge1(char *des);
bool judge(char *des);
bool judgeiden();
void print();
void output(char first[],char second[],char third[],char fourth[]);


char constnum[1000][1000]={0};//����
int conum=0;

char iden[1000][100]={0};//��ʶ��
int idnum=0;

VAR1 VAR[1000];//����
int numvar=0;

int value=0;//����ʶ��ֵ�������ʱ�õ�
int tempvar=0;
char strvar[100]={0};
int label=0;

char buf[2000]={0};//�ַ����뻺����
int s,e=0;
int sentence=1;

FILE *save=NULL;//��Ԫʽ����λ��
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
		cout <<"input the file name to save ��Ԫʽ:"<<endl;
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






/////////////////////////////////////////////////////��������////////////////////
void copystr(char des[],char in[])
{
	int l=strlen(in);
	for(int i=0;i<l;i++)
		des[i]=in[i];
	des[l]='\0';
}


bool judgeiden()//����ʶ��
{
	char IDen[1000][100]={"While","If","Then","Do","Begin"};
	int num=4;
	for(int i=0;i<num;i++)
		if(judge1(IDen[i]))
			return true;
	return false;
}


bool judge(char * des)//ƥ���ַ�������ͬ��ǰ��
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

bool judge1(char * des)//ƥ���ַ�������ͬ��ǰ��
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
void print()//��ӡ�Ѿ�ʶ����ַ�
{
	for(int i=0;i<s;i++)
		cout <<buf[i];
	cout <<endl;
}
void output(char first[],char second[],char third[],char fourth[])//�����Ԫʽ
{
	cout <<sentence<<":( "<<first<<","<<second<<","<<third<<","<<fourth<<" )"<<endl;
	fprintf(save," %s %s %s %s \n",first,second,third,fourth);
	sentence++;
}
/********************************************************************************/


/*****************************�������ʶ��***************************************/

// ProgDef:     Program Iden ';' SubProg '.'
// �����ĳ����壬������������
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
// �ӳ��򣬰����������壬������
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

/**********************�������岿�ֵķ���**************************/

//VarDef:	     Var VarDefList ';'
//�������� ��ʼ����ΪVar��Ȼ���Ǳ��������б�
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
//���������б� ���������������ʽ
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
		if(judge1("Begin"))//����Begin�������忪ʼ���ţ��˳�
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
//��������ʽ ���������б�ͱ�������
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
// �������࣬���ͺ͸�����
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
// �����б� ����������������Ÿ���
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
//����׶Σ��������Ǳ�ʶ������
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

/*******************��������******************/
//Begin StateList End
//��begin��ʼ��end���� �м�Ϊ���������б�
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
// ����б� Ϊ�����������һ����;���������;�������������һ��;
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
// ������ɣ���4�ֻ�����乻��
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

/*********************��ֵ���**********************/

// AsignState:  Variable':''='Expr
// ��ֵ������ߵı����͸�ֵ�����Լ��ұߵı��ʽ
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
// ���ʽ����+-*/�Լ��������㣬ͨ������λ�����������ȼ�
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
//�˳���
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
// �����������������ű��ʽ
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
// ���ڳ������б����ļ�⣬����ֵΪ�����Ѿ�����������λ�ã�û�з���-1 ����
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
//if������IBTǰ׺������壬�Լ���ѡ��else�Ͷ�Ӧ�ĳ�����
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
// ifǰ׺����if �б���ʽ Then 
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
// while������while��ʼ�� bool���ʽ Do �Լ��ӳ�����
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

/***********************������Ŀ**************************/

//BoolExpr:  Expr RelationOp Expr
//bool���ʽֻ�����˱Ƚ������
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
// ��ϵ������
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
//���������֣����ͣ�������
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
//���ͣ�ֻ������
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

//����
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
//��ʶ�����ַ�ֻ��Ϊ�ַ�,����Ϊ����
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

//��ĸ
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