#include"stdafx.h"
/*
解压缩文件
能够实现解压当个目录
解压当个文件
解压所有文件

根据压缩是的结构进行解压

解压之前预处理：
判断解压文件是否正常
读取压缩文件中的目录及文件信息

*/

extern char Dir[30000][1000];
extern int num_dir;

extern FIle File[30000];
extern int num_file;

extern char useless_Dir[1000][500];
extern char useless_File[1000][1000];
extern int num_ud;
extern int num_uf;

extern char workpath[1000];
extern char buffer[1000];

extern char data[MAX_CP+10000];
extern char tempdata[MAX_CP+10000];

extern unsigned int record_place;
extern unsigned int total_size;

char compressfile[1000];


bool pre_uncompress_work(char filename[])//解压之前的预处理
{
	int i=0;
	FILE *read_file=NULL;
	read_file=fopen(filename,"r");
	if(read==NULL)
	{
		cout <<filename<<" is not exist"<<endl;
		return false;
	}
	unsigned int size=get_filesize(read_file);
	if(size<20)
	{
		cout <<filename<<" is damaged"<<endl;
		return false;
	}
	strcpy(compressfile,filename);
	fseek(read_file,size-20,SEEK_SET);
	fscanf(read_file,"%d",&record_place);
	if(record_place>=size)
	{
		cout <<filename<<" is damaged"<<endl;
		return false;
	}
	size=ftell(read_file);

	fseek(read_file,record_place,SEEK_SET);

	fread(&num_dir,sizeof(int),1,read_file);
	if(num_dir<1||num_dir>30000)
	{
		cout <<filename<<" is damaged"<<endl;
		return false;
	}

	for(i=1;i<num_dir;i++)
	{
		//fscanf(read_file,"%s",Dir[i]);
		fgets(Dir[i],1000,read_file);
		Dir[i][strlen(Dir[i])-1]='\0';
	}
	fscanf(read_file,"%d",&num_file);

	if(num_file<0||num_file>30000)
	{
		cout <<filename<<" is damaged"<<endl;
		return false;
	}
	for(i=0;i<num_file;i++)
	{
		fscanf(read_file,"%d %d %d %d",&File[i].ndir,&File[i].place_raw,&File[i].place_c,&File[i].block);
		fgetc(read_file);
		fgets(File[i].name,1000,read_file);
		File[i].name[strlen(File[i].name)-1]='\0';
	}
	fseek(read_file,0L,SEEK_SET);
	fclose(read_file);
	return true;
}

int find_File_place(char filename[])//根据输入的文件名称，获取编号
{
	char temp[1000];
	for(int i=0;i<num_file;i++)
	{
		strcpy(temp,Dir[File[i].ndir]);
		if(File[i].ndir!=0)
			strcat(temp,"\\");
		strcat(temp,File[i].name);
		if(strcmp(temp,filename)==0)
			return i;
	}
	return -1;
}

bool judge_file_not_exist(char name[])//判断文件是否纯在，是目的文件有没有
{
	FILE *p=NULL;
	p=fopen(name,"r");
	if(p==NULL)
	{
		return true;
	}
	fclose(p);
	cout <<name<<" is exist"<<endl;
	return false;
}

bool uncompress_work_file(int index,char dst[])//解压缩特定的文件，index代表数组中的索引
{
	if(!judge_file_not_exist(dst))
	{
		return false;
	}

	FILE *p=NULL;
	p=fopen(dst,"wb");//binary方式打开,important
	if(p==NULL)
	{
		cout <<dst<<" can't be created"<<endl;
		return false;
	}

	FILE *read_file=fopen(compressfile,"rb");
	unsigned int start=0;
	unsigned int end=0;
	unsigned int block_place=File[index].place_c;

	fseek(read_file,File[index].place_c,SEEK_SET);
	int lsrc=0,ldst=0;
	for(int i=0;i<File[index].block;i++)
	{
		show_percent(i,File[index].block);
		int t=0;
		if(i==0)//设置data中的开始位置
		{
			start=File[index].place_raw;
		}
		else
			start=0;

		fread(&t,sizeof(int),1,read_file);
		//fscanf(read_file,"%d",&t);
		if(t<0)
			lsrc=t*-1;
		else
			lsrc=t;
		//cout <<"lsrc:"<<t<<endl;
		lsrc=fread(data,sizeof(char),lsrc,read_file);
		if(t>0)
		{
			if(!uncompress_str(data,lsrc,tempdata,ldst))
			{
				fclose(p);
				cout <<"File: "<<File[index].name<<" uncompress wrong!"<<endl;
				return false;
			}
			if(i==File[index].block-1)//设置data中要读到的结束位置
			{
				if(index==num_file-1)
					end=ldst;
				else
					end=File[index+1].place_raw;
			}
			else
				end=ldst;
			fwrite(&tempdata[start],sizeof(char),end-start,p);
		}
		else
		{
			if(i==File[index].block-1)
			{
				if(index==num_file-1)
					end=lsrc;
				else
					end=File[index+1].place_raw;
				
			}
			else
				end=lsrc;
			fwrite(&data[start],sizeof(char),end-start,p);
		}
	}
	fclose(read_file);
	fclose(p);
	return true;
}


