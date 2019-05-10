#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include<iostream>
using namespace std;

#define END 5
#define BEGIN 2
#define DEL_LINE 4
#define REPLACE 3
#define SAVE 19
#define PGEND -2
#define PGHOME -3
#define FIND 6
#define ESC 27
#define QUIT 17

#define FUC_F 0
#define F1 59
#define F2 60



char str_find[100]={0};
char buf[1000][1000]={0};
int buf_y=0;
int buf_x[1000]={0};

char getInput();
void del_y(int s);
void show();
void move_y(int s,int e);
void add_char(char in,int x,int y);
void replace(char src[],char dst[]);
void choice(int ch,int x,int y);
int find_str(int &x,int &y);
void init(char name[]);

HANDLE hout;
COORD coord; //屏幕上的坐标
COORD coord1; 


int fx=0,fy=0;
int findx=0,findy=0;
int over=0;

char filename[200]={0};
int fileflag=0;

int max=0;
int now=0;

int first_line=0;
int end_line=0;


int test=0;

void test_info()
{
	//cout <<endl<<test<<" "<<coord.X<<" "<<coord.Y<<endl;
}


void save_now()
{
	char name[100];
	sprintf(name,"edit_temp_%03d",now);
	FILE *p=fopen(name,"w+");
	for(int i=0;i<=buf_y;i++)
	{
		buf[i][buf_x[i]]='\0';
		fprintf(p,"%s\n",buf[i]);
	}
	fclose(p);
}

void add_buf(int n)
{
	char name[100];
	sprintf(name,"edit_temp_%03d",n);
	FILE *p=fopen(name,"r");
	
	buf_y=0;
	while(!feof(p)&&buf_y<1000)
	{
		memset(buf[buf_y],0,1000);
		fgets(buf[buf_y],1000,p);
		buf_x[buf_y]=strlen(buf[buf_y]);
		buf[buf_y][buf_x[buf_y]-1]='\0';
		cout <<buf[buf_y]<<" "<<buf_y<<endl;
		buf_y++;
	}
	fclose(p);
	buf_y-=1;
}


void init(char name[])
{
	FILE *p=NULL;
	p=fopen(name,"r+");
	if(p==NULL)
	{
		cout <<name<<" is not exist!"<<endl;
		return ;
	}
	
	fileflag=1;
	int t=0;
	FILE *file=NULL;
	char temp[1000];
	char input[1000]={0};
	while(!feof(p))
	{
		memset(input,0,1000);
		fgets(input,1000,p);
		// cout <<input;
		if(t%42==0)
		{
			sprintf(temp,"edit_temp_%03d",max);
			max++;
			if(file!=NULL)
				fclose(file);
			file=fopen(temp,"w+");
		}
		fprintf(file,"%s",input);
		t++;
	}
	fclose(p);
	fclose(file);
	
	max-=1;
	now=0;
	
	sprintf(temp,"edit_temp_%03d",now);
	p=fopen(temp,"r");
	
	while(!feof(p)&&buf_y<1000)
	{
		fgets(buf[buf_y],1000,p);
		buf_x[buf_y]=strlen(buf[buf_y]);
		buf[buf_y][buf_x[buf_y]-1]='\0';
		buf_y++;
	}
	fclose(p);
	buf_y-=1;
	show();
	cout <<endl<<"this page line : "<<buf_y<<endl;
	cout <<"the total line : "<<t<<endl;
}

int main()
{
    hout=GetStdHandle(STD_OUTPUT_HANDLE);
    int t=0;
	cout <<"open file : 1  ||  new file 0"<<endl<<"-->";
	cin >>t;
	if(t==1)
	{
		cout <<"input the file name:";
		cin >>filename;
		init(filename);
	}
	else
	{
		show();
	}
	
	while(1)
    {
        char ch=getInput();
		if(ch==-1)
			break;	   
    } 
    getchar();
	
	char temp[100];
	for(int i=0;i<=max;i++)
	{
		sprintf(temp,"edit_temp_%03d",i);
		remove(temp);
	}

	return 0;
}

