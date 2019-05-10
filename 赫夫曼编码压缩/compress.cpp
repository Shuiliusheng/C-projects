#include<iostream>
#include<stdio.h>
using namespace std;

typedef struct{
	char raw;
	int left,right;
}Node;

char code_char[256][20]={0};
Node node[10000];
int num_node=0;

int frequency[256]={0};
int node_char[256]={0};
FILE *read=NULL;
FILE *save=NULL;
//left 0; right 1

unsigned int filesize=0;

bool init(char name[],char name1[])
{
	read=fopen(name,"rb");
	save=fopen(name1,"wb");
	if(read==NULL)
	{
		cout <<"file "<<name<<" cann't be open!"<<endl;
		return false;
	}

	fseek(read,0,SEEK_END);
	filesize=ftell(read);
	fseek(read,0,SEEK_SET);

	for(int i=0;i<256;i++)
		node_char[i]=-2;
	return true;
}


void get_frequency()
{
	if(read==NULL)
		return ;
	fseek(read,0,SEEK_SET);
	while(!feof(read))
	{
		int t=fgetc(read);
		frequency[t]++;
	}
	return ;
}


int find_min_frequency()
{
	int i=0;
	int min=-1;
	int n=-1;
	for(i=0;i<256;i++)
	{
		if(frequency[i]!=0)
		{
			if(frequency[i]<min||min==-1)
			{
				min=frequency[i];
				n=i;
			}
		}
	}
	return n;
}


void make_tree()
{
	num_node=0;
	while(1)
	{
		int n1=find_min_frequency();
		if(n1==-1)
			break;
		int add=frequency[n1];
		frequency[n1]=0;
		if(node_char[n1]==-2)
		{
			node_char[n1]=num_node;
			node[num_node].raw=n1;
			node[num_node].left=-1;
			node[num_node++].right=-1;
		}
		
		int n2=find_min_frequency();
		if(n2==-1)
			break;
		add+=frequency[n2];
		frequency[n2]=0;
		
		if(node_char[n2]==-2)
		{
			node_char[n2]=num_node;
			node[num_node].raw=n2;
			node[num_node].left=-1;
			node[num_node++].right=-1;
			node[num_node].left=node_char[n2];
			node[num_node].right=node_char[n1];
		}
		else
		{
			node[num_node].left=node_char[n1];
			node[num_node].right=node_char[n2];
		}
		node_char[n1]=num_node;
		num_node++;
		frequency[n1]=add;
	}
}

char tempcode[20]={0};
int num=0;
void read_tree_code(int n)
{
	if(n>=num_node||n<0)
		return;
	if(node[n].left==-1&&node[n].right==-1)
	{
		for(int i=0;i<num;i++)
			code_char[(unsigned char)node[n].raw][i]=tempcode[i];
		code_char[(unsigned char)node[n].raw][num]='\0';
	//	printf("%3u %s\n",(unsigned char)node[n].raw,code_char[(unsigned char)node[n].raw]);
	}
	else
	{
		tempcode[num]='0';
		num++;
		read_tree_code(node[n].left);
		num--;
		tempcode[num]='1';
		num++;
		read_tree_code(node[n].right);
		num--;
	}
}

void show()
{
	for(int i=0;i<num_node;i++)
	{
		cout <<"NO: "<<i<<" "<<(int)node[i].raw<<endl;
		//cout <<(char)node[i].raw<<endl;
		cout <<"Left : "<<node[i].left<<endl;
		cout <<"Right: "<<node[i].right<<endl<<endl;
	}

}


void save_tree()
{
	char c='0';
	fwrite(&filesize,1,sizeof(int),save);
	fwrite(&c,1,sizeof(char),save);
	fwrite(&num_node,1,sizeof(int),save);
	cout <<num_node<<endl;
	for(int i=0;i<num_node;i++)
		fwrite(&node[i],1,sizeof(Node),save);
}

char transform_array(char str[])
{
	unsigned char t=0;
	for(int i=0;i<8;i++)
		t+=(((unsigned char)(str[i]-'0'))<<(7-i));
	return t;
}

void compress()
{
	save_tree();
	char temp[16]={0};
	int num=0;
	fseek(read,0,SEEK_SET);
	int t1=0,t2=0;
	while(!feof(read))
	{
		unsigned char t=0;
		fread(&t,1,sizeof(char),read);
		int l=strlen(code_char[t]);
		t1++;
		printf("%0.6lf\r",1.0*t1/filesize);
		for(int i=0;i<l;i++)
		{
			temp[num++]=code_char[t][i];
			if(num>=8)
			{
				num=0;
				temp[8]='\0';
				char c=transform_array(temp);
				t2++;

				fwrite(&c,1,sizeof(char),save);
			}
		}
	}
	cout <<endl;
	if(num!=0)
	{
		for(int i=num;i<8;i++)
			temp[i]='0';
		char c=transform_array(temp);
		t2++;
		fwrite(&c,1,sizeof(char),save);
		c=num;
		fseek(save,0,SEEK_SET);
		fwrite(&c,1,sizeof(char),save);
	}
	fclose(read);
	fclose(save);
}

int main()
{
	char name[200],name1[200];
	cout <<"input the name to compress: "<<endl;
	cin >>name;
	cout <<"input the name to save after compress:"<<endl;
	cin >>name1;
	init(name,name1);
	get_frequency();
	make_tree();
	read_tree_code(num_node-1);
	if(num_node==1)
	{
		code_char[(int)node[0].raw][0]='0';
		code_char[(int)node[0].raw][1]='\0';
	}
	compress();

	return 0;
}