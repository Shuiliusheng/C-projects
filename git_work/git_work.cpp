
#include "stdafx.h"
extern char buffer[1000];
extern char workpath[1000];
extern int num_dir;
extern int num_file;
extern char log_file[1000];

void help()
{
	cout <<"注意！"<<endl;
	cout <<"1. 使用之前，请使用init进行初始化或者加载"<<endl;
	cout <<"2. 使用之后，请使用exit进行关闭，已保存操作"<<endl;

	cout <<endl<<"功能"<<endl;
	cout <<"cls : 清屏"<<endl;
	cout <<"exit: 退出"<<endl;
	cout <<"init: 初始化git/加载原有的git"<<endl;
	cout <<"del-log : 删除某一次的记录"<<endl;
	cout <<"show-dir  : 显示当前的目录信息"<<endl;
	cout <<"show-log  : 实现当前已有的记录文件信息"<<endl;
	cout <<"clear-log : 清除git项目"<<endl;
	cout <<"create-log: 创建一个记录文件，记录此时工程"<<endl;
	cout <<"recovery n: 恢复编号为n的记录文件"<<endl;
	cout <<"show-constraint: 显示目前的文件和目录的约束"<<endl;
	cout <<"add-dir-constraint : 添加目录的约束，表示其之后不再被关注"<<endl;
	cout <<"add_file-constraint: 添加文件的约束，表示其之后不再被关注"<<endl;
	cout <<"del-dir-constraint : 删除目录的约束，表示之后重新被记录"<<endl;
	cout <<"del-file-constraint: 删除文件的约束，表示之后重新被记录"<<endl;
	
}

void reback_constraint()
{
	char filename[1000];
	strcpy(filename,log_file);
	strcat(filename,"\\log_constraint");
	save_constraint_to_file(filename);
}

int main()
{
	char cmd[100];
	int flag=0;
	while(1)
	{
		cout <<"User>>";
		cin >>cmd;
		if(strcmp(cmd,"exit")==0)
		{
			break;
		}
		else if(strcmp(cmd,"init")==0)
		{
			if(flag==0)
				init_git();
			else
				cout <<"init already"<<endl;
			flag=1;
			cout <<"init git successfully!"<<endl;
		}
		else if(strcmp(cmd,"show-dir")==0)
		{
			if(flag==0)
			{
				cout <<"init git first!"<<endl;
				continue;
			}  
			strcpy(buffer,workpath);
			num_dir=1;
			num_file=0;
			printDir(buffer,0);
			show_read();
			cout <<endl;
		}
		else if(strcmp(cmd,"create-log")==0)
		{
			if(flag==0)
			{
				cout <<"init git first!"<<endl;
				continue;
			}  
			show_constraint();
			int t=0;
			cout <<"输入1继续，其它退回: ";
			cin >>t;
			if(t!=1)
				continue;

			char dir_log[1000];
			strcpy(buffer,workpath);
			num_dir=1;
			num_file=0;
			printDir(buffer,0);
			create_log(dir_log);

			cout <<"dir: "<<dir_log<<endl;
		}
		else if(strcmp(cmd,"show-log")==0)
		{
			show_log();
		}
		else if(strcmp(cmd,"recovery")==0)
		{
			if(flag==0)
			{
				cout <<"init git first!"<<endl;
				continue;
			}  
			int log=0;
			cin >>log;
			strcpy(buffer,workpath);
			num_dir=1;
			num_file=0;
			printDir(buffer,0);
			if(recovery_log(log))
			{
				cout <<"recovery successful"<<endl;
				reback_constraint();
			}
		}
		else if(strcmp(cmd,"clear-log")==0)
		{
			del_git();
			flag=0;
		}
		else if(strcmp(cmd,"cls")==0)
		{
			system("cls");
		}
		else if(strcmp(cmd,"help")==0)
		{
			help();
		}
		else if(strcmp(cmd,"add-dir-constraint")==0)
		{
			if(flag==0)
			{
				cout <<"init git first!"<<endl;
				continue;
			}  
			add_dir_constraint();
			reback_constraint();
		}
		else if(strcmp(cmd,"add-file-constraint")==0)
		{
			if(flag==0)
			{
				cout <<"init git first!"<<endl;
				continue;
			}  
			add_file_constraint();
			reback_constraint();
		}
		else if(strcmp(cmd,"show-constraint")==0)
		{
			if(flag==0)
			{
				cout <<"init git first!"<<endl;
				continue;
			}  
			show_constraint();
			reback_constraint();
		}
		else if(strcmp(cmd,"del-dir-constraint")==0)
		{
			if(flag==0)
			{
				cout <<"init git first!"<<endl;
				continue;
			}  
			char dir[1000];
			cin >>dir;
			del_dir_constraint(dir);
			reback_constraint();
		}
		else if(strcmp(cmd,"del-file-constraint")==0)
		{
			if(flag==0)
			{
				cout <<"init git first!"<<endl;
				continue;
			}  
			char temp[1000];
			cin >>temp;
			del_file_constraint(temp);
			reback_constraint();
		}
		else if(strcmp(cmd,"del-log")==0)
		{
			if(flag==0)
			{
				cout <<"init git first!"<<endl;
				continue;
			}  
			int log=0;
			cin >>log;
			if(delete_log(log))
			{
				cout <<"log "<<log<<" is delete succesfully!"<<endl;
				write_log();
			}
		}
	}
	if(flag==1)
	{
		write_log();
		reback_constraint();
	}
	return 0;
}

