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
ɾ����¼�ļ��ķ�����
������Ҫɾ���ļ�¼����һ����¼��operator�ļ���
�ҵ�����һ�λᱻɾ�����ļ���Ȼ��Ҫɾ�ļ�¼�е�
ѹ�����˵���Щ�ļ�ɾ����Ȼ���޸�Ҫɾ���ļ�¼�е�
operator�ļ��еĻ��õ���makefile�������漰����Щ
�ļ��Ĳ������Ӷ�ɾ����Щָ�ͬʱ��Ҫ�޸�detail�ļ�
����ЩҪɾ���ļ�����Ϣ��
���log�е�lon-num��Ϊ��Ч�����ǲ�����ɾ��log�ļ�����
�ĸü�¼���ļ��к���Щ�ļ�����Ϊrecoveryʱ�����õ���Щ
ֻ��ɾ���˶���ĺ�����Ϊ��Ч��
���ɾ����ʱ���һ����¼����ֱ��ɾ������
*/

bool find_delete_file(int log)//Ѱ��Ҫ��ɾ������ʱ�ļ�
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

bool judge_del_file(char filename[],int dir)//�ж��ļ��Ƿ�ΪҪɾ�����ļ�
{
	for(int i=0;i<tnum_file;i++)
	{
		if(dir==temp_file[i].dir&&strcmp(filename,temp_file[i].filename)==0)
			return true;
	}
	return false;
}

bool change_operator_log(int log)//���ݷ����޸�operator_log�ļ�
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

void delete_useless_file(int log)//ɾ���Ѿ����õ���ʱѹ���ļ�
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

bool change_detail_log(int log)//����ɾ�������ı�detail_log�ļ�
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

bool delete_log(int log)//ɾ�����Ϊlog�ļ�¼
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