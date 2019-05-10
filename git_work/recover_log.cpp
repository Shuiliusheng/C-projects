#include "stdafx.h"
extern LOG Log[100];
extern File file[30000];
extern char Dir[30000][400];
extern char log_file[1000];
extern char buffer[1000];
extern char workpath[1000];
extern int num_dir;
extern int num_file;
extern int log_num;
extern File temp_file[30000];
extern char temp_Dir[30000][400];
extern int tnum_dir;
extern int tnum_file;
extern char flag[30000];

extern int make_dir[30000];
extern int num_md;
extern int del_dir[30000];
extern int num_dd;

extern int make_file[30000];
extern int num_mf;
extern int del_file[30000];
extern int num_df;
extern char flag[30000];

char temp_Dir1[10000][400];
int tnum_dir1=0;

int read_flag=1;

/*
恢复记录，首先是恢复第一个记录，这个不是直接重做operator，首先删除所有文件
而是采用与当前文件比较，保持不变的，补全不一致的，删除多余的
之后则是根据log的operator文件进行重做即可
注意的是采用了变量的重用，没有多分配其它的空间
*/


bool read_dir_file_1(int log)//读取某一次的detail_log文件，结果读入temp1
{
	char filename[1000];
	sprintf(filename,"%s\\log_%d\\detail_log",log_file,log);
	FILE *p=NULL;
	p=fopen(filename,"r");
	if(p==NULL)
	{
		cout <<filename<<" is not exist!"<<endl;
		return false;
	}
	tnum_dir1=0;
	int i=0;
	fscanf(p,"%d",&tnum_dir1);
	tnum_dir1++;
	for(i=1;i<tnum_dir1;i++)
	{
		fscanf(p,"%s",temp_Dir1[i]);
	}
	fclose(p);
	return true;
}
bool read_dir_file_2(int log)//读取某一次的detail_log文件，结果读入temp
{
	char filename[1000];
	sprintf(filename,"%s\\log_%d\\detail_log",log_file,log);
	FILE *p=NULL;
	p=fopen(filename,"r");
	if(p==NULL)
	{
		cout <<filename<<" is not exist!"<<endl;
		return false;
	}
	tnum_dir=0;
	int i=0;
	fscanf(p,"%d",&tnum_dir);
	tnum_dir++;
	for(i=1;i<tnum_dir;i++)
	{
		fscanf(p,"%s",temp_Dir[i]);
	}
	fclose(p);
	return true;
}



bool recover_first_log()//将还原第一次记录单独提取出来，与当前作比较，删多的，补全少了的
{
	if(!read_dir_file(0))
	{
		return false;
	}
	strcpy(buffer,workpath);
	num_dir=1;
	num_file=0;

	char filename[1000];
	sprintf(filename,"%s\\log_%d\\log_constraint",log_file,0);
	if(!add_constraint_from_file(filename))
	{
		cout <<filename<<"  is shortage!"<<endl;
		return false;
	}
	printDir(buffer,0);

	find_different();
	int i=0;
	char name[1000];
	for(i=0;i<num_mf;i++)
	{
		strcpy(name,workpath);
		strcat(name,"\\");
		int t=file[make_file[i]].dir;
		if(t!=0)
		{
			strcat(name,Dir[t]);
			strcat(name,"\\");
		}
		strcat(name,file[make_file[i]].filename);
		remove(name);
		cout <<"remove:"<<name<<endl;
	}
	for(i=0;i<num_md;i++)
	{
		strcpy(name,workpath);
		strcat(name,"\\");
		strcat(name,Dir[make_dir[i]]);
		cout <<"remove:"<<name<<endl;
		RemoveDirectoryA(name);
	}
	for(i=0;i<num_dd;i++)
	{
		strcpy(name,workpath);
		strcat(name,"\\");
		strcat(name,temp_Dir[del_dir[i]]);
		cout <<"make:"<<name<<endl;
		_mkdir(name);
	}
	char src[1000],dst[1000];
	char log_dir[1000];
	sprintf(log_dir,"%s\\log_%d",log_file,0);

	for(i=0;i<num_df;i++)
	{
		int t=del_file[i];
		sprintf(src,"%s\\%d_%s",log_dir,temp_file[t].dir,temp_file[t].filename);
		if(temp_file[t].dir!=0)
			sprintf(dst,"%s\\%s\\%s",workpath,temp_Dir[temp_file[t].dir],temp_file[t].filename);
		else
			sprintf(dst,"%s\\%s",workpath,temp_file[t].filename);
		cout <<"src:"<<src<<endl;
		cout <<"dst:"<<dst<<endl;
		uncompress_file(src,dst);
	}
	return true;
}