char getInput()
{ 
    CONSOLE_SCREEN_BUFFER_INFO csbi; //控制台屏幕缓冲区信息
 
    coord.X=10;
    coord.Y=10;

	GetConsoleScreenBufferInfo(hout,&csbi);
    coord.X=csbi.dwCursorPosition.X; //得到坐标X的值
    coord.Y=csbi.dwCursorPosition.Y+first_line; //得到坐标Y的值

	int ch=0;
    ch=getch();
	
//	cout <<ch<<endl;
    //0x0d表示回车，0XE0表示上下左右等键的键码
    while(ch==0xE0)
    {
        //上
		ch=getch();
		test=ch;
        if(ch==0x48)
        {
            if(coord.Y!=0)
				coord.Y--;
			coord.Y-=first_line;
			if(coord.Y<0)
				coord.Y=0;
			SetConsoleCursorPosition(hout,coord);
			coord.Y+=first_line;
        }
        //下
        else if(ch==0x50)
        {
            coord.Y++;
			coord.Y-=first_line;
			if(coord.Y<0)
				coord.Y=0;
			SetConsoleCursorPosition(hout,coord);
			coord.Y+=first_line;
        }
        //左
        else if(ch==0x4b)
        {
            if(coord.X!=0)
				coord.X--;
			SetConsoleCursorPosition(hout,coord);
        }
        //右
        else if(ch==0x4d)
        {
            if(coord.X!=79)
				coord.X++;
            else
            {
                coord.X=0;
                coord.Y++;
				coord.Y-=first_line;
				if(coord.Y<0)
					coord.Y=0;
            }
			SetConsoleCursorPosition(hout,coord);
			coord.Y+=first_line;
        }
		else if(ch==73)
		{
			//cout <<"pgup"<<endl;
			if(first_line>0)
			{
				coord.Y-=10;
				if(coord.Y<0)
					coord.Y=0;
				show();
			}
			else
			{
				if(now>0)
				{
					save_now();
					add_buf(now-1);
					show();
					cout <<endl<<"this page line : "<<buf_y<<endl;
					now=now-1;
				}
			}
		}
		else if(ch==81)
		{
		//	cout <<"pgdn"<<endl;
			
			if(end_line<buf_y)
			{
				coord.Y+=10;
				show();
			}
			else
			{
				if(now<max)
				{
					save_now();
					add_buf(now+1);
					show();
					cout <<endl<<"this page line : "<<buf_y<<endl;
					now=now+1;
				}
			}
		}
		else if(ch==79)
		{
			cout <<"end"<<endl;
			ch=-2;
			break;
		}
		else if(ch==71)
		{
		//	cout <<"home"<<endl;
			ch=-3;
			break;
		}
		
        ch=getch();
    }
	int x=coord.X;
	int y=coord.Y;
	
	if(ch>126)
	{
		for(int i=buf_x[y];i>=x;i--)
		{
			buf[y][i+2]=buf[y][i];
		}
		buf_x[y]+=2;
		buf[y][x]=ch;
		buf[y][x+1]=getch();
		fx=x+2;
  		fy=y;
	}
	else
	{
		choice(ch,x,y);
	}

	if(over==1)
		return -1;
	if(fx==-1)
		fx=0;
	
	coord.X=fx;
	coord.Y=fy;
	show();
	coord.Y-=first_line;
	if(coord.Y<0)
		coord.Y=0;
 	SetConsoleCursorPosition(hout,coord);
	
	if(buf_y>10&&now==max&&fileflag==0)
	{
		save_now();
		now++;
		max++;
		for(int i=0;i<=buf_y;i++)
		{
			buf_x[i]=0;
			memset(buf[i],0,1000);
		}
		buf_y=0;
		system("cls");
	}

    return ch;
}

void show()
{
	int s=coord.Y-20;
	int e=coord.Y+21;
	if(s<0)
	{
		s=0;
		e+=(20-coord.Y);
	}
	if(s>buf_y)
		s=buf_y-41;
	if(e>buf_y)
		e=buf_y;
	
	system("cls");
	for(int i=s;i<e;i++)
	{
		for(int c=0;c<buf_x[i];c++)
		{
			printf("%c",buf[i][c]);
		}
		printf("\n");
	}
	for(int c=0;c<buf_x[e];c++)
	{
		printf("%c",buf[e][c]);
	}
	first_line=s;
	end_line=e;
	test_info();
}

