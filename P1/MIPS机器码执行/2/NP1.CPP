#include<iostream>
#include<stdio.h>
using namespace std;
char inst[33];
int FUNCT=0;
int OP=0;
int shemt=0;
int rs=0,rt=0,rd=0;
unsigned int im16=0;
unsigned int im26=0;
unsigned int HI=0;
unsigned int LO=0;
unsigned int refile[32]={0};
char Mem_inst[102400][9]={0};
unsigned int Mem_data[102400]={0};

int PC=0;
int start_inst=3072;
unsigned int inst_place=0;
unsigned int data_place=0;
int single_step=0;//0:continue   1:single


unsigned int ElseA=0;
unsigned int A=0;
unsigned int B=0;
unsigned int Alu_out=0;
unsigned int Mem=0;
unsigned int Reg=0;

unsigned int Alu_op=0;
unsigned int Alu_a=0;
unsigned int Alu_b=0;
unsigned int Ext_op=0;
unsigned int PC_sel=0;
unsigned int Reg_sel=0;
unsigned int Reg_addr=0;
unsigned int Mem_addr=0;

bool Write_reg=false;
bool Write_PC=false;
bool Write_Mem=false;

bool BEQ=false;
bool BNE=false;

#define NPC 1
#define RS 2
#define SHEMT 7
#define ELSE 8

#define RT 3
#define IM 4

#define MEM 5
#define ALU 6
#define Unuse -1


#define UNSIGNEXT 1
#define SIGNEXT 2

#define F_R 0  //R型指令

#define F_IN 50
#define F_OUT 51

#define IN_HEX 1
#define IN_DEC 2
#define IN_CHAR 3

#define OUT_CHAR 3
#define OUT_DEC 2
#define OUT_HEX 1
#define OUT_UDEC 4



#define F_LW 35  //加载
#define F_LB 32
#define F_LBU 36
#define F_LH 33
#define F_LHU 37

#define F_SW 43		//保存
#define F_SB 40
#define F_SH 41

#define F_ADDI 8    //立即数
#define F_ADDUI 9
#define F_ORI 13
#define F_ANDI 12
#define F_XORI 14
#define F_SLTI 10
#define F_SLTIU 11
#define F_LUI 15

#define F_BEQ 4   //比较
#define F_BNE 5
#define F_BLEZ 6
#define F_BGTZ 7
#define F_BGEZ 1 

#define F_J 2
#define F_JAL 3
#define OP_JALR 9
#define OP_JR 8


#define OP_ADD 32
#define OP_ADDU 33
#define OP_SUB 34
#define OP_SUBU 35

#define OP_DIV 26
#define OP_DIVU 27
#define OP_MULT 24
#define OP_MULTU 25

#define OP_MFHI 16
#define OP_MFLO 18

#define OP_SLT 42
#define OP_SLTU 43

#define OP_AND 36
#define OP_NOR 39
#define OP_OR 37
#define OP_XOR 38

#define OP_SLL 0
#define OP_SLLV 4
#define OP_SRL 2
#define OP_SRLV 6
#define OP_SRA 3
#define OP_SRAV 7 


void Execute();
void debug_look();
void load_inst()
{
	char name[100];
	cout <<"input the instruction file name"<<endl;
	cin >>name;
	FILE *p=NULL;
	p=fopen(name,"rb");
	if(p!=NULL)
	{
		inst_place=start_inst;
		PC=inst_place;
		while(!feof(p))
			fscanf(p,"%s",Mem_inst[inst_place++]);
		if(inst_place>=102400)
			cout <<"error"<<endl;
	}
	cout <<"instruction number is: "<<inst_place-start_inst<<endl;
	fclose(p);
}

void read()//read the command
{
	int a=0;
	if(single_step==5)
	{
		single_step=1;
		debug_look();
	}
	if(single_step==1)
	{	
		cout <<"input 1 继续 ; 2 查看内存 3 直接执行 4 结束执行:";
		cin >>a;
		if(a==2)
			single_step=5;
		if(a==3)
			single_step=0;
		printf("PC : %u  0x%08x \n",PC<<2,PC<<2);
		if(a==4)
			PC=inst_place;
	}
	
	for(int i=7;i>=0;i--)
	{
		int temp=0;
		if(Mem_inst[PC][i]<='9')
			temp=Mem_inst[PC][i]-'0';
		else
			temp=Mem_inst[PC][i]-'a'+10;
		int temp1=i*4;
		for(int n=3;n>=0;n--)
		{
			inst[temp1+n]=temp%2+'0';
			temp/=2;
		}
		inst[32]='\0';
	}
	PC++;
}

