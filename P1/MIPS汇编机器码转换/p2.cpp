#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

#define LOAD 1
#define STORE 2

#define R3 3
#define RI 4
#define BEQ 5
#define J 6
#define JALR 7
#define MUL 8
#define JR 9
#define SHIFT 10
#define MF 11
#define BE 12
#define BGE 13
#define LUI 14

#define IO 15


struct label{
	char temp[100];
	unsigned int pc;
};

label Lable[1000];

int num_label=0;
unsigned int SPC=3000;
unsigned int PC=3000;

int funct=0;
char inst[33]={0};
char save_name[100];

void uniform()
{
	char filename[100];
	cout <<"input the file name:"<<endl;
	cin >>filename;
	FILE *p=NULL;
	p=fopen(filename,"rb");
	char temp[30000]={0};
	char temp1[30000]={0};
	int n=0;

	n=fread(temp,1,30000,p);
	cout <<n<<endl;
	temp[n]='\0';
	int a=0,i=0;
	for(i=0;i<n;i++)
	{
		if(temp[i]==':')
		{
			if(temp[i+1]>='a'&&temp[i+1]<='z'||temp[i+1]<='Z'&&temp[i+1]>='A')
			{
				temp1[a++]=temp[i];
				temp1[a++]=' ';
				continue;
			}
		}
		if(temp[i]==';'||temp[i]=='#')
		{
			int t=0;
			for(t=i;t<n;t++)
				if(temp[t]=='\n'||temp[t]=='\r')
					break;
			i=t;
			continue;
		}			
		temp1[a++]=temp[i];
		if(temp[i]==',')
		{
			int t=0;
			for(t=i+1;t<n;t++)
				if(temp[t]!=' ')
					break;
			i=t-1;
		}
	}
	n=a;
	for(i=0;i<n;i++)
	{
		if(temp1[i]==':')
		{
			int t=0;
			for(t=i+1;t<n;t++)
				if(temp1[t]>='a'&&temp1[t]<='z'||temp1[t]<='Z'&&temp1[t]>='A')
					break;
				else
					temp1[t]=' ';
		}
	}
	a=0;
	for(i=0;i<n;i++)
	{
		temp[a++]=temp1[i];
		if(temp1[i]=='$')
		{
			int t=0;
			for(t=i+1;t<n;t++)
			{
				if(temp1[t]!=' ')
					temp[a++]=temp1[t];
				if(temp1[t]=='\n'||temp1[t]=='\r')
					break;
			}
			i=t;
		}
	}
	n=a;
	a=0;
	for(i=0;i<n;i++)
	{
		temp1[a++]=temp[i];
		if(temp[i]=='$'&&temp[i+1]<='z'&&temp[i+1]>='a')
		{
			if(temp[i+1]=='z')
			{
				temp1[a++]='0';
				i=i+4;
			}
			else if(temp[i+1]=='a'&&temp[i+2]=='t')
			{
				temp1[a++]='1';
				i=i+2;
			}
			else if(temp[i+1]=='g')
			{
				temp1[a++]='2';
				temp1[a++]='8';
				i=i+2;
			}
			else if(temp[i+1]=='s'&&temp[i+2]=='p')
			{
				temp1[a++]='2';
				temp1[a++]='9';
				i=i+2;
			}
			else if(temp[i+1]=='f')
			{
				temp1[a++]='3';
				temp1[a++]='0';
				i=i+2;
			}
			else if(temp[i+1]=='r')
			{
				temp1[a++]='3';
				temp1[a++]='1';
				i=i+2;
			}
			else if(temp[i+1]=='v')
			{
				temp1[a++]=2+temp[i+2];
				i+=2;
			}
			else if(temp[i+1]=='a')
			{
				temp1[a++]=4+temp[i+2];
				i+=2;
			}
			else if(temp[i+1]=='t')
			{
				switch(temp[i+2]){
					case '0':
					case '1':temp1[a++]=8+temp[i+2];break;
					case '8':
					case '9':temp1[a++]='2';temp1[a++]=temp[i+2]-4;break;
					default:temp1[a++]='1';temp1[a++]=temp[i+2]-2;
				}
				i+=2;
			}
			else if(temp[i+1]=='k')
			{
				temp1[a++]='2';
				temp1[a++]=temp[i+2]+6;
				i=i+2;
			}
			else
			{
				if(temp[i+2]<'4')
				{
					temp1[a++]='1';
					temp1[a++]=temp[i+2]+6;
				}
				else
				{
					temp1[a++]='2';
					temp1[a++]=temp[i+2]-4;
				}
				i+=2;
			}
		}
	}



	cout <<"规整后的代码名称:"<<endl;
	cin >>save_name;

	FILE *save=fopen(save_name,"wb");
	fwrite(temp1,1,a,save);
	fclose(save);
	fclose(p);
	
	cout <<"保存成功!"<<endl<<endl;

}