bool work_operator_log(int log)//根据operator_log文件进行redo
{
	char filename[1000];
	sprintf(filename,"%s\\log_%d\\operator_log",log_file,log);
	FILE *p=NULL;
	p=fopen(filename,"r");
	if(p==NULL)
	{
		cout <<filename<<" is not exist!"<<endl;
		return false;
	}

	int op=0;
	int ndir=0;
	char dir[400];
	char name[1000];
	char src[1000];
	char log_dir[1000];
	sprintf(log_dir,"%s\\log_%d",log_file,log);

	while(!feof(p))
	{
		fscanf(p,"%d",&op);
		if(op==CLEAR)
			continue;
		else if(op==DEL_D)
		{
			fscanf(p,"%s",dir);
			strcpy(name,workpath);
			strcat(name,"\\");
			strcat(name,dir);
			RemoveDirectoryA(name);
			cout <<"remove:"<<name<<endl;
		}
		else if(op==MK_D)
		{
			fscanf(p,"%s",dir);
			strcpy(name,workpath);
			strcat(name,"\\");
			strcat(name,dir);
			_mkdir(name);
			cout <<"make:"<<name<<endl;
		}
		else if(op==DEL_F)
		{
			fscanf(p,"%d",&ndir);
			fscanf(p,"%s",dir);

			if(ndir!=0)
			{
				if(read_flag%2==1)
					sprintf(name,"%s\\%s\\%s",workpath,temp_Dir[ndir],dir);
				else
					sprintf(name,"%s\\%s\\%s",workpath,temp_Dir1[ndir],dir);
			}
			else
				sprintf(name,"%s\\%s",workpath,dir);
			remove(name);
			cout <<"remove:"<<name<<endl;
		}
		else if(op==MK_F)
		{
			fscanf(p,"%d",&ndir);
			fscanf(p,"%s",dir);
			
			sprintf(src,"%s\\%d_%s",log_dir,ndir,dir);
			
			if(ndir!=0)
			{
				if(read_flag%2==1)
					sprintf(name,"%s\\%s\\%s",workpath,temp_Dir1[ndir],dir);
				else
					sprintf(name,"%s\\%s\\%s",workpath,temp_Dir[ndir],dir);
			}
			else
				sprintf(name,"%s\\%s",workpath,dir);
			uncompress_file(src,name);
			cout <<"src:"<<src<<endl;
			cout <<"dst:"<<ndir<<" "<<name<<endl;
		}
	}
	fclose(p);
	return true;
}

bool recovery_one_log(int log)//恢复某一次log，前提是当前环境为log-1次的环境
{
	if(read_flag%2==1)
	{
		if(!read_dir_file_1(log))
			return false;
	}
	else
	{
		if(!read_dir_file_2(log))
			return false;
	}
	if(!work_operator_log(log))
		return false;
	read_flag++;
	return true;
}

bool recovery_log(int log)//恢复记录文件，log为记录的编号
{
	if(log>=log_num||log<0||Log[log].log_num==-1)
	{
		cout <<"don't find this log"<<endl;
		return false;
	}
	if(!recover_first_log())
	{
		cout <<"the first log is bad!"<<endl;
		return false;
	}
	if(log==0)
	{
		return true;
	}
	cout <<"recovery log -> "<<0<<endl<<endl;
	for(int i=1;i<=log;i++)
	{
		if(!recovery_one_log(i))
		{
			cout <<"recovery log "<<i<<" wrong"<<endl;
			return false;
		}
		cout <<"recovery log -> "<<i<<endl<<endl;
	}
	char filename[1000];
	sprintf(filename,"%s\\log_%d\\log_constraint",log_file,log);
	if(!add_constraint_from_file(filename))
	{
		cout <<filename<<"  is shortage!"<<endl;
		return false;
	}
	return true;
}

void del_log(char path[])//删除特定路径下的所有文件包括本身
{
	char rawpath[1000];
	strcpy(rawpath,path);
	strcat(path,"\\*.*");
    struct _finddata_t data;
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
			if(data.name[0]!='.')
			{
				char tpath[1000];
				strcpy(tpath,rawpath);
				strcat(tpath,"\\");
				strcat(tpath,data.name);
				del_log(tpath);
			}
		}
        else
        {
			char tpath[1000];
			strcpy(tpath,rawpath);
			strcat(tpath,"\\");
			strcat(tpath,data.name);
			cout <<"del:"<<tpath<<" "<<remove(tpath)<<endl;;
		}
        nRet = _findnext( hnd, &data );
    }
	cout <<"del:"<<rawpath<<" "<<RemoveDirectoryA(rawpath)<<endl;;
    _findclose( hnd );     // 关闭当前句柄
}

void del_git()//删除git创建的文件
{
	char path[1000];
	_getcwd(path,1000);  
	strcat(path,"\\git_log");
	del_log(path);
}