void imm_ext(int choice)
{
	im16=0;
	int i=0;
	switch(choice){
	case UNSIGNEXT:
	{
		for(i=16;i<=31;i++)
		{
			im16*=2;
			im16+=(inst[i]-'0');
		}
	}
	case SIGNEXT:
	{
		for(i=0;i<16;i++)
		{
			im16*=2;
			im16+=(inst[16]-'0');
		}
		for(i=16;i<=31;i++)
		{
			im16*=2;
			im16+=(inst[i]-'0');
		}
	}
	default:{}
	}
}


void translate()
{
	int i=0;
	FUNCT=0;
	for(i=0;i<6;i++)
	{
		FUNCT*=2;
		FUNCT+=(inst[i]-'0');
	}
	OP=0;
	for(i=26;i<=31;i++)
	{
		OP*=2;
		OP+=(inst[i]-'0');
	}
	
	shemt=0;
	for(i=21;i<=25;i++)
	{
		shemt*=2;
		shemt+=(inst[i]-'0');
	}
	
	rs=0;rt=0;rd=0;
	for(i=0;i<5;i++)
	{
		rs*=2,rt*=2,rd*=2;
		rs+=(inst[6+i]-'0');
		rt+=(inst[11+i]-'0');
		rd+=(inst[16+i]-'0');
	}

	
	im26=0;
	
	for(i=6;i<=31;i++)
	{
		im26*=2;
		im26+=(inst[i]-'0');
	}
}

int Alu()
{
	unsigned int Alu_out=0;
	switch(Alu_op){
		case OP_ADDU:
		case OP_ADD:Alu_out=A+B;break;
		case OP_SUB:
		case OP_SUBU:Alu_out=A-B;break;
		case OP_AND:Alu_out=A&B;break;	
		case OP_OR:Alu_out=A|B;break;
		case OP_NOR:Alu_out=~(A&B);break;
		case OP_XOR:Alu_out=A^B;break;
		case OP_SLT:Alu_out=((int)A<(int)B)?1:0;break;
		case OP_SLTU:Alu_out=(A<B)?1:0;break;
		case OP_SLL:Alu_out=B<<A;break;
		case OP_SLLV:
		{
			int temp=A%32;
			Alu_out=B<<temp;break;
		}
		case OP_SRL:Alu_out=B>>A;break;
		case OP_SRLV:
		{
			int temp1=A%32;
			Alu_out=B>>temp1;break;
		}
		case OP_SRA:
		{
			int t=B<<(32-A);
			Alu_out=t+B>>A;break;
		}
			
		case OP_SRAV:
		{
			int temp=A%32;
			int t=B<<(32-temp);
			Alu_out=t+B>>temp;break;
		}
		case OP_MULT:
		{
			double t=4294967296;
			double temp=(int)A*(int)B;
			HI=(unsigned int)(temp/t);
			LO=(unsigned int)(temp-HI*t);
			break;
		}
		case OP_MULTU:
		{
			double t=4294967296;
			double temp=A*B;
			HI=(unsigned int)(temp/t);
			LO=(unsigned int)(temp-HI*t);
			break;
		}
		case OP_DIV:
		{
			if(B==0)
			{
				cout <<"除数为零"<<endl;
				break;
			}
			LO=(unsigned int)((int)A/(int)B);	
			HI=(unsigned int)((int)A%(int)B);
			break;
		}
		case OP_DIVU:
		{
			if(B==0)
			{
				cout <<"除数为零"<<endl;
				break;
			}
			LO=A/B;	
			HI=A%(int)B;
			break;
		}
		case OP_MFHI:Alu_out=HI;break;
		case OP_MFLO:Alu_out=LO;break;
		default:{}
	}
	return Alu_out;
	
}