void save_file(char name[])
{
	save_now();
	FILE *p=fopen(name,"w+");
	char temp[1000]={0};
	for(int i=0;i<=max;i++)
	{
		FILE *q=NULL;
		sprintf(name,"edit_temp_%03d",i);
		q=fopen(name,"r+");
		if(q==NULL)
		{
			cout <<"file is damage!"<<endl;
			return ;
		}
		
		while(!feof(q))
		{
			memset(temp,0,1000);
			fgets(temp,1000,q);
			fprintf(p,"%s",temp);
		}
		fclose(q);
	}
	fclose(p);
}

void choice(int ch,int x,int y)
{
	if(ch==9)//tab
	{
		ch=' ';
		for(int i=0;i<8;i++)
			add_char(ch,x+i,y);
	}
	else if(ch==END)
	{
		fx=buf_x[y];
		fy=y;
	}
	else if(ch==BEGIN)
	{
		fx=0;
		fy=y;
	}
	else if(ch==DEL_LINE)
	{
		del_y(y);
		fx=buf_x[y-1];
		fy=y-1;
	}
	else if(ch==REPLACE)
	{
		coord.X=0;
		coord.Y=buf_y+2;
		SetConsoleCursorPosition(hout,coord);

		char dst[100];
		char src[100];
		cout <<"src: ";
		scanf("%s",src);
		cout <<"dst: ";
		scanf("%s",dst);
		
		replace(src,dst);
	}
	else if(ch==SAVE)
	{
		coord.X=0;
		coord.Y=buf_y+2;
		SetConsoleCursorPosition(hout,coord);

		char name[100]="";
		cout <<"the save file name: ";
		scanf("%s",name);
		
		save_file(name);
		
	}
	else if(ch==FIND)
	{
		coord.X=0;
		coord.Y=buf_y+2;
		SetConsoleCursorPosition(hout,coord);
		cout <<"input the string to find: ";
		cin >>str_find;
		
		findx=0;
		findy=0;

		if(find_str(findx,findy)==1)
		{
			fx=findx;
			fy=findy;
		}
		else
		{
			cout <<str_find<<" is not find!"<<endl;
		}
	}
	else if(ch==FUC_F)
	{
		ch=getch();
		if(ch==F1)
		{
			
			if(find_str(findx,findy)==1)
			{
				fx=findx;
				fy=findy;
				findx+=strlen(str_find);
			}
			else
			{
				findx=0;
				findy=0;
			}
		}
		else if(ch==F2)
		{
			coord.X=0;
			coord.Y=buf_y+2;
			SetConsoleCursorPosition(hout,coord);
			cout <<"----------help info-------------"<<endl;
			cout <<"ctrl+d      删除一行"<<endl;
			cout <<"ctrl+c      替换"<<endl;
			cout <<"ctrl+f      查找"<<endl;
			cout <<"F1          查找继续查找下一个"<<endl;
			cout <<"F2          帮助信息"<<endl;	
			cout <<"ctrl+e      行末尾"<<endl;
			cout <<"ctrl+b      行开始"<<endl;
			cout <<"ctrl+s      保存文件"<<endl;
			cout <<"home        第一行开始"<<endl;
			cout <<"end         最后一行结尾"<<endl;
			cout <<"input enter quit: ";
			getch();
		}
	}
	else if(ch==ESC)
	{
		over=1;
		coord.X=0;
		coord.Y=buf_y+2;
		SetConsoleCursorPosition(hout,coord);
		char t=0;
		if(fileflag==1)
		{
			cout <<"write back to "<<filename<<" ? (Y/N)  ->:"<<endl;
			cin >>t;
			if(t=='Y'||t=='y')
			{
				save_file(filename);
			}
		}
		else
		{
			cout <<"save file ? (Y/N)  ->:"<<endl;
			cin >>t;
			if(t=='Y'||t=='y')
			{
				char name[100]="";
				cout <<"the save file name: ";
				scanf("%s",name);
				
				save_file(name);
			}
		}
	}
	else
	{
		add_char(ch,x,y);
	}
}


