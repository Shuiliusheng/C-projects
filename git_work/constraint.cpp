#include"stdafx.h"

char dir_constraint[1000][1000];
char file_constraint[1000][1000];
int num_dc=0;
int num_fc=0;

int error_show_flag=0;

/*
Լ��ָ�����ڱ����¼�Ĺ����н�ĳЩ�ļ���
�������ļ����ԣ����������У��Ӷ����ټ�¼�Ĵ�С
Ҳ����Ҫ������Щû�б�Ҫ���ļ�
�����ӵ�Լ��֮����Ҫ���ǵľ������֮����Щ�ļ������ļ���
�����ڱ����ԣ�����֮ǰȴ�У����ʱ���Ҫ�ȶ���Щ֮ǰ������û�е�
�ļ������ļ����µ��ļ��ǲ��������ļ�Լ��������������·����Լ��·��
�Ӷ������ٱ����ԣ���������ڴ�����¼ʱ��ǰһ����¼���бȽ�ʱ�����

*/


void del_dir_constraint(char dir[])//���������·����ɾ���Ѿ��洢��Լ��
{
	int i=0;
	for(i=0;i<num_dc;i++)
		if(strcmp(dir,dir_constraint[i])==0)
			break;
	if(i==num_dc)
	{
		cout <<dir<<" is not a constraint dir"<<endl;
	}
	else
	{
		for(int c=i+1;c<num_dc;c++)
			strcpy(dir_constraint[c-1],dir_constraint[c]);
		num_dc--;
		cout <<"delete "<<dir<<" successfully!"<<endl;
	}
}

void del_file_constraint(char filename[])//����������ļ�����ɾ���Ѿ��洢��Լ��
{
	int i=0;
	for(i=0;i<num_fc;i++)
		if(strcmp(filename,file_constraint[i])==0)
			break;
	if(i==num_fc)
	{
		cout <<filename<<" is not a constraint file"<<endl;
	}
	else
	{
		for(int c=i+1;c<num_fc;c++)
			strcpy(file_constraint[c-1],file_constraint[c]);
		num_fc--;
		cout <<"delete "<<filename<<" successfully!"<<endl;
	}
}

void show_constraint()//��ʾ���е�Լ��
{
	int i=0;
	cout <<"constraint directory:"<<endl;
	for(i=0;i<num_dc;i++)
	{
		cout <<dir_constraint[i]<<endl;
	}
	cout <<endl<<"constraint file:"<<endl;
	for(i=0;i<num_fc;i++)
	{
		cout <<file_constraint[i]<<endl;
	}
	cout <<endl;
}

void add_dir_constraint()//���·����Լ��
{
	int num=0;
	cout <<"input the number of dir(relative dir) constraint:";
	cin >>num;
	for(int i=0;i<num;i++)
	{
		cout <<"No"<<i+1<<":";
		cin >>dir_constraint[num_dc++];
	}
}

void add_file_constraint()//����ļ���Լ��
{
	int num=0;
	cout <<"input the number of file (filename with relative dir) constraint:";
	cin >>num;
	for(int i=0;i<num;i++)
	{
		cout <<"No"<<i+1<<":";
		cin >>file_constraint[num_fc++];
	}
}

bool add_constraint_from_file(char filename[])//�ӱ�����ļ��м���Լ��������·�����ļ�
{
	FILE *p=NULL;
	p=fopen(filename,"r");
	if(p==NULL)
	{
		if(error_show_flag==0)
			cout <<filename<<" can't be read"<<endl;
		return false;
	}

	int i=0;
	fscanf(p,"%d\n",&num_dc);
	for(i=0;i<num_dc;i++)
	{
		fscanf(p,"%s\n",dir_constraint[i]);
	}
	fscanf(p,"%d\n",&num_fc);
	for(i=0;i<num_fc;i++)
	{
		fscanf(p,"%s\n",file_constraint[i]);
	}
	fclose(p);
	return true;
}

bool save_constraint_to_file(char filename[])//�����е��ļ���·��Լ����������
{
	FILE *p=NULL;
	p=fopen(filename,"w");
	if(p==NULL)
	{
		cout <<filename<<" can't be created"<<endl;
		return false;
	}

	int i=0;
	fprintf(p,"%d\n",num_dc);
	for(i=0;i<num_dc;i++)
	{
		fprintf(p,"%s\n",dir_constraint[i]);
	}
	fprintf(p,"%d\n",num_fc);
	for(i=0;i<num_fc;i++)
	{
		fprintf(p,"%s\n",file_constraint[i]);
	}
	fclose(p);
	return true;
}

bool judge_dir_constraint(char dir[])//�ж�·���Ƿ�ΪԼ��
{
	int i=0;
	for(i=0;i<num_dc;i++)
	{
		if(strcmp(dir,dir_constraint[i])==0)
			return true;
	}
	return false;
}

bool judge_file_constraint(char filename[])//�ж��ļ����ǲ���Լ��
{
	int i=0;
	for(i=0;i<num_fc;i++)
	{
		if(strcmp(filename,file_constraint[i])==0)
			return true;
	}
	return false;
}