int get_FUN(char op[])
{
	if(strcmp(op,"IN")==0||strcmp(op,"in")==0)
	{
		char temp[7]="110010";
		char temp1[7]="000000";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return IO;
	}
	
	if(strcmp(op,"OUT")==0||strcmp(op,"out")==0)
	{
		char temp[7]="110011";
		char temp1[7]="000000";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return IO;
	}


	if(strcmp(op,"LB")==0||strcmp(op,"lb")==0)
	{
		char temp[7]="100000";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return LOAD;
	}
	if(strcmp(op,"LBU")==0||strcmp(op,"lbu")==0)
	{
		char temp[7]="100100";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return LOAD;
	}
	if(strcmp(op,"LH")==0||strcmp(op,"lh")==0)
	{
		char temp[7]="100001";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return LOAD;
	}
	if(strcmp(op,"LHU")==0||strcmp(op,"lhu")==0)
	{
		char temp[7]="100101";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return LOAD;
	}
	if(strcmp(op,"LW")==0||strcmp(op,"lw")==0)
	{
		char temp[7]="100011";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return LOAD;
	}
	
	if(strcmp(op,"SB")==0||strcmp(op,"sb")==0)
	{
		char temp[7]="101000";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return STORE;
	}
	if(strcmp(op,"SH")==0||strcmp(op,"sh")==0)
	{
		char temp[7]="101001";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return STORE;
	}
	if(strcmp(op,"SW")==0||strcmp(op,"sw")==0)
	{
		char temp[7]="101011";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return STORE;
	}
	


	if(strcmp(op,"ADD")==0||strcmp(op,"add")==0)
	{
		char temp[7]="000000";
		char temp1[7]="100000";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return R3;
	}
	if(strcmp(op,"ADDU")==0||strcmp(op,"addu")==0)
	{
		char temp[7]="000000";
		char temp1[7]="100001";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return R3;
	}
	if(strcmp(op,"SUB")==0||strcmp(op,"sub")==0)
	{
		char temp[7]="000000";
		char temp1[7]="100010";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return R3;
	}
	if(strcmp(op,"SUBU")==0||strcmp(op,"subu")==0)
	{
		char temp[7]="000000";
		char temp1[7]="100011";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return R3;
	}
	if(strcmp(op,"SLT")==0||strcmp(op,"slt")==0)
	{
		char temp[7]="000000";
		char temp1[7]="101010";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return R3;
	}
	if(strcmp(op,"SLTU")==0||strcmp(op,"sltu")==0)
	{
		char temp[7]="000000";
		char temp1[7]="101011";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return R3;
	}
	if(strcmp(op,"SLL")==0||strcmp(op,"sll")==0)
	{
		char temp[7]="000000";
		char temp1[7]="000000";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return SHIFT;
	}
	if(strcmp(op,"SRL")==0||strcmp(op,"srl")==0)
	{
		char temp[7]="000000";
		char temp1[7]="000010";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return SHIFT;
	}
	if(strcmp(op,"SRA")==0||strcmp(op,"sra")==0)
	{
		char temp[7]="000000";
		char temp1[7]="000011";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return SHIFT;
	}
	if(strcmp(op,"SLLV")==0||strcmp(op,"sllv")==0)
	{
		char temp[7]="000000";
		char temp1[7]="000100";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return R3;
	}
	if(strcmp(op,"SRLV")==0||strcmp(op,"srlv")==0)
	{
		char temp[7]="000000";
		char temp1[7]="000110";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return R3;
	}
	if(strcmp(op,"SRAV")==0||strcmp(op,"srav")==0)
	{
		char temp[7]="000000";
		char temp1[7]="000111";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return R3;
	}
	if(strcmp(op,"AND")==0||strcmp(op,"and")==0)
	{
		char temp[7]="000000";
		char temp1[7]="100100";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return R3;
	}
	if(strcmp(op,"OR")==0||strcmp(op,"or")==0)
	{
		char temp[7]="000000";
		char temp1[7]="100101";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return R3;
	}
	if(strcmp(op,"XOR")==0||strcmp(op,"xor")==0)
	{
		char temp[7]="000000";
		char temp1[7]="100110";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return R3;
	}
	if(strcmp(op,"NOR")==0||strcmp(op,"nor")==0)
	{
		char temp[7]="000000";
		char temp1[7]="100111";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return R3;
	}



	if(strcmp(op,"ADDI")==0||strcmp(op,"addi")==0)
	{
		char temp[7]="001000";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return RI;
	}
	if(strcmp(op,"ADDIU")==0||strcmp(op,"addiu")==0)
	{
		char temp[7]="001001";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return RI;
	}
	if(strcmp(op,"ANDI")==0||strcmp(op,"andi")==0)
	{
		char temp[7]="001100";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return RI;
	}
	if(strcmp(op,"ORI")==0||strcmp(op,"ori")==0)
	{
		char temp[7]="001101";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return RI;
	}
	if(strcmp(op,"XORI")==0||strcmp(op,"xori")==0)
	{
		char temp[7]="001110";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return RI;
	}
	if(strcmp(op,"LUI")==0||strcmp(op,"lui")==0)
	{
		char temp[7]="001111";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return LUI;
	}
	if(strcmp(op,"SLTI")==0||strcmp(op,"slti")==0)
	{
		char temp[7]="001010";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return RI;
	}
	if(strcmp(op,"SLTIU")==0||strcmp(op,"sltiu")==0)
	{
		char temp[7]="001011";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return RI;
	}




	if(strcmp(op,"BEQ")==0||strcmp(op,"beq")==0)
	{
		char temp[7]="000100";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return BEQ;
	}
	if(strcmp(op,"BNE")==0||strcmp(op,"bne")==0)
	{
		char temp[7]="000101";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return BEQ;
	}
	if(strcmp(op,"BLEZ")==0||strcmp(op,"blez")==0)
	{
		char temp[7]="000110";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return BE;
	}
	if(strcmp(op,"BGEZ")==0||strcmp(op,"bgez")==0)
	{
		char temp[7]="000001";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return BGE;
	}
	if(strcmp(op,"BGTZ")==0||strcmp(op,"bgtz")==0)
	{
		char temp[7]="000111";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return BE;
	}
	if(strcmp(op,"BLTZ")==0||strcmp(op,"bltz")==0)
	{
		char temp[7]="000001";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return BE;
	}
	

	if(strcmp(op,"J")==0||strcmp(op,"j")==0)
	{
		char temp[7]="000010";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return J;
	}
	if(strcmp(op,"JAL")==0||strcmp(op,"jal")==0)
	{
		char temp[7]="000011";
		for(int i=0;i<6;i++)
			inst[i]=temp[i];
		return J;
	}


	if(strcmp(op,"JALR")==0||strcmp(op,"jalr")==0)
	{
		char temp[7]="000000";
		char temp1[7]="001001";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return JALR;
	}


	if(strcmp(op,"JR")==0||strcmp(op,"jr")==0)
	{
		char temp[7]="000000";
		char temp1[7]="001000";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return JR;
	}
	

	if(strcmp(op,"MULT")==0||strcmp(op,"mult")==0)
	{
		char temp[7]="000000";
		char temp1[7]="011000";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return MUL;
	}
	if(strcmp(op,"MULTU")==0||strcmp(op,"multu")==0)
	{
		char temp[7]="000000";
		char temp1[7]="011001";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return MUL;
	}
	if(strcmp(op,"DIV")==0||strcmp(op,"div")==0)
	{
		char temp[7]="000000";
		char temp1[7]="011010";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return MUL;
	}
	if(strcmp(op,"DIVU")==0||strcmp(op,"divu")==0)
	{
		char temp[7]="000000";
		char temp1[7]="011011";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return MUL;
	}

	
	if(strcmp(op,"MFHI")==0||strcmp(op,"mfhi")==0)
	{
		char temp[7]="000000";
		char temp1[7]="010000";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return MF;
	}
	if(strcmp(op,"MFLO")==0||strcmp(op,"mflo")==0)
	{
		char temp[7]="000000";
		char temp1[7]="010010";
		for(int i=0;i<6;i++)
			inst[i]=temp[i],inst[26+i]=temp1[i];
		return MF;
	}

	return -1;
}

