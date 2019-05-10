#include"stdafx.h"

extern int num_dc;
extern int num_fc;
extern int error_show_flag;

LOG Log[100];
File file[30000];
char Dir[30000][400]={0};
char log_file[1000];
char buffer[1000];
char workpath[1000];
int num_dir=1;
int num_file=0;
int log_num=0;
int raw_length=0;


File temp_file[30000];
char temp_Dir[30000][400]={0};
int tnum_dir=1;
int tnum_file=0;

int make_dir[30000]={0};
int num_md=0;
int del_dir[30000]={0};
int num_dd=0;

int make_file[30000]={0};
int num_mf=0;
int del_file[30000]={0};
int num_df=0;
char flag[30000]={0};

void init_git()//��ʼ��git��������ȡgit�Ĺ���·��
{
    _getcwd(buffer,1000);  
	strcpy(workpath,buffer);
	raw_length=strlen(buffer);
	strcpy(log_file,buffer);
	strcat(log_file,"\\");
	strcat(log_file,"git_log");
	_mkdir("git_log");
	read_log();
	char filename[1000];
	strcpy(filename,log_file);
	strcat(filename,"\\log_constraint");
	error_show_flag=1;
	add_constraint_from_file(filename);
	error_show_flag=0;
	printDir(buffer,0);

}
void save_detail_log(char filename[])//����detail log�ļ����������ļ���Ŀ¼�ľ�����Ϣ���ļ�
{
	FILE *p=NULL;
	p=fopen(filename,"w");
	if(p==NULL)
	{
		cout <<filename<<" can't be create"<<endl;
		return ;
	}
	fprintf(p,"%d\n",num_dir-1);
	int i=0;
	for(i=1;i<num_dir;i++)
		fprintf(p,"%s\n",Dir[i]);
	fprintf(p,"%d\n",num_file);
	for(i=0;i<num_file;i++)
	{
		fprintf(p,"%d %d\n",file[i].dir,file[i].write_time);
		fprintf(p,"%s\n",file[i].filename);
	}
	fclose(p);
}

bool read_dir_file(int log)//��ȡĳһ�ε�detail_log�ļ�
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
	tnum_file=0;
	int i=0;
	
	fscanf(p,"%d",&tnum_dir);
	tnum_dir++;
	for(i=1;i<tnum_dir;i++)
	{
		fscanf(p,"%s",temp_Dir[i]);
	}
	fscanf(p,"%d",&tnum_file);
	for(i=0;i<tnum_file;i++)
	{
		fscanf(p,"%d %d",&temp_file[i].dir,&temp_file[i].write_time);
		fscanf(p,"%s",temp_file[i].filename);
	}
	fclose(p);
	return true;
}


int find_dir(int n)//�������ұȶ�Ŀ¼
{
	int i=0;
	for(i=1;i<tnum_dir;i++)
	{
		if(flag[i]==0&&strcmp(Dir[n],temp_Dir[i])==0)
		{
			flag[i]=1;
			return i;
		}
	}
	return -1;
}

void find_different_dir()//Ѱ�Ҳ�ͬ��Ŀ¼
{
	int i=0;
	memset(flag,0,10000);
	num_md=0;
	num_dd=0;
	for(i=1;i<num_dir;i++)
	{
		if(find_dir(i)==-1)
		{
			make_dir[num_md++]=i;
		}
	}
	for(i=1;i<tnum_dir;i++)
		if(flag[i]==0)
		{
			if(!judge_dir_constraint(temp_Dir[i]))
				del_dir[num_dd++]=i;
		}

}

int find_file(int n)//Ѱ�Ҳ�ͬ���ļ��Ĺ��ܺ���
{
	int i=0;
	for(i=0;i<tnum_file;i++)
	{
		int t=file[n].dir;
		int t1=temp_file[i].dir;
		
		if(flag[i]==0&&strcmp(Dir[t],temp_Dir[t])==0&&file[n].write_time==temp_file[i].write_time&&strcmp(file[n].filename,temp_file[i].filename)==0)
		{
			flag[i]=1;
			return i;
		}
	}
	return -1;
}

void find_different_file()//Ѱ�Ҳ�ͬ���ļ�
{
	int i=0;
	memset(flag,0,10000);
	num_mf=0;
	num_df=0;
	char temp[1000];
	for(i=0;i<num_file;i++)
	{
		if(find_file(i)==-1)
		{
			make_file[num_mf++]=i;
		}
	}
	for(i=0;i<tnum_file;i++)
		if(flag[i]==0)
		{
			if(judge_dir_constraint(temp_Dir[temp_file[i].dir]))//��Ϊ�����dir��constraint�����޷��ҵ�·���µ��ļ�
				continue;
			if(num_fc!=0)
			{
				strcpy(temp,temp_Dir[temp_file[i].dir]);
				strcat(temp,"\\");
				strcat(temp,temp_file[i].filename);
			}
			if(judge_file_constraint(temp))//��Ϊ�����file��constraint���Ӷ�������������
				continue;
			del_file[num_df++]=i;
		}
}

