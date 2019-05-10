#include"stdafx.h"
/*
用于在压缩之前的准备工作：
1. 添加压缩的文件
2. 删除压缩的文件
3. 添加压缩的目录
4. 删除压缩的目录
5. 去除不压缩的文件
6. 去除不压缩的目录
7. 查找当前情况下会被压缩的所有文件
8. 显示当前添加的文件和目录
*/


char Dir[30000][1000];
int num_dir;

FIle File[30000];
int num_file;

char useless_Dir[1000][500];
char useless_File[1000][1000];
int num_ud;
int num_uf;

char workpath[1000];
char buffer[1000];
int raw_length;

void init()//压缩之前的初始化，变量之类的赋值
{
	num_dir=1;
	num_file=0;
	num_ud=0;
	num_uf=0;

	 _getcwd(buffer,1000);  
	strcpy(workpath,buffer);
	raw_length=strlen(buffer);

}

void copy_path(char *path,int dir)//复制相对路径
{
	int l=strlen(path);
	int c=0;
	for(int i=raw_length+1;i<=l;i++)
	{
		if(c<500)
			Dir[dir][c++]=path[i];
	}
}

void del_add_file(char name[])//删除已经添加的文件
{
	int i=0;
	if(num_file==0)
	{
		cout <<"there is not file add"<<endl;
		return ;
	}
	for(i=0;i<num_file;i++)
		if(strcmp(name,File[i].name)==0)
			break;
	if(i==num_file)
	{
		cout <<name<<" is not add"<<endl;
		return ;
	}
	for(int c=i+1;c<num_file;c++)
	{
		strcpy(File[c-1].name,File[c].name);
		File[c-1].ndir=0;
	}
	num_file--;
	cout <<name<<" is delete"<<endl;
}

void del_add_dir(char name[])//删除已经添加的目录
{
	int i=0;
	if(num_dir==1)
	{
		cout <<"there is not dir add"<<endl;
		return ;
	}
	for(i=0;i<num_dir;i++)
		if(strcmp(name,Dir[i])==0)
			break;
	if(i==num_dir)
	{
		cout <<name<<" is not add"<<endl;
		return ;
	}
	for(int c=i+1;c<num_dir;c++)
	{
		strcpy(Dir[c-1],Dir[c]);
	}
	num_dir--;
	cout <<name<<" is delete"<<endl;
}


void add_compress_file()//添加要压缩的文件
{
	int i=0;
	int num=0;
	cout <<"attention: the dir and the file is under this dir!"<<endl;
	cout <<"input the file num:";
	cin >>num;
	getchar();
	for(i=0;i<num;i++)
	{
		cout <<"No "<<i+1<<" :";
		gets(File[num_file].name);
		if(judge_file_exist(File[num_file].name))
		{
			File[num_file].ndir=0;
			num_file++;
		}
		else
		{
			i--;
		}
	}
	cout <<endl;
}

void add_useless_file()//添加无用的文件，即不被压缩
{
	int i=0;
	int num=0;
	cout <<"attention: the dir and the file is under this dir!"<<endl;
	cout <<"input the file num:";
	cin >>num;
	getchar();
	for(i=0;i<num;i++)
	{
		cout <<"No "<<i+1<<" :";
		gets(useless_File[num_uf]);
		if(judge_file_exist(useless_File[num_uf]))
		{
			num_uf++;
		}
		else
		{
			i--;
		}
	}
	cout <<endl;
}

void add_compress_dir()//添加要被压缩的目录
{
	int i=0;
	int num=0;
	cout <<"attention: the dir and the file is under this dir!"<<endl;
	cout <<"input the dir num:";
	cin >>num;
	getchar();
	for(i=0;i<num;i++)
	{
		cout <<"No "<<i+1<<" :";
		gets(Dir[num_dir]);
		if(judge_dir_exist(Dir[num_dir]))
			num_dir++;
		else
		{
			i--;
		}
	}
	cout <<endl;
}

void add_useless_dir()//添加不会被压缩的目录
{
	int i=0;
	int num=0;
	cout <<"attention: the dir and the file is under this dir!"<<endl;
	cout <<"input the dir num:";
	cin >>num;
	getchar();
	for(i=0;i<num;i++)
	{
		cout <<"No "<<i+1<<" :";
		gets(useless_Dir[num_ud]);
		if(judge_dir_exist(useless_Dir[num_ud]))
		{
			num_ud++;
		}
		else
		{
			i--;
		}
	}
	cout <<endl;
}

