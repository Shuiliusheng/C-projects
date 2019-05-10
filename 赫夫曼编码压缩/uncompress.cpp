#include<iostream>
#include<stdio.h>
using namespace std;

typedef struct{
	char raw;
	int left,right;
}Node;

Node node[10000];
int num_node=0;

FILE *read=NULL;
FILE *save=NULL;
char endnum=0;
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
	fread(&filesize,1,sizeof(int),read);
	fread(&endnum,1,sizeof(char),read);
	return true;
}

void read_tree()
{
	fread(&num_node,1,sizeof(int),read);
	cout <<"num_node:"<<num_node<<endl;
	for(int i=0;i<num_node;i++)
	{
		fread(&node[i],1,sizeof(Node),read);
	}
	
}


void show()
{
	for(int i=0;i<num_node;i++)
	{
		cout <<"NO: "<<i<<" "<<(int)node[i].raw<<endl;
		cout <<"Left : "<<node[i].left<<endl;
		cout <<"Right: "<<node[i].right<<endl<<endl;
	}

}
char tempchar[9]={0};
int num_char=0;
int end=8;

unsigned int num=0,num1=0;

bool read_char()
{
	unsigned char t1=0,t2=0;
	if(!feof(read))
	{
		fread(&t1,1,sizeof(char),read);
		int flag=fread(&t2,1,sizeof(char),read);
		if(flag!=0)
		{
			fseek(read,-1,SEEK_CUR);	
		}
		for(int i=7;i>=0;i--)
		{
			tempchar[i]='0'+t1%2;
			t1=t1/2;
		}
		num++;
	}
	else
	{
		return false;
	}
	if(feof(read))
	{
		end=endnum+1;
	}
	else
		end=8;
	return true;
}

void travel_tree(int n)
{
	if(num_char>=end)
	{
		if(read_char())
		{
			num_char=0;
		}
		else
		{
			return ;
		}
	}
	if(n>=num_node||n<0)
	{
		return;
	}
	if(node[n].left==-1&&node[n].right==-1)
	{
		//if(num1<filesize)
		num1+=fwrite(&node[n].raw,1,sizeof(char),save);
		printf("%0.6lf\r",1.0*num1/filesize);
	}
	else
	{
		if(tempchar[num_char++]=='0')
		{
			travel_tree(node[n].left);
		}
		else
		{
			travel_tree(node[n].right);
		}
	}
}

void uncompress()
{
	read_tree();
	if(read_char())
		num_char=0;
	while(!feof(read)||(feof(read)&&num_char<end))
	{
		travel_tree(num_node-1);
	}
	cout <<endl;
}


int main()
{
	char name[200],name1[200];
	cout <<"input the compressed file: "<<endl;
	cin >>name;
	cout <<"input the name to save after uncompress:"<<endl;
	cin >>name1;
	init(name,name1);
	uncompress();
	fclose(read);
	fclose(save);
	return 0;
}