void find_different()//Ѱ�Ҵ˴�log�ļ����ϴεĲ�ͬ
{
	find_different_dir();
	find_different_file();
}

void save_change_file(char dir_log[])//ѹ�����淢���仯���ļ�
{
	int i=0;
	char src[1000],dst[1000];
	for(i=0;i<num_mf;i++)
	{
		int t=make_file[i];
		if(file[t].dir!=0)
			sprintf(src,"%s\\%s\\%s",workpath,Dir[file[t].dir],file[t].filename);
		else
			sprintf(src,"%s\\%s",workpath,file[t].filename);
		sprintf(dst,"%s\\%d_%s",dir_log,file[t].dir,file[t].filename);
		compress_file(src,dst);
	}
}    

void make_operation_log(char dir_log[])//����operator_log�ļ�
{
	char src[1000];
	sprintf(src,"%s\\operator_log",dir_log);
	FILE *p=NULL;
	p=fopen(src,"w");
	if(p==NULL)
	{
		cout <<src<<" can't be created"<<endl;
		return ;
	}

	int i=0;
	for(i=0;i<num_df;i++)
	{
		fprintf(p,"%d %d\n",DEL_F,temp_file[del_file[i]].dir);
		fprintf(p,"%s\n",temp_file[del_file[i]].filename);
	}
	for(i=0;i<num_dd;i++)
	{
		fprintf(p,"%d\n",DEL_D);
		fprintf(p,"%s\n",temp_Dir[del_dir[i]]);
	}
	for(i=0;i<num_md;i++)
	{
		fprintf(p,"%d\n",MK_D);
		fprintf(p,"%s\n",Dir[make_dir[i]]);
	}
	for(i=0;i<num_mf;i++)
	{
		fprintf(p,"%d %d\n",MK_F,file[make_file[i]].dir);
		fprintf(p,"%s\n",file[make_file[i]].filename);
	}
	fclose(p);
}

void create_log(char dir_log[])//����һ��log�ļ�
{
	init_log(log_num,dir_log);
	input_info();
	if(log_num==0)
	{
		create_first(dir_log);
	}
	else
	{
		if(!read_dir_file(log_num-1))
		{
			return ;
		}
		find_different();
		save_change_file(dir_log);
		make_operation_log(dir_log);
	}
	char filename[1000];
	strcpy(filename,dir_log);
	strcat(filename,"\\log_constraint");
	save_constraint_to_file(filename);
	log_num++;
}

void input_info()//������־�ļ�������
{
	cout <<"input the daily record: (input \"exit\" to quit)"<<endl;
	char temp[100];
	gets(temp);
	while(strcmp(temp,"exit")!=0)
	{
		strcat(Log[log_num].info,temp);
		strcat(Log[log_num].info,"\n");
		gets(temp);
	}
	get_now_time(Log[log_num].time);
	Log[log_num].log_num=log_num;
}


void init_log(int n,char dir_log[])//��ʼ��log�ļ��������ļ��У�detail-log�ļ�
{
	char log_dir[1000];
	sprintf(log_dir,"%s\\log_%d",log_file,n);
	_mkdir(log_dir);//����·��
	strcpy(dir_log,log_dir);
	strcat(log_dir,"\\detail_log");
	save_detail_log(log_dir);//����detail_log�ļ�
}

void create_first(char dir_log[])//��һ�ν���log�ļ�
{
	char src[1000],dst[1000];
	for(int i=0;i<num_file;i++)
	{
		if(file[i].dir!=0)
			sprintf(src,"%s\\%s\\%s",workpath,Dir[file[i].dir],file[i].filename);
		else
			sprintf(src,"%s\\%s",workpath,file[i].filename);
		sprintf(dst,"%s\\%d_%s",dir_log,file[i].dir,file[i].filename);
		compress_file(src,dst);
	}
	sprintf(src,"%s\\operator_log",dir_log);
	FILE *p=NULL;
	p=fopen(src,"w");
	if(p==NULL)
	{
		cout <<src<<" can't be created"<<endl;
		return ;
	}

	int c=0;
	fprintf(p,"%d\n",CLEAR);
	for(c=1;c<num_dir;c++)
	{
		fprintf(p,"%d\n",MK_D);
		fprintf(p,"%s\n",Dir[c]);
	}
	for(c=0;c<num_file;c++)
	{
		fprintf(p,"%d %d\n",MK_F,file[c].dir);
		fprintf(p,"%s\n",file[c].filename);
	}
	fclose(p);
}