void translate1()
{
	switch(FUNCT){
		case F_R:{
			switch(OP){
				case OP_JALR:{
					Alu_op=Unuse;
					Ext_op=Unuse;
					PC_sel=RS;
					Reg_sel=NPC;
					Reg_addr=rd;
					Write_reg=true;
					Write_PC=true;
					Write_Mem=false;
					BEQ=false;
					BNE=false;
					break;
				}
				case OP_JR:{
					Alu_op=Unuse;
					Ext_op=Unuse;
					PC_sel=RS;
					
					Write_reg=false;
					Write_PC=true;
					Write_Mem=false;
					BEQ=false;
					BNE=false;
					break;
				}
				default:{
					Write_reg=true;
					Write_PC=false;
					Write_Mem=false;
					
					Alu_op=OP;
					Alu_a=RS;
					Alu_b=RT;
					Ext_op=Unuse;
					Reg_sel=ALU;
					Reg_addr=rd;
				}
			}
			break;
		}
		case F_IN:{
			switch(shemt){
				case IN_DEC:{
					unsigned int t=0;
					char t1=0;
					scanf("%d%c",&t,&t1);
					refile[rd]=t;
					break;
				}
				case IN_HEX:{
					unsigned int t=0;
					char t1=0;
					scanf("%x%c",&t,&t1);
					refile[rd]=t;
					break;
				}
				case IN_CHAR:{
					unsigned int t=0,t1=0;
					scanf("%c%c",&t,&t1);
					refile[rd]=t;
					break;
				}
				default:{}
			}
			break;
		}
		case F_OUT:{
			switch(shemt){
				case OUT_CHAR:{
					printf("%c",refile[rd]%256);
					break;
				}
				case OUT_DEC:{
					printf("%d",refile[rd]);
					break;
				}
				case OUT_HEX:{
					printf("%x",refile[rd]);
					break;
				}
				case OUT_UDEC:{
					printf("%u",refile[rd]);
					break;
				}
				default:{}
			}
			break;
		}
		case F_LW:{

			Write_reg=false;
			Write_PC=false;
			Write_Mem=false;
			
			Alu_op=OP_ADD;
			Ext_op=SIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Execute();
			Mem_addr=Alu_out;
			Mem_addr=Mem_addr>>2;
			Mem=Mem_data[Mem_addr];
			
			Write_reg=true;
			Write_PC=false;
			Write_Mem=false;
			Alu_op=Unuse;
			Ext_op=Unuse;
			Reg_sel=MEM;
			Reg_addr=rt;
			break;
		}
		case F_LUI:{
			Write_reg=true;
			Write_PC=false;
			Write_Mem=false;
			Alu_op=OP_SLL;
			Ext_op=UNSIGNEXT;
			Alu_a=ELSE;
			Alu_b=IM;
			Reg_addr=rt;
			Reg_sel=ALU;
			ElseA=16;
			break;
		}
		case F_SW:{
			Write_reg=false;
			Write_PC=false;
			Write_Mem=false;
			
			Alu_op=OP_ADD;
			Ext_op=SIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Execute();
			Mem_addr=Alu_out;
			Mem_addr=Mem_addr>>2;
			Reg=refile[rt];
			
			Write_reg=false;
			Write_PC=false;
			Write_Mem=true;
			Alu_op=Unuse;
			Ext_op=Unuse;
			break;
		}
		case F_LB:{
			Write_reg=false;
			Write_PC=false;
			Write_Mem=false;
			
			Alu_op=OP_ADD;
			Ext_op=SIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Execute();
			Mem_addr=Alu_out;
			int n=Mem_addr%4;
			Mem_addr=Mem_addr>>2;
			Mem=Mem_data[Mem_addr]<<(24-8*n);
			Mem=Mem>>24;
			unsigned int temp=0;
			unsigned int t=Mem>>7;
			if(t==1)
			{
				for(int i=0;i<24;i++)
				{
					temp*=2;
					temp+=t;
				}
				Mem+=(temp*256);
			}
			
			Write_reg=true;
			Write_PC=false;
			Write_Mem=false;
			Alu_op=Unuse;
			Ext_op=Unuse;
			Reg_sel=MEM;
			Reg_addr=rt;

			break;
		}
		case F_LBU:{
			Write_reg=false;
			Write_PC=false;
			Write_Mem=false;
			
			Alu_op=OP_ADD;
			Ext_op=SIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Execute();
			Mem_addr=Alu_out;
			int n=Mem_addr%4;
			Mem_addr=Mem_addr>>2;
			Mem=Mem_data[Mem_addr]<<(24-8*n);
			Mem=Mem>>24;

			Write_reg=true;
			Write_PC=false;
			Write_Mem=false;
			Alu_op=Unuse;
			Ext_op=Unuse;
			Reg_sel=MEM;
			Reg_addr=rt;
			break;
		}
		case F_LH:{
			Write_reg=false;
			Write_PC=false;
			Write_Mem=false;
			
			Alu_op=OP_ADD;
			Ext_op=SIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Execute();
			Mem_addr=Alu_out;
			int n=Mem_addr/2;
			n%=2;
			Mem_addr=Mem_addr>>2;
			Mem=Mem_data[Mem_addr]<<(16-16*n);
			Mem=Mem>>16;
			unsigned int temp=0;
			unsigned int t=Mem>>15;
			if(t==1)
			{
				for(int i=0;i<16;i++)
				{
					temp*=2;
					temp+=t;
				}
				Mem+=(temp*65536);
			}

			Write_reg=true;
			Write_PC=false;
			Write_Mem=false;
			Alu_op=Unuse;
			Ext_op=Unuse;
			Reg_sel=MEM;
			Reg_addr=rt;

			break;
		}
		case F_LHU:{
			Write_reg=false;
			Write_PC=false;
			Write_Mem=false;
			
			Alu_op=OP_ADD;
			Ext_op=SIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Execute();
			Mem_addr=Alu_out;
			int n=Mem_addr/2;
			n%=2;
			Mem_addr=Mem_addr>>2;
			Mem=Mem_data[Mem_addr]<<(16-16*n);
			Mem=Mem>>16;

			Write_reg=true;
			Write_PC=false;
			Write_Mem=false;
			Alu_op=Unuse;
			Ext_op=Unuse;
			Reg_sel=MEM;
			Reg_addr=rt;

			break;
		}
		case F_SB:{
			Write_reg=false;
			Write_PC=false;
			Write_Mem=false;
			
			Alu_op=OP_ADD;
			Ext_op=SIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Execute();
			Mem_addr=Alu_out;
			int n=Mem_addr%4;
			unsigned int temp=255;
			Mem_addr=Mem_addr>>2;
			temp=temp<<(8*n);

			Reg=Mem_data[Mem_addr]|temp;
			Reg-=temp;
			temp=refile[rt]%256;
			temp=temp<<(8*n);
			Reg=Reg+temp;

			Write_reg=false;
			Write_PC=false;
			Write_Mem=true;
			Alu_op=Unuse;
			Ext_op=Unuse;
			break;
		}
		case F_SH:{
			Write_reg=false;
			Write_PC=false;
			Write_Mem=false;
			
			Alu_op=OP_ADD;
			Ext_op=SIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Execute();
			Mem_addr=Alu_out;
			int n=Mem_addr/2;
			n%=2;
			unsigned int temp=65535;
			Mem_addr=Mem_addr>>2;
			temp=temp<<(16*n);
			Reg=Mem_data[Mem_addr]|temp;
			Reg-=temp;
			temp=refile[rt]%65536;
			temp=temp<<(16*n);
			Reg=Reg+temp;

			Write_reg=false;
			Write_PC=false;
			Write_Mem=true;
			Alu_op=Unuse;
			Ext_op=Unuse;
			break;
		}
		case F_ADDUI:
		case F_ADDI:{
			Write_reg=true;
			Write_PC=false;
			Write_Mem=false;
			Alu_op=OP_ADD;
			Ext_op=SIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Reg_sel=ALU;
			Reg_addr=rt;
			break;
		}
		case F_ANDI:{
			Write_reg=true;
			Write_PC=false;
			Write_Mem=false;
			Alu_op=OP_AND;
			Ext_op=UNSIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Reg_sel=ALU;
			Reg_addr=rt;
			break;
		}
		case F_ORI:{
			Write_reg=true;
			Write_PC=false;
			Write_Mem=false;
			Alu_op=OP_OR;
			Ext_op=UNSIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Reg_sel=ALU;
			Reg_addr=rt;
			break;
		}
		case F_XORI:{
			Write_reg=true;
			Write_PC=false;
			Write_Mem=false;
			Alu_op=OP_XOR;
			Ext_op=UNSIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Reg_sel=ALU;
			Reg_addr=rt;
			break;
		}
		case F_SLTI:{
			Write_reg=true;
			Write_PC=false;
			Write_Mem=false;
			Alu_op=OP_SLT;
			Ext_op=SIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Reg_sel=ALU;
			Reg_addr=rt;
			break;
		}
		case F_SLTIU:{
			Write_reg=true;
			Write_PC=false;
			Write_Mem=false;
			Alu_op=OP_SLTU;
			Ext_op=SIGNEXT;
			Alu_a=RS;
			Alu_b=IM;
			Reg_sel=ALU;
			Reg_addr=rt;
			break;
		}
		case F_BEQ:{
			Write_reg=false;
			Write_PC=false;
			Write_Mem=false;
			
			Alu_op=OP_ADD;
			Ext_op=SIGNEXT;
			Alu_a=NPC;
			Alu_b=IM;
			Execute();

			Write_reg=false;
			Write_PC=true;
			Write_Mem=false;
			Alu_op=OP_SUB;
			Ext_op=Unuse;
			Alu_a=RS;
			Alu_b=RT;
			BEQ=true;
			BNE=false;
			PC_sel=Unuse;
			break;
		}
		case F_BNE:{
			Write_reg=false;
			Write_PC=false;
			Write_Mem=false;
			
			Alu_op=OP_ADD;
			Ext_op=SIGNEXT;
			Alu_a=NPC;
			Alu_b=IM;
			Execute();
			
			Write_reg=false;
			Write_PC=true;
			Write_Mem=false;
			Alu_op=OP_SUB;
			Ext_op=Unuse;
			Alu_a=RS;
			Alu_b=RT;
			BEQ=false;
			BNE=true;
			PC_sel=Unuse;
			break;
		}
		case F_BLEZ:{
			if((int)refile[rs]<=0)
			{
				Write_reg=false;
				Write_PC=false;
				Write_Mem=false;
				
				Alu_op=OP_ADD;
				Ext_op=SIGNEXT;
				Alu_a=NPC;
				Alu_b=IM;
				Execute();
				PC=Alu_out;
			}
			break;
		}
		case F_BGTZ:{
			if((int)refile[rs]>0)
			{
				Write_reg=false;
				Write_PC=false;
				Write_Mem=false;
				
				Alu_op=OP_ADD;
				Ext_op=SIGNEXT;
				Alu_a=NPC;
				Alu_b=IM;
				Execute();
				PC=Alu_out;
			}
			break;
		}
		case F_BGEZ:{
			if(rt==1)
			{
				if((int)refile[rs]>=0)//BGEZ
				{
					Write_reg=false;
					Write_PC=false;
					Write_Mem=false;
					
					Alu_op=OP_ADD;
					Ext_op=SIGNEXT;
					Alu_a=NPC;
					Alu_b=IM;
					Execute();
					PC=Alu_out;
				}
				break;
			}
			else
			{
				if(rt==0)
				{
					if((int)refile[rs]<0)//BLTZ
					{
						Write_reg=false;
						Write_PC=false;
						Write_Mem=false;
						
						Alu_op=OP_ADD;
						Ext_op=SIGNEXT;
						Alu_a=NPC;
						Alu_b=IM;
						Execute();
						PC=Alu_out;
					}
					break;
				}
			}
		}
		case F_J:{
			Write_reg=false;
			Write_PC=true;
			Write_Mem=false;
			
			Alu_op=Unuse;
			Ext_op=Unuse;
			BEQ=false;
			BNE=false;
			PC_sel=IM;
			break;
		}
		case F_JAL:{
			Write_reg=true;
			Write_PC=true;
			Write_Mem=false;
			
			Alu_op=Unuse;
			Ext_op=Unuse;
			BEQ=false;
			BNE=false;
			PC_sel=IM;
			Reg_addr=31;
			Reg_sel=NPC;
			break;
		}
		default:{}
	}
}

