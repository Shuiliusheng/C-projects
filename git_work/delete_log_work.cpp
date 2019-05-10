#include"stdafx.h"
extern LOG Log[100];
extern int log_num;
extern char log_file[1000];

extern File temp_file[30000];
extern char temp_Dir[30000][400];
extern int tnum_dir;
extern int tnum_file;
extern File file[30000];
extern int num_file;

/*
删除记录文件的方法：
根据想要删除的记录的下一个记录的operator文件，
找到在下一次会被删除的文件，然后将要删的记录中的
压缩得了的这些文件删除，然后修改要删除的记录中的
operator文件中的会用到即makefile功能中涉及到这些
文件的操作，从而删除这些指令，同时还要修改detail文件
中这些要删除文件的信息。
最后将log中的lon-num设为无效，但是并不会删除log文件夹中
的该记录的文件夹和这些文件，因为recovery时还会用到这些
只是删除了多余的和设置为无效。
如果删除的时最后一个记录，则直接删除即可
*/

bool find_delete_file(int log)//寻找要被删除的临时文件
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
	char dir[1000];
	int ndir;
	tnum_file=0;
	while(!feof(p))
	{
		fscanf(p,"%d",&op);
		if(op==CLEAR)
			continue;
		else if(op==DEL_D)
		{
			fscanf(p,"%s",dir);
		}
		else if(op==MK_D)
		{
			fscanf(p,"%s",dir);
		}
		else if(op==DEL_F)
		{
			fscanf(p,"%d",&ndir);
			fscanf(p,"%s",dir);
			temp_file[tnum_file].dir=ndir;
			strcpy(temp_file[tnum_file].filename,dir);
			tnum_file++;
		}
		else if(op==MK_F)
		{
			fscanf(p,"%d",&ndir);
			fscanf(p,"%s",dir);
		}
	}
	fclose(p);
	return true;
}

bool judge_del_file(char filename[],int dir)//判断文件是否为要删除的文件
{
	for(int i=0;i<tnum_file;i++)
	{
		if(dir==temp_file[i].dir&&strcmp(filename,temp_file[i].filename)==0)
			return true;
	}
	return false;
}

bool change_operator_log(int log)//根据方法修改operator_log文件
{
	char filename[1000];
	char filename1[1000];
	sprintf(filename,"%s\\log_%d\\operator_log",log_file,log);
	sprintf(filename1,"%s\\log_%d\\operator_log1",log_file,log);
	FILE *p=NULL;
	p=fopen(filename,"r");
	if(p==NULL)
	{
		cout <<filename<<" is not exist!"<<endl;
		return false;
	}
	FILE *save=NULL;
	save=fopen(filename1,"w");
	if(save==NULL)
	{
		cout <<filename1<<" can't be created"<<endl;
		return false;
	}
	int op=0;
	char dir[1000];
	int ndir;
	while(!feof(p))
	{
		fscanf(p,"%d",&op);
		if(op==CLEAR)
			continue;
		else if(op==DEL_D)
		{
			fscanf(p,"%s",dir);
			fprintf(save,"%d\n",op);
			fprintf(save,"%s\n",dir);
		}
		else if(op==MK_D)
		{
			fscanf(p,"%s",dir);
			fprintf(save,"%d\n",op);
			fprintf(save,"%s\n",dir);
		}
		else if(op==DEL_F)
		{
			fscanf(p,"%d",&ndir);
			fscanf(p,"%s",dir);
			fprintf(save,"%d %d\n",op,ndir);
			fprintf(save,"%s\n",dir);
		}
		else if(op==MK_F)
		{
			fscanf(p,"%d",&ndir);
			fscanf(p,"%s",dir);
			if(!judge_del_file(dir,ndir))
			{
				fprintf(save,"%d %d\n",op,ndir);
				fprintf(save,"%s\n",dir);
			}
		}
	}
	fclose(p);
	fclose(save);

	cout <<"remove: "<<remove(filename)<<endl;
	cout <<"rename: "<<rename(filename1,filename)<<endl;
	return true;
}

void delete_useless_file(int log)//删除已经无用的临时压缩文件
{
	char log_dir[1000];
	sprintf(log_dir,"%s\\log_%d",log_file,log);
	char name[1000];
	for(int i=0;i<tnum_file;i++)
	{
		sprintf(name,"%s\\%d_%s",log_dir,temp_file[i].dir,temp_file[i].filename);
		cout <<"remove "<<name<<" "<<remove(name)<<endl;
	}
	return ;
}

bool change_detail_log(int log)//根据删除方法改变detail_log文件
{
	char filename[1000],filename1[1000];
	sprintf(filename,"%s\\log_%d\\detail_log",log_file,log);
	sprintf(filename1,"%s\\log_%d\\detail_log1",log_file,log);
	FILE *p=NULL;
	p=fopen(filename,"r");
	if(p==NULL)
	{
		cout <<filename<<" is not exist!"<<endl;
		return false;
	}
	FILE *save=NULL;
	save=fopen(filename1,"w");
	if(save==NULL)
	{
		cout <<filename1<<" can't be created"<<endl;
		return false;
	}

	tnum_dir=0;
	num_file=0;
	int i=0;
	
	fscanf(p,"%d",&tnum_dir);
	fprintf(save,"%d\n",tnum_dir);
	tnum_dir++;
	for(i=1;i<tnum_dir;i++)
	{
		fscanf(p,"%s",temp_Dir[i]);
		fprintf(save,"%s\n",temp_Dir[i]);
	}
	int num=0;
	fscanf(p,"%d",&num);
	for(i=0;i<num;i++)
	{
		fscanf(p,"%d %d",&file[num_file].dir,&file[num_file].write_time);
		fscanf(p,"%s",file[num_file].filename);
		if(!judge_del_file(file[num_file].filename,file[num_file].dir))
			num_file++;
	}

	fprintf(save,"%d\n",num_file);
	for(i=0;i<num_file;i++)
	{
		fprintf(save,"%d %d\n",file[i].dir,file[i].write_time);
		fprintf(save,"%s\n",file[i].filename);
	}

	fclose(p);
	fclose(save);

	cout <<"remove: "<<remove(filename)<<endl;
	cout <<"rename: "<<rename(filename1,filename)<<endl;

	return true;
}

bool delete_log(int log)//删除编号为log的记录
{
	if(log>=log_num||Log[log].log_num==-1||log<0)
	{
		cout <<"Log "<<log<<" is not exist"<<endl;
		return false;
	}
	if(log==log_num-1)
	{
		Log[log].log_num=-1;
		log_num--;
		char filename[1000];
		sprintf(filename,"%s\\log_%d",log_file,log);
		del_log(filename);
		return true;
	}
	if(!read_dir_file_2(log))
	{
		return false;
	}
	if(!find_delete_file(log+1))
	{
		return false;
	}
	if(!change_operator_log(log))
	{
		return false;
	}
	if(!change_detail_log(log))
	{
		return false;
	}
	delete_useless_file(log);
	Log[log].log_num=-1;
	return true;
}