int get_else(char code[])
{
	switch(funct){
		case LOAD:
		case STORE:
		{
			int rs=0,rt=0,temp=0,i=0;
			unsigned int im=0;
			sscanf(code,"$%d,%d($%d)",&rt,&temp,&rs);
			im=(unsigned int)temp;
			for(i=4;i>=0;i--)
			{
				inst[6+i]=rs%2+'0';
				inst[11+i]=rt%2+'0';
				rs/=2;
				rt/=2;
			}
			for(i=31;i>=16;i--)
			{
				inst[i]=im%2+'0';
				im/=2;
			}
			break;
		}
		case R3:
		{
			int rs=0,rt=0,rd=0;
			sscanf(code,"$%d,$%d,$%d",&rd,&rs,&rt);
			for(int i=4;i>=0;i--)
			{
				inst[6+i]=rs%2+'0';
				inst[11+i]=rt%2+'0';
				inst[16+i]=rd%2+'0';
				inst[21+i]='0';
				rs/=2;
				rd/=2;
				rt/=2;
			}
			break;
		}
		case SHIFT:
		{
			int rd=0,rt=0,temp=0;
			unsigned int s=0;
			sscanf(code,"$%d,$%d,%d",&rd,&rt,&temp);
			s=(unsigned int)temp;
			for(int i=4;i>=0;i--)
			{
				inst[6+i]='0';
				inst[11+i]=rt%2+'0';
				inst[16+i]=rd%2+'0';
				inst[21+i]=s%2+'0';
				s/=2;
				rd/=2;
				rt/=2;
			}
			break;
		}
		case RI:
		{
			int rs=0,rt=0,temp=0,i=0;
			unsigned int im=0;
			sscanf(code,"$%d,$%d,%d",&rt,&rs,&temp);
			im=(unsigned int)temp;
			for(i=4;i>=0;i--)
			{
				inst[6+i]=rs%2+'0';
				inst[11+i]=rt%2+'0';
				rs/=2;
				rt/=2;
			}
			for(i=31;i>=16;i--)
			{
				inst[i]=im%2+'0';
				im/=2;
			}
			break;
		}
		case LUI:
		{
			int rt=0,i=0;
			int im=0;
			sscanf(code,"$%d,%d",&rt,&im);
			for(i=4;i>=0;i--)
			{
				inst[6+i]='0';
				inst[11+i]=rt%2+'0';
				rt/=2;
			}
			for(i=31;i>=16;i--)
			{
				inst[i]=im%2+'0';
				im/=2;
			}
			break;
		}
		case BEQ:
		{
			int rs=0,rt=0,i=0,t1=0;
			char temp[100]={0};
			unsigned int im=0;
			sscanf(code,"$%d,$%d,%s",&rs,&rt,&temp);
			for(i=0;i<num_label;i++)
			{
				if(strcmp(temp,Lable[i].temp)==0)
				{
					t1=Lable[i].pc-PC;
					break;
				}
			}
			t1=t1>>2;
			im=(unsigned int)(t1-1);
			for(i=4;i>=0;i--)
			{
				inst[6+i]=rs%2+'0';
				inst[11+i]=rt%2+'0';
				rs/=2;
				rt/=2;
			}
			for(i=31;i>=16;i--)
			{
				inst[i]=im%2+'0';
				im/=2;
			}
			break;
		}
		case BGE:
		{
			int rs=0,i=0,t1=0;
			char temp[100]={0};
			unsigned int im=0;
			sscanf(code,"$%d,%s",&rs,&temp);

			for(i=0;i<num_label;i++)
			{
				if(strcmp(temp,Lable[i].temp)==0)
				{
					t1=Lable[i].pc-PC;
					break;
				}
			}
			t1=t1>>2;
			im=(unsigned int)(t1-1);
			for(i=4;i>=0;i--)
			{
				inst[6+i]=rs%2+'0';
				inst[11+i]='0';
				rs/=2;
			}
			inst[15]='1';
			for(i=31;i>=16;i--)
			{
				inst[i]=im%2+'0';
				im/=2;
			}
			break;
		}
		case BE:
		{
			int rs=0,i=0,t1=0;
			char temp[100]={0};
			unsigned int im=0;
			sscanf(code,"$%d,%s",&rs,&temp);

			for(i=0;i<num_label;i++)
			{
				if(strcmp(temp,Lable[i].temp)==0)
				{
					t1=Lable[i].pc-PC;
					break;
				}
			}
			t1=t1>>2;
			im=(unsigned int)(t1-1);
			for(i=4;i>=0;i--)
			{
				inst[6+i]=rs%2+'0';
				inst[11+i]='0';
				rs/=2;
			}
			for(i=31;i>=16;i--)
			{
				inst[i]=im%2+'0';
				im/=2;
			}
			break;
		}
		case J:
		{
			int i=0;
			unsigned int pc=PC;
			for(i=0;i<num_label;i++)
			{
				if(strcmp(code,Lable[i].temp)==0)
				{
					pc=Lable[i].pc;
					break;
				}
			}
			pc=pc>>2;
			for(i=25;i>=0;i--)
			{
				inst[6+i]=pc%2+'0';
				pc/=2;
			}
			break;
		}
		case JALR:
		{
			int rs=0,rd=0;
			sscanf(code,"$%d,$%d",&rd,&rs);
			for(int i=4;i>=0;i--)
			{
				inst[6+i]=rs%2+'0';
				inst[11+i]='0';
				inst[16+i]=rd%2+'0';
				inst[21+i]='0';
				rd/=2;
				rs/=2;
			}
			break;
		}
		case MUL:
		{
			int rs=0,rt=0;
			sscanf(code,"$%d,$%d",&rs,&rt);
			for(int i=4;i>=0;i--)
			{
				inst[6+i]=rs%2+'0';
				inst[11+i]=rt%2+'0';
				inst[16+i]='0';
				inst[21+i]='0';
				rt/=2;
				rs/=2;
			}
			break;
		}
		case JR:
		{
			int rs=0;
			sscanf(code,"$%d",&rs);
			for(int i=4;i>=0;i--)
			{
				inst[6+i]=rs%2+'0';
				inst[11+i]='0';
				inst[16+i]='0';
				inst[21+i]='0';
				rs/=2;
			}
			break;
		}
		case MF:
		{	
			int rd=0;
			sscanf(code,"$%d",&rd);
			for(int i=4;i>=0;i--)
			{
				inst[6+i]='0';
				inst[11+i]='0';
				inst[16+i]=rd%2+'0';
				inst[21+i]='0';
				rd/=2;
			}
			break;
		}
		case IO:
		{
			int rd=0,s=0;
			sscanf(code,"$%d,%d",&rd,&s);
			for(int i=4;i>=0;i--)
			{
				inst[6+i]='0';
				inst[11+i]='0';
				inst[16+i]=rd%2+'0';
				inst[21+i]=s%2+'0';
				rd/=2;
				s/=2;
			}
			break;	
		}
		default:return -1;
	}
	return 0;
}