void Execute()
{
	if(Write_Mem)
	{
		Mem_data[Mem_addr]=Reg;
		return ;
	}
	switch(Ext_op){
		case UNSIGNEXT:imm_ext(UNSIGNEXT);break;
		case SIGNEXT:imm_ext(SIGNEXT);break;
		default:;
	}
	switch(Alu_a){
		case NPC:A=PC;break;
		case RS:A=refile[rs];break;
		case SHEMT:A=shemt;break;
		case ELSE:A=ElseA;break;
		default:;
	}
	
	switch(Alu_b){
		case RT:B=refile[rt];break;
		case IM:B=im16;break;
		default:;
	}

	int temp=Alu();
	if(BEQ&&temp==0&&Write_PC)
	{
		PC=Alu_out;
		return ;
	}
	if(BNE&&temp!=0&&Write_PC)
	{
		PC=Alu_out;
		return ;
	}
	
	if(Write_reg)
	{
		switch(Reg_sel){
			case ALU:refile[Reg_addr]=temp;break;
			case MEM:refile[Reg_addr]=Mem;break;
			case NPC:refile[Reg_addr]=PC;break;
			default:;
		}
	}
	if(Write_PC)
	{
		switch(PC_sel){
		case RS:PC=refile[rs];break;
		case IM:{
			PC=PC>>26;
			PC=PC<<26;
			PC+=im26;
			break;
		}
		default:;
		}
	}
	Alu_out=temp;
}

