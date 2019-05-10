
#include "stdafx.h"
extern char buffer[1000];
extern char workpath[1000];
extern int num_dir;
extern int num_file;
extern char log_file[1000];

void help()
{
	cout <<"ע�⣡"<<endl;
	cout <<"1. ʹ��֮ǰ����ʹ��init���г�ʼ�����߼���"<<endl;
	cout <<"2. ʹ��֮����ʹ��exit���йرգ��ѱ������"<<endl;

	cout <<endl<<"����"<<endl;
	cout <<"cls : ����"<<endl;
	cout <<"exit: �˳�"<<endl;
	cout <<"init: ��ʼ��git/����ԭ�е�git"<<endl;
	cout <<"del-log : ɾ��ĳһ�εļ�¼"<<endl;
	cout <<"show-dir  : ��ʾ��ǰ��Ŀ¼��Ϣ"<<endl;
	cout <<"show-log  : ʵ�ֵ�ǰ���еļ�¼�ļ���Ϣ"<<endl;
	cout <<"clear-log : ���git��Ŀ"<<endl;
	cout <<"create-log: ����һ����¼�ļ�����¼��ʱ����"<<endl;
	cout <<"recovery n: �ָ����Ϊn�ļ�¼�ļ�"<<endl;
	cout <<"show-constraint: ��ʾĿǰ���ļ���Ŀ¼��Լ��"<<endl;
	cout <<"add-dir-constraint : ���Ŀ¼��Լ������ʾ��֮���ٱ���ע"<<endl;
	cout <<"add_file-constraint: ����ļ���Լ������ʾ��֮���ٱ���ע"<<endl;
	cout <<"del-dir-constraint : ɾ��Ŀ¼��Լ������ʾ֮�����±���¼"<<endl;
	cout <<"del-file-constraint: ɾ���ļ���Լ������ʾ֮�����±���¼"<<endl;
	
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
			cout <<"����1�����������˻�: ";
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

