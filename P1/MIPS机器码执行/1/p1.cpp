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
		case OP_ADDU:
		case OP_ADD:refile[rd]=refile[rs]+refile[rt];break;
		case OP_SUB:
		case OP_SUBU:refile[rd]=refile[rs]-refile[rt];break;
		case OP_AND:refile[rd]=refile[rs]&refile[rt];break;	
		case OP_OR:refile[rd]=refile[rs]|refile[rt];break;
		case OP_NOR:refile[rd]=~(refile[rs]&refile[rt]);break;
		case OP_XOR:refile[rd]=refile[rs]^refile[rt];break;
		case OP_SLT:refile[rd]=((int)refile[rs]<(int)refile[rt])?1:0;break;
		case OP_SLTU:refile[rd]=(refile[rs]<refile[rt])?1:0;break;
		case OP_SLL:refile[rd]=refile[rt]<<shemt;break;
		case OP_SLLV:
		{
			int temp=refile[rs]%32;
			refile[rd]=refile[rt]<<temp;break;
		}
		case OP_SRL:refile[rd]=refile[rt]>>shemt;break;
		case OP_SRLV:
		{
			int temp1=refile[rs]%32;
			refile[rd]=refile[rt]>>temp1;break;
		}
		case OP_SRA:
		{
			int t=refile[rt]<<(32-shemt);
			refile[rd]=t+refile[rt]>>shemt;break;
		}
			
		case OP_SRAV:
		{
			int temp=refile[rs]%32;
			int t=refile[rt]<<(32-temp);
			refile[rd]=t+refile[rt]>>temp;break;
		}
		case OP_MULT:
		{
			double t=4294967296;
			double temp=(int)refile[rs]*(int)refile[rt];
			HI=(unsigned int)(temp/t);
			LO=(unsigned int)(temp-HI*t);
			break;
		}
		case OP_MULTU:
		{
			double t=4294967296;
			double temp=refile[rs]*refile[rt];
			HI=(unsigned int)(temp/t);
			LO=(unsigned int)(temp-HI*t);
			break;
		}
		case OP_DIV:
		{
			if(refile[rt]==0)
			{
				cout <<"除数为零"<<endl;
				break;
			}
			LO=(unsigned int)((int)refile[rs]/(int)refile[rt]);	
			HI=(unsigned int)((int)refile[rs]%(int)refile[rt]);
			break;
		}
		case OP_DIVU:
		{
			if(refile[rt]==0)
			{
				cout <<"除数为零"<<endl;
				break;
			}
			LO=refile[rs]/refile[rt];	
			HI=refile[rs]%(int)refile[rt];
			break;
		}
		case OP_MFHI:refile[rd]=HI;break;
		case OP_MFLO:refile[rd]=LO;break;
		default:{}
	}
	

}

