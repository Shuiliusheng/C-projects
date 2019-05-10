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

char Mem_inst[102400][9]={0};
char reg[32][5]={"zero","at","vo","v1","a0","a1","a2","a3","t0","t1","t2","t3","t4","t5","t6","t7"
,"s0","s1","s2","s3","s4","s5","s6","s7","t8","t9","k0","k1","gp","sp","fp","ra"};

int PC=0;
int start_inst=3072;
unsigned int inst_place=0;
unsigned int data_place=0;

int single_step=0;//0:continue   1:single



#define UNSIGNEXT 0
#define SIGNEXT 1

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
#define F_ADDIU 9
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



void translate(char *inst);
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

void read()//read the command
{
	int a=0;
	if(single_step==1)
	{	
		cout <<"input 1 继续  3 直接执行 4 结束执行:";
		cin >>a;
		if(a==3)
			single_step=0;
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

void ALU(int OP)
{
	
	switch(OP){
		case OP_ADDU:printf(" addu $%d, $%d, $%d\t\t",rd,rs,rt);printf(" addu $%s, $%s, $%s\n",reg[rd],reg[rs],reg[rt]);break;
		case OP_ADD:printf(" add $%d, $%d, $%d\t\t",rd,rs,rt);printf(" add $%s, $%s, $%s\n",reg[rd],reg[rs],reg[rt]);break;
		case OP_SUB:printf(" sub $%d, $%d, $%d\t\t",rd,rs,rt);printf(" sub $%s, $%s, $%s\n",reg[rd],reg[rs],reg[rt]);break;
		case OP_SUBU:printf(" subu $%d, $%d, $%d\t\t",rd,rs,rt);printf(" subu $%s, $%s, $%s\n",reg[rd],reg[rs],reg[rt]);break;
		case OP_AND:printf(" and $%d, $%d, $%d\t\t",rd,rs,rt);printf(" and $%s, $%s, $%s\n",reg[rd],reg[rs],reg[rt]);break;	
		case OP_OR:printf(" or $%d, $%d, $%d\t\t",rd,rs,rt);printf(" or $%s, $%s, $%s\n",reg[rd],reg[rs],reg[rt]);break;
		case OP_NOR:printf(" nor $%d, $%d, $%d\t\t",rd,rs,rt);printf(" nor $%s, $%s, $%s\n",reg[rd],reg[rs],reg[rt]);break;
		case OP_XOR:printf(" xor $%d, $%d, $%d\t\t",rd,rs,rt);printf(" xor $%s, $%s, $%s\n",reg[rd],reg[rs],reg[rt]);break;
		case OP_SLT:printf(" slt $%d, $%d, $%d\t\t",rd,rs,rt);printf(" slt $%s, $%s, $%s\n",reg[rd],reg[rs],reg[rt]);break;
		case OP_SLTU:printf(" sltu $%d, $%d, $%d\t\t",rd,rs,rt);printf(" sltu $%s, $%s, $%s\n",reg[rd],reg[rs],reg[rt]);break;
		case OP_SLL:printf(" sll $%d, $%d, %d\t\t",rd,rt,shemt);printf(" sll $%s, $%s, %d\n",reg[rd],reg[rt],shemt);break;
		case OP_SLLV:printf(" sllv $%d, $%d, $%d\t\t",rd,rt,rs);printf(" sllv $%s, $%s, $%s\n",reg[rd],reg[rt],reg[rs]);break;
		case OP_SRL:printf(" srl $%d, $%d, %d\t\t",rd,rt,shemt);printf(" srl $%s, $%s, %d\n",reg[rd],reg[rt],shemt);break;
		case OP_SRLV:printf(" srlv $%d, $%d, $%d\t\t",rd,rt,rs);printf(" srlv $%s, $%s, $%s\n",reg[rd],reg[rt],reg[rs]);break;
		case OP_SRA:printf(" sra $%d, $%d, %d\t\t",rd,rt,shemt);printf(" sra $%s, $%s, %d\n",reg[rd],reg[rt],shemt);break;			
		case OP_SRAV:printf(" srav $%d, $%d, $%d\t\t",rd,rt,rs);printf(" srav $%s, $%s, $%s\n",reg[rd],reg[rt],reg[rs]);break;
		case OP_MULT:printf(" mult $%d, $%d\t\t",rs,rt);printf(" mult $%s, $%s\n",reg[rs],reg[rt]);break;
		case OP_MULTU:printf(" multu $%d, $%d\t\t",rs,rt);printf(" multu $%s, $%s\n",reg[rs],reg[rt]);break;
		case OP_DIV:printf(" div $%d, $%d\t\t",rs,rt);printf(" div $%s, $%s\n",reg[rs],reg[rt]);break;
		case OP_DIVU:printf(" divu $%d, $%d\t\t",rs,rt);printf(" divu $%s, $%s\n",reg[rs],reg[rt]);break;
		case OP_MFHI:printf(" mfhi $%d\t\t",rd);printf(" mfhi $%s\n",reg[rd]);break;
		case OP_MFLO:printf(" mflo $%d\t\t",rd);printf(" mflo $%s\n",reg[rd]);break;
		default:{}
	}
	

}

int execute()
{
	if(rs==0&&rt==0&&rd==0&&shemt==0&&OP==0)
		return -1;
	switch(FUNCT){
		case F_R:{
			switch(OP){
				case OP_JALR:printf(" jalr $%d, $%d\t\t",rd,rs);printf(" jalr $%s, $%s\n",reg[rd],reg[rs]);break;
				case OP_JR:printf(" jr $%d\t\t",rs);printf(" jr $%s\n",reg[rs]);break;
				default:ALU(OP);
			}
			break;
		}
		case F_IN:{
			printf(" in $%d, %d\t\t",rd,shemt);
			printf(" in $%s, %d\n",reg[rd],shemt);
			break;
		}
		case F_OUT:{
			printf(" out $%d, %d\t\t",rd,shemt);
			printf(" out $%s, %d\n",reg[rd],shemt);
			break;
		}
		case F_LW:{
			imm_ext(SIGNEXT);
			printf(" lw $%d, %d($%d)\t\t",rt,im16,rs);
			printf(" lw $%s, %d($%s)\n",reg[rt],im16,reg[rs]);
			break;
		}
		case F_LUI:{
			imm_ext(UNSIGNEXT);
			printf(" lui $%d, %u\t\t",rt,im16);
			printf(" lui $%s, %u\n",reg[rt],im16);
			break;
		}
		case F_SW:{
			imm_ext(SIGNEXT);
			printf(" sw $%d, %d($%d)\t\t",rt,im16,rs);
			printf(" sw $%s, %d($%s)\n",reg[rt],im16,reg[rs]);
			break;
		}
		case F_LB:{
			imm_ext(SIGNEXT);
			printf(" lb $%d, %d($%d)\t\t",rt,im16,rs);
			printf(" lb $%s, %d($%s)\n",reg[rt],im16,reg[rs]);
			break;
		}
		case F_LBU:{
			imm_ext(SIGNEXT);
			printf(" lbu $%d, %d($%d)\t\t",rt,im16,rs);
			printf(" lbu $%s, %d($%s)\n",reg[rt],im16,reg[rs]);
			break;
		}
		case F_LH:{
			imm_ext(SIGNEXT);
			printf(" lh $%d, %d($%d)\t\t",rt,im16,rs);
			printf(" lh $%s, %d($%s)\n",reg[rt],im16,reg[rs]);
			break;
		}
		case F_LHU:{
			imm_ext(SIGNEXT);
			printf(" lhu $%d, %d($%d)\t\t",rt,im16,rs);
			printf(" lhu $%s, %d($%s)\n",reg[rt],im16,reg[rs]);
			break;
		}
		case F_SB:{
			imm_ext(SIGNEXT);
			printf(" sb $%d, %d($%d)\t\t",rt,im16,rs);
			printf(" sb $%s, %d($%s)\n",reg[rt],im16,reg[rs]);
			break;
		}
		case F_SH:{
			imm_ext(SIGNEXT);
			printf(" sh $%d, %d($%d)\t\t",rt,im16,rs);
			printf(" sh $%s, %d($%s)\n",reg[rt],im16,reg[rs]);
			break;
		}
		case F_ADDIU:{
			imm_ext(SIGNEXT);
			printf(" addiu $%d, $%d, %d\t\t",rt,rs,im16);
			printf(" addiu $%s, $%s, %d\n",reg[rt],reg[rs],im16);
			break;
		}
		case F_ADDI:{
			imm_ext(SIGNEXT);
			printf(" addi $%d, $%d, %d\t\t",rt,rs,im16);
			printf(" addi $%s, $%s, %d\n",reg[rt],reg[rs],im16);
			break;
		}
		case F_ANDI:{
			imm_ext(UNSIGNEXT);
			printf(" andi $%d, $%d, %d\t\t",rt,rs,im16);
			printf(" andi $%s, $%s, %d\n",reg[rt],reg[rs],im16);
			break;
		}
		case F_ORI:{
			imm_ext(UNSIGNEXT);
			printf(" ori $%d, $%d, %d\t\t",rt,rs,im16);
			printf(" ori $%s, $%s, %d\n",reg[rt],reg[rs],im16);
			break;
		}
		case F_XORI:{
			imm_ext(UNSIGNEXT);
			printf(" xori $%d, $%d, %d\t\t",rt,rs,im16);
			printf(" xori $%s, $%s, %d\n",reg[rt],reg[rs],im16);
			break;
		}
		case F_SLTI:{
			imm_ext(SIGNEXT);
			printf(" slti $%d, $%d, %d\t\t",rt,rs,im16);
			printf(" slti $%s, $%s, %d\n",reg[rt],reg[rs],im16);
			break;
		}
		case F_SLTIU:{
			imm_ext(SIGNEXT);
			printf(" sltiu $%d, $%d, %d\t\t",rt,rs,im16);
			printf(" sltiu $%s, $%s, %d\n",reg[rt],reg[rs],im16);
			break;
		}
		case F_BEQ:{
			imm_ext(SIGNEXT);
			printf(" beq $%d, $%d, %d\t\t",rs,rt,im16);
			printf(" beq $%s, $%s, %d\n",reg[rs],reg[rt],im16);
			break;
		}
		case F_BNE:{
			imm_ext(SIGNEXT);
			printf(" bne $%d, $%d, %d\t\t",rs,rt,im16);
			printf(" bne $%s, $%s, %d\n",reg[rs],reg[rt],im16);
			break;
		}
		case F_BLEZ:{
			imm_ext(SIGNEXT);
			printf(" blez $%d, %d\t\t",rs,im16);
			printf(" blez $%s, %d\n",reg[rs],im16);
			break;
		}
		case F_BGTZ:{
			imm_ext(SIGNEXT);
			printf(" bgtz $%d, %d\t\t",rs,im16);
			printf(" bgtz $%s, %d\n",reg[rs],im16);
			break;
		}
		case F_BGEZ:{
			if(rt==1)
			{
				imm_ext(SIGNEXT);
				printf(" bgez $%d, %d\t\t",rs,im16);
				printf(" bgez $%s, %d\n",reg[rs],im16);
				break;
			}
			else
			{
				if(rt==0)
				{
					imm_ext(SIGNEXT);
					printf(" bltz $%d, %d\t\t",rs,im16);
					printf(" bltz $%s, %d\n",reg[rs],im16);
					break;
				}
			}
		}
		case F_J:{
			printf(" j 0x%x\t\t",im26*4);
			printf(" j %d\n",im26*4);
			break;
		}
		case F_JAL:{
			printf(" jal 0x%x\t\t",im26*4);
			printf(" jal %d\n",im26*4);
			break;
		}
		default:{}
	}
	return 1;
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
			execute();//执行和写回
		}
		cout <<endl<<"执行完毕!"<<endl;

	}	
	return 0;
}