bool uncompress_work_file(char filename[],char dst[])//根据文件名称进行解压缩
{
	int index=find_File_place(filename);
	if(index==-1)
	{
		cout <<filename<<" is not exist in this file"<<endl;
		return false;
	}
	if(uncompress_work_file(index,dst))
		return true;
	else
		return false;
}

bool judge_raw_dir_exist(char path[])//判断路径是否存在，如果不在，可否生成
{
	if(strcmp(path,"\\")==0)
	{
		strcpy(path,workpath);
		return true;
	}
	else 
	{
		struct _finddata_t data;
		long hnd = _findfirst(path, &data );    // 查找文件名与正则表达式chRE的匹配第一个文件
		if ( hnd < 0 )
		{
			if(_mkdir(path)!=0)
			{
				cout <<path<<" is not exist"<<endl;
				return false;
			}
		}
	}
	return true;
}

char temp_path[1000];
char flag_dir[30000]={0};

bool judge_path_same(int s,int d)//判断后者是否是前者的子路径
{
	if(s==0)
	{
		int i=0;
		int l=strlen(Dir[d]);
		for(i=0;i<l;i++)
			if(Dir[d][i]=='\\')
				return false;
		return true;
	}
	else
	{
		int l=strlen(Dir[s]),i=0;
		for(i=0;i<l;i++)
			if(Dir[s][i]!=Dir[d][i])
				break;
		if(i!=l)
			return false;
		else if(Dir[d][i]=='\\')
			return true;
		else
			return false;
	}
}

bool rebuild_dir(int n)//重建第n个目录
{
	char path[1000];
	strcpy(path,temp_path);
	if(n!=0)
	{
		strcat(path,"\\");
		strcat(path,Dir[n]);
		if(_mkdir(path)!=0)
		{
			cout <<path<<" can't be created"<<endl;
			return false;
		}
	}
	int i=0;
	for(i=1;i<num_dir;i++)
	{
		if(flag_dir[i]!=1&&judge_path_same(n,i))
		{
			flag_dir[i]=1;
			if(!rebuild_dir(i))
				return false;
		}
	}

	char tpath[1000];
	for(i=0;i<num_file;i++)
	{
		if(File[i].ndir==n)
		{
			strcpy(tpath,path);
			strcat(tpath,"\\");
			strcat(tpath,File[i].name);
			cout <<"uncompress "<<tpath<<endl;
			if(!uncompress_work_file(i,tpath))
			{
				cout <<"uncompress "<<tpath<<" failed"<<endl;
				//return false;
			}
		}
	}
	cout <<endl;
	return true;
}

bool uncompress_work_dir(char name[],char dstpath[])//重建路径
{
	if(!judge_raw_dir_exist(dstpath))
	{
		return false;
	}
	int i=0;
	if(strcmp(name,"\\")==0)
		i=0;
	else
	{
		for(i=0;i<num_dir;i++)
			if(strcmp(name,Dir[i])==0)
				break;
		if(i==num_dir)
		{
			cout <<name<<" is not exist in this compress file"<<endl;
			return false;
		}
	}
	strcpy(temp_path,dstpath);
	memset(flag_dir,0,30000);
	if(!rebuild_dir(i))
		return false;
	return true;
}