void copy_path(char *path,int dir)//�������·��
{
	int l=strlen(path);
	int c=0;
	for(int i=raw_length+1;i<=l;i++)
	{
		if(c<300)
			Dir[dir][c++]=path[i];
	}
}

void printDir(char* path,int dir)//ѭ�����������ļ�
{
	char rawpath[1000];
	strcpy(rawpath,path);
	strcat(path,"\\*.*");
    struct _finddata_t data;
	char temp[1000];
    long hnd = _findfirst(path, &data );    // �����ļ�����������ʽchRE��ƥ���һ���ļ�
    if ( hnd < 0 )
    {
        perror( path );
		return ;
    }
    int  nRet = 1;
    while ( nRet >= 0 )
    {
        if ( data.attrib == _A_SUBDIR )  // �����Ŀ¼
        {
			if(data.name[0]!='.'&&!(dir==0&&strcmp(data.name,"git_log")==0))
			{
				char tpath[1000];
				strcpy(tpath,rawpath);
				strcat(tpath,"\\");
				strcat(tpath,data.name);
				copy_path(tpath,num_dir);
				if(!judge_dir_constraint(Dir[num_dir]))
				{
					num_dir++;
					printDir(tpath,num_dir-1);
				}
			}
		}
        else
        {
			if(num_fc!=0)
			{
				strcpy(temp,Dir[dir]);
				if(dir!=0)
				{	
					strcat(temp,"\\");
				}
				strcat(temp,data.name);
			}
			if(!judge_file_constraint(temp))
			{
				file[num_file].dir=dir;
				file[num_file].write_time=data.time_write;
				strcpy(file[num_file].filename,data.name);
				num_file++;
			}
		}
        nRet = _findnext( hnd, &data );
    }
    _findclose( hnd );     // �رյ�ǰ���
}



void show_read()//��ʾ��ȡ����Ŀ¼���ļ�
{
	cout <<"DIR:"<<num_dir-1<<endl;
	for(int i=1;i<num_dir;i++)
		cout <<Dir[i]<<endl;
	cout <<endl<<"filename:"<<num_file<<endl;
	for(int c=0;c<num_file;c++)
	{
		cout <<Dir[file[c].dir]<<"\\";
		cout <<file[c].filename<<endl;
	}
	cout <<endl;
}

void read_log()//��ȡlog�ļ���log�ļ��а����ż�¼������ʱ�䣬�Լ��������
{
	char filename[300];
	strcpy(filename,log_file);
	strcat(filename,"\\log");
	FILE *p=NULL;
	p=fopen(filename,"r");
	if(p==NULL)
	{
		log_num=0;
		return ;
	}
	else
	{
		fread(&log_num,sizeof(int),1,p);
		for(int i=0;i<log_num;i++)
		{
			int t=0;
			fread(&Log[i].log_num,sizeof(int),1,p);
			fread(Log[i].time,sizeof(char),30,p);
			fread(&t,sizeof(int),1,p);
			fread(Log[i].info,sizeof(char),t,p);
		}
	}
	fclose(p);
}

void write_log()//����log�ļ�
{
	char filename[1000];
	strcpy(filename,log_file);
	strcat(filename,"\\log");
	FILE *p=NULL;
	p=fopen(filename,"wb");
	if(p==NULL)
	{
		cout <<filename<<" can't be created!"<<endl;
		return ;
	}
	fwrite(&log_num,sizeof(int),1,p);
	for(int i=0;i<log_num;i++)
	{
		int t=strlen(Log[i].info)+1;
		fwrite(&Log[i].log_num,sizeof(int),1,p);
		fwrite(Log[i].time,sizeof(char),30,p);
		fwrite(&t,sizeof(int),1,p);
		fwrite(Log[i].info,sizeof(char),t,p);
	}
	fclose(p);
}

void show_log()//��ʾ��ǰ��log�ļ�
{
	for(int i=0;i<log_num;i++)
	{
		if(Log[i].log_num!=-1)
		{
			cout <<"No "<<i<<endl;
			cout <<"Dir: "<<"git_log\\log_"<<Log[i].log_num<<endl;
			cout <<"Time:"<<Log[i].time<<endl;
			cout <<"Log: "<<endl;
			cout <<Log[i].info<<endl;
			cout <<endl;
		}
	}
}