void debug_look()
{
	cout <<endl<<".........look.........."<<endl;
	int place=1;
	int flag=1;
	cout <<"0:exit   1:look register    2:look memory"<<endl;
	cin >>flag;
	while(flag)
	{
		switch(flag){
		case 1:cout <<endl<<"------------look register(-1:exit)--------------"<<endl;break;
		default:cout <<endl<<"------------look memory(-1:exit)--------------"<<endl;
		}
		cout <<"input address:"<<endl;
		cin >>place;
		while(place>-1)
		{
			if(flag==1&&place<33)
			{
				if(place==32)
				{
					for(int a=0;a<32;a++)
					{
						printf("%2d:",a);
						printf("unsigned: %10u  0x%08x   signed: %10d  0x%08x\n",refile[a],refile[a],(int)refile[a],(int)refile[a]);
					}
				}
				else
				{
					printf("unsigned: %u  0x%08x\n",refile[place],refile[place]);
					printf("signed: %d  0x%08x\n",(int)refile[place],(int)refile[place]);
				}				
			}
			else
			{
				for(int a=0;a<31;a++)
				{
					printf("%4d:",a);
					printf("unsigned: %10u  0x%08x   signed: %10d  0x%08x\n",Mem_data[a],Mem_data[a],(int)Mem_data[a],(int)Mem_data[a]);
				}
			}
			
			cin >>place;
		}
		cout <<"0:exit   1:look register    2:look memory"<<endl;
		cin >>flag;
	}
}


void clear()
{
	int i=0;
	for(i=0;i<32;i++)
		refile[i]=0;
	for(i=0;i<102400;i++)
		Mem_data[i]=0;
}

int main()
{
	while(1)
	{
		load_inst();

		cout <<endl<<"0: continue   1: single step"<<endl;
		cin >>single_step;
		while(PC<inst_place)
		{
			read();//根据PC读取指令
			translate();//对指令进行译码
			translate1();
			Execute();//执行和写回
		}
		cout <<endl<<"执行完毕!"<<endl;
		debug_look();//查看寄存器内容和存储器内容
		cout <<endl;
		int flag=0;
		cout <<"0: 不清空数据   1: 清空数据 : "<<endl;
		cout <<"input :";
		cin >>flag;
		if(flag==1)
		{
			clear();
			cout <<"已清空!"<<endl<<endl;
		}		
	}	
	return 0;
}