void add_char(char in,int x,int y)
{
	int i=0,c=0;
	if(x>=buf_x[y]&&in!='\n'&&in!='\r'&&in!=8&&in!=PGHOME&&in!=PGEND)
	{
		buf_x[y]++;
		buf[y][x]=in;
		fx=x+1;
		fy=y;
		
		return ;
	}
	
	if(in=='\n'||in=='\r')
	{
		move_y(y,buf_y);
		buf_y++;
		
		for(i=x;i<buf_x[y];i++)
		{
			buf[y+1][i-x]=buf[y][i];
			buf[y][i]=0;
			
		}
		buf_x[y]=x;
		buf_x[y+1]=i-x;

		fx=0;
		fy=y+1;
		return ;
	}

	if(in==8)//backspace 
	{
		if(x>0)
		{
			if((int)(unsigned char)buf[y][x-2]<=126)
			{
				for(i=x-1;i<buf_x[y];i++)
				{
					buf[y][i]=buf[y][i+1];
					buf[y][i+1]=0;
				}
				if(i!=x-1)
				{
					buf_x[y]--;
					fx=x-1;
 					fy=y;
				}
			}
			else
			{
				for(i=x-2;i<buf_x[y];i++)
				{
					buf[y][i]=buf[y][i+2];
					buf[y][i+2]=0;
				}
				if(i!=x-2)
				{
					buf_x[y]-=2;
					fx=x-2;
 					fy=y;
				}
			}
			if(x>=buf_x[y])
			{
				fx=buf_x[y];
				fy=y;
			}
		}
		else if(x==0&&y>0)
		{
			fx=buf_x[y-1];
			fy=y-1;
			for(int i=0;i<buf_x[y];i++)
				buf[y-1][i+fx]=buf[y][i];
			buf_x[y-1]+=buf_x[y];

			del_y(y);

		}
		return ;

	}
	
	if(in==PGEND)
	{
		fx=buf_x[buf_y];
		fy=buf_y;
		return ;
	}
	if(in==PGHOME)
	{
		fx=0;
		fy=0;
		return ;
	}

	for(i=buf_x[y];i>=x;i--)
	{
		buf[y][i+1]=buf[y][i];
	}
	buf_x[y]++;
	buf[y][x]=in;
	fx=x+1;
  	fy=y;
}



void show1()
{
	system("cls");
	for(int i=0;i<buf_y;i++)
	{
		for(int c=0;c<buf_x[i];c++)
		{
			printf("%c",buf[i][c]);
		}
		printf("\n");
	}
	for(int c=0;c<buf_x[buf_y];c++)
	{
		printf("%c",buf[buf_y][c]);
	}
}

void del_y(int s)
{
	for(int i=s;i<buf_y;i++)
	{
		for(int c=0;c<1000;c++)
		{
			buf[i][c]=buf[i+1][c];
		}
		buf_x[i]=buf_x[i+1];
	}
	buf_y--;
}


void move_y(int s,int e)
{
	if(e>=1000)
	{
		cout <<"the y is max!"<<endl;
		return ;
	}
	
	for(int i=e;i>s;i--)
	{
		for(int c=0;c<1000;c++)
		{
			buf[i+1][c]=buf[i][c];
		}
		buf_x[i+1]=buf_x[i];
	}
	for(int c=0;c<1000;c++)
		buf[s+1][c]=0;
	buf_x[s+1]=0;

}
int find_str(int &x,int &y)
{
	int l=strlen(str_find);
	if(l==0)
		return 0;
	
	int i=0,j=0,c=0,n=0;
	
	for(n=y;n<=buf_y;n++)
	{
		if(n==y)
			i=x;
		else
			i=0;
		for(;i<buf_x[n];i++)
		{
			for(j=0;j<l;j++)
			{
				if(str_find[j]!=buf[n][i+j])
					break;
			}
			if(j==l)
			{
				x=i;
				y=n;
				return 1;
			}
		}
	}
	return 0;
}
void replace(char src[],char dst[])
{
	char temp[1000]={0};
	int num=0;
	
	int ls=strlen(src);
	int ld=strlen(dst);
	
	int i=0,j=0,c=0,n=0;
	
	for(n=0;n<=buf_y;n++)
	{
		num=0;
		for(i=0;i<buf_x[n];i++)
		{
			for(j=0;j<ls;j++)
			{
				if(src[j]!=buf[n][i+j])
					break;
			}
			if(j==ls)
			{
				for(c=0;c<ld;c++)
					temp[num++]=dst[c];
				i=i+j-1;
				continue;
			}
			temp[num++]=buf[n][i];
		}
		for(i=0;i<num;i++)
			buf[n][i]=temp[i];
		buf_x[n]=num;
	}
	
}