#include"stdafx.h"

char dir_constraint[1000][1000];
char file_constraint[1000][1000];
int num_dc=0;
int num_fc=0;

int error_show_flag=0;

/*
约束指的是在保存记录的过程中将某些文件夹
或者是文件忽略，不算入其中，从而减少记录的大小
也不需要保存那些没有必要的文件
在增加的约束之后，需要考虑的就是添加之后有些文件或者文件夹
不会在被考略，但是之前却有，这个时候就要比对那些之前有现在没有的
文件或者文件夹下的文件是不是属于文件约束或者是所处的路径是约束路径
从而不会再被考略，这个出现在创建记录时和前一个记录进行比较时的情况

*/


void del_dir_constraint(char dir[])//根据输入的路径，删除已经存储的约束
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

void del_file_constraint(char filename[])//根据输入的文件名，删除已经存储的约束
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

void show_constraint()//显示已有的约束
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

void add_dir_constraint()//添加路径的约束
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

void add_file_constraint()//添加文件的约束
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

bool add_constraint_from_file(char filename[])//从保存的文件中加载约束，包括路径和文件
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

bool save_constraint_to_file(char filename[])//将现有的文件和路径约束保存起来
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

bool judge_dir_constraint(char dir[])//判断路径是否为约束
{
	int i=0;
	for(i=0;i<num_dc;i++)
	{
		if(strcmp(dir,dir_constraint[i])==0)
			return true;
	}
	return false;
}

bool judge_file_constraint(char filename[])//判断文件名是不是约束
{
	int i=0;
	for(i=0;i<num_fc;i++)
	{
		if(strcmp(filename,file_constraint[i])==0)
			return true;
	}
	return false;
}