void execute()
{
	switch(FUNCT){
		case F_R:{
			switch(OP){
				case OP_JALR:{
					refile[rd]=PC;
					PC=refile[rs];					
					break;
				}
				case OP_JR:PC=refile[rs];break;
				default:ALU(OP);
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
			imm_ext(SIGNEXT);
			unsigned int addr=refile[rs]+im16;
			addr=addr>>2;
			refile[rt]=Mem_data[addr];
			break;
		}
		case F_LUI:{
			imm_ext(UNSIGNEXT);
			refile[rt]=im16<<16;
			break;
		}
		case F_SW:{
			imm_ext(SIGNEXT);
			unsigned int addr=refile[rs]+im16;
			addr=addr>>2;
			Mem_data[addr]=refile[rt];
			break;
		}
		case F_LB:{
			imm_ext(SIGNEXT);
			unsigned int addr=refile[rs]+im16;
			int n=addr%4;
			addr=addr>>2;
			refile[rt]=Mem_data[addr]<<(24-8*n);
			refile[rt]=refile[rt]>>24;
			unsigned int temp=0;
			unsigned int t=refile[rt]>>7;
			if(t==1)
			{
				for(int i=0;i<24;i++)
				{
					temp*=2;
					temp+=t;
				}
				refile[rt]+=(temp*256);
			}
			break;
		}
		case F_LBU:{
			imm_ext(SIGNEXT);
			unsigned int addr=refile[rs]+im16;
			int n=addr%4;
			addr=addr>>2;
			refile[rt]=Mem_data[addr]<<(24-8*n);
			refile[rt]=refile[rt]>>24;
			break;
		}
		case F_LH:{
			imm_ext(SIGNEXT);
			unsigned int addr=refile[rs]+im16;
			int n=addr/2;
			n%=2;
			addr=addr>>2;
			refile[rt]=Mem_data[addr]<<(16-16*n);
			refile[rt]=refile[rt]>>16;
			unsigned int temp=0;
			unsigned int t=refile[rt]>>15;
			if(t==1)
			{
				for(int i=0;i<16;i++)
				{
					temp*=2;
					temp+=t;
				}
				refile[rt]+=(temp*65536);
			}
			break;
		}
		case F_LHU:{
			imm_ext(SIGNEXT);
			unsigned int addr=refile[rs]+im16;
			int n=addr/2;
			n%=2;
			addr=addr>>2;
			refile[rt]=Mem_data[addr]<<(16-16*n);
			refile[rt]=refile[rt]>>16;
			break;
		}
		case F_SB:{
			imm_ext(SIGNEXT);
			unsigned int addr=refile[rs]+im16;
			int n=addr%4;
			unsigned int temp=255;
			addr=addr>>2;
			temp=temp<<(8*n);
			Mem_data[addr]=Mem_data[addr]|temp;
			Mem_data[addr]-=temp;
			temp=refile[rt]%256;
			temp=temp<<(8*n);
			Mem_data[addr]=Mem_data[addr]+temp;
			break;
		}
		case F_SH:{
			imm_ext(SIGNEXT);
			unsigned int addr=refile[rs]+im16;
			int n=addr/2;
			n%=2;
			unsigned int temp=65535;
			addr=addr>>2;
			temp=temp<<(16*n);
			Mem_data[addr]=Mem_data[addr]|temp;
			Mem_data[addr]-=temp;
			temp=refile[rt]%65536;
			temp=temp<<(16*n);
			Mem_data[addr]=Mem_data[addr]+temp;
			break;
		}
		case F_ADDUI:
		case F_ADDI:{
			imm_ext(SIGNEXT);
			refile[rt]=refile[rs]+im16;
			break;
		}
		case F_ANDI:{
			imm_ext(UNSIGNEXT);
			refile[rt]=refile[rs]&im16;
			break;
		}
		case F_ORI:{
			imm_ext(UNSIGNEXT);
			refile[rt]=refile[rs]|im16;
			break;
		}
		case F_XORI:{
			imm_ext(UNSIGNEXT);
			refile[rt]=refile[rs]^im16;
			break;
		}
		case F_SLTI:{
			imm_ext(SIGNEXT);
			refile[rt]=(int)refile[rs]<(int)im16 ?1:0;
			break;
		}
		case F_SLTIU:{
			imm_ext(SIGNEXT);
			refile[rt]=refile[rs]<im16 ?1:0;
			break;
		}
		case F_BEQ:{
			if(refile[rs]==refile[rt])
			{
				imm_ext(SIGNEXT);
				PC=PC+(int)im16;
			}
			break;
		}
		case F_BNE:{
			if(refile[rt]!=refile[rs])
			{
				imm_ext(SIGNEXT);
				PC=PC+(int)im16;
			}
			break;
		}
		case F_BLEZ:{
			//cout <<"BLEZ:"<<(int)refile[rs]<<endl;
			if((int)refile[rs]<=0)
			{
				imm_ext(SIGNEXT);
				PC=PC+(int)im16;
			}
			break;
		}
		case F_BGTZ:{
			if((int)refile[rs]>0)
			{
				imm_ext(SIGNEXT);
				PC=PC+(int)im16;
			}
			break;
		}
		case F_BGEZ:{
			//cout <<"BGEZ:"<<(int)refile[rs]<<endl;
			if(rt==1)
			{
				if((int)refile[rs]>=0)//BGEZ
				{
					imm_ext(SIGNEXT);
					PC=PC+(int)im16;
				}
				break;
			}
			else
			{
				if(rt==0)
				{
					if((int)refile[rs]<0)//BLTZ
					{
						imm_ext(SIGNEXT);
						PC=PC+(int)im16;
					}
					break;
				}
			}
		}
		case F_J:{
			PC=PC>>26;
			PC=PC<<26;
			PC+=im26;
			break;
		}
		case F_JAL:{
			refile[31]=PC;
			PC=PC>>26;
			PC=PC<<26;
			PC+=im26;		
			break;
		}
		default:{}
	}
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
			execute();//执行和写回
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