void load_inst()
{
	FILE *p,*q;
	int flag=0;
	p=fopen(save_name,"rb");
	
	char temp[30]={0};

	cout <<endl<<"input the instruction PC: ";
	cin >>PC;
	SPC=PC;
	while(!feof(p))
	{
		fscanf(p,"%s",temp);
		int length=strlen(temp);
		if(temp[length-1]==':')
		{
			printf("%x\n",PC);
			Lable[num_label].temp[length]='\0';
			Lable[num_label].pc=PC;
			for(int i=0;i<length-1;i++)
				Lable[num_label].temp[i]=temp[i];
			num_label++;
			fscanf(p,"%s",temp);
		}

		fscanf(p,"%s",temp);
		PC+=4;		
	}
	fclose(p);
	PC=SPC;
	
	p=fopen(save_name,"rb");
	cout <<"input the name to store 01 file :";
	cin >>save_name;
	q=fopen(save_name,"wb");
	cout <<"hex: 1   binary :2"<<endl;
	cin >>flag;
	while(!feof(p))
	{
		fscanf(p,"%s",temp);
		int length=strlen(temp);
		if(temp[length-1]==':')
			fscanf(p,"%s",temp);

		funct=get_FUN(temp);
		if(funct==-1)
			continue;
		fscanf(p,"%s",temp);
		
		int flag1=get_else(temp);
		if(flag1==-1)
			continue;			
		if(flag==1)
		{
			int i=0;
			for(i=0;i<8;i++)
			{
				int temp=0;
				for(int a=0;a<4;a++)
				{
					temp*=2;
					temp+=(inst[a+i*4]-'0');
				}
				if(temp<10)
					inst[i]=temp+'0';
				else
					inst[i]=temp-10+'a';
			}
			inst[8]='\0';
		}
		fprintf(q,"%s\n",inst);
		PC+=4;		
	}
	fclose(q);
	fclose(p);
}





int main()
{
	uniform();
	load_inst();
	return 0;
}