void precompress_dir_file()//文件添加之后的目录遍历
{
	char temp[1000];
	if(num_dir!=1)
	{
		int t=num_dir;
		for(int i=1;i<t;i++)
		{
			strcpy(temp,buffer);
			strcat(temp,"\\");
			strcat(temp,Dir[i]);
			printDir(temp,i);
		}
	}
	else if(num_file!=0)
	{

	}
	else
	{
		printDir(buffer,0);
	}
}

void show_dir()//显示已经添加的目录和文件
{
	cout <<"DIR:"<<num_dir-1<<endl;
	cout <<"\\"<<endl;
	for(int i=1;i<num_dir;i++)
		cout <<Dir[i]<<endl;
	cout <<endl<<"filename:"<<num_file<<endl;
	for(int c=0;c<num_file;c++)
	{
		cout <<Dir[File[c].ndir]<<"\\";
		cout <<File[c].name<<endl;
	}
	cout <<endl;
	cout <<"DIR :"<<num_dir-1<<"  File: "<<num_file<<endl;
}

void printDir(char* path,int dir)//循环查找所有文件
{
	char rawpath[1000];
	strcpy(rawpath,path);
	strcat(path,"\\*.*");
    struct _finddata_t data;
	char temp[1000];
    long hnd = _findfirst(path, &data );    // 查找文件名与正则表达式chRE的匹配第一个文件
    if ( hnd < 0 )
    {
        perror( path );
		return ;
    }
    int  nRet = 1;
    while ( nRet >= 0 )
    {
        if ( data.attrib == _A_SUBDIR )  // 如果是目录
        {
			if(data.name[0]!='.'&&!(dir==0&&strcmp(data.name,"git_log")==0))
			{
				char tpath[1000];
				strcpy(tpath,rawpath);
				strcat(tpath,"\\");
				strcat(tpath,data.name);
				copy_path(tpath,num_dir);
				if(!judge_dir_useless(Dir[num_dir]))
				{
					num_dir++;
					printDir(tpath,num_dir-1);
				}
			}
		}
        else
        {
			if(num_uf!=0)
			{
				strcpy(temp,Dir[dir]);
				if(dir!=0)
				{	
					strcat(temp,"\\");
				}
				strcat(temp,data.name);
			}
			if(!judge_file_useless(temp))
			{
				File[num_file].ndir=dir;
				strcpy(File[num_file].name,data.name);
				num_file++;
			}
		}
        nRet = _findnext( hnd, &data );
    }
    _findclose( hnd );     // 关闭当前句柄
}


bool judge_dir_useless(char dir[])//判断路径是否被限定为无用
{
	int i=0;
	for(i=0;i<num_ud;i++)
	{
		if(strcmp(dir,useless_Dir[i])==0)
			return true;
	}
	return false;
}

bool judge_file_useless(char filename[])//判断文件是否被限定为无用
{
	int i=0;
	for(i=0;i<num_uf;i++)
	{
		if(strcmp(filename,useless_File[i])==0)
			return true;
	}
	return false;
}

bool judge_dir_exist(char path[])//判断路径是否存在
{
	char rawpath[1000];
	strcpy(rawpath,workpath);
	strcat(rawpath,"\\");
	strcat(rawpath,path);
	strcat(rawpath,"\\*.*");
    struct _finddata_t data;
    long hnd = _findfirst(rawpath, &data );   
    if ( hnd < 0 )
	{
		cout <<path<<" is not exist"<<endl;
		return false;
    }
	for(int i=1;i<num_dir;i++)
		if(strcmp(path,Dir[i])==0)
		{
			cout <<path<<" is already existing!"<<endl;
			return false;
		}

	return true;
}

bool judge_file_exist(char name[])//判断文件是否存在
{
	char rawpath[1000];
	strcpy(rawpath,workpath);
	strcat(rawpath,"\\");
	strcat(rawpath,name);
	FILE *p=NULL;
	p=fopen(rawpath,"r");
	if(p==NULL)
	{
		cout <<name<<" is not exist"<<endl;
		return false;
	}
	fclose(p);

	for(int i=0;i<num_file;i++)
		if(strcmp(name,File[i].name)==0)
		{
			cout <<name<<" is already existing!"<<endl;
			return false;
		}

	return true;
}

