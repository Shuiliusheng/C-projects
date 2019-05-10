// compress_file.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

int compress_flag=0;
int init_flag=0;
int read_flag=0;

void help_info()
{
	cout <<endl<<"      help information"<<endl<<endl;
	cout <<" cls : ����"<<endl;
	cout <<" exit : �˳�"<<endl;
	cout <<" init : ѹ���ͽ�ѹ�ļ�ʱ�ĳ�ʼ������"<<endl;
	cout <<" add-dir : ���Ҫ��ѹ����·�����ڵ�ǰ����·���µ�·����"<<endl;
	cout <<" add-file: ���Ҫ��ѹ�����ļ����ڵ�ǰ����·���µ��ļ���"<<endl;
	cout <<" del-file: ɾ����ӵ��ļ�"<<endl;
	cout <<" del-dir : ɾ����ӵ�·��"<<endl;
	cout <<" add-useless-file: ��Ӳ���Ҫѹ�����ļ�����"<<endl;
	cout <<" add-useless-dir : ��Ӳ���Ҫѹ����·������"<<endl;
	cout <<" compress: ѹ�������û�����·�����ļ�����ѹ���˹���·���������ļ�"<<endl;
	cout <<" read-compress: ��ȡѹ�����ļ�����ѹ��֮ǰ�Ĳ���"<<endl;
	cout <<" uncompress-file: ��ѹ��ѹ���ļ��е��ļ�"<<endl;
	cout <<" uncompress-dir : ��ѹ��ѹ���ļ��е�·��"<<endl;
	cout <<" show-add: ��ʾ��ǰ�е�·�����ļ�"<<endl;
	cout <<" cf : ѹ�������ļ�"<<endl;
	cout <<" ucf: ��ѹ�������ļ�"<<endl;
}

int main()
{
	char cmd[1000];
	while(1)
	{
		cout <<"User>>";
		cin >>cmd;
		if(strcmp(cmd,"exit")==0)
			break;
		if(strcmp(cmd,"init")==0)
		{
			init_flag=1;
			init();
			cout <<"init successfully!"<<endl<<endl;
		}
		else if(strcmp(cmd,"add-dir")==0)
		{
			if(init_flag==0)
			{
				cout <<"init first please!"<<endl<<endl;
				continue;
			}
			add_compress_dir();
		}
		else if(strcmp(cmd,"add-file")==0)
		{
			if(init_flag==0)
			{
				cout <<"init first please!"<<endl<<endl;
				continue;
			}
			add_compress_file();
		}
		else if(strcmp(cmd,"show-add")==0)
		{
			show_dir();
		}
		else if(strcmp(cmd,"compress")==0)
		{
			if(init_flag==0)
			{
				cout <<"init first please!"<<endl<<endl;
				continue;
			}
			char name[1000];
			cout <<"dst name:";
			getchar();
			gets(name);
			precompress_dir_file();
			if(!compress_file(name))
			{
				remove(name);
				cout <<"compress failed\n"<<endl;
				continue;
			}
			cout <<endl;
			show_dir();
			init_flag=0;
		}
		else if(strcmp(cmd,"del-dir")==0)
		{
			char temp[1000];
			gets(temp);
			del_add_dir(temp);
		}
		else if(strcmp(cmd,"del-file")==0)
		{
			char temp[1000];
			gets(temp);
			del_add_file(temp);
		}
		else if(strcmp(cmd,"add-useless-file")==0)
		{
			if(init_flag==0)
			{
				cout <<"init first please!"<<endl<<endl;
				continue;
			}
			add_useless_file();
		}
		else if(strcmp(cmd,"add-useless-dir")==0)
		{
			if(init_flag==0)
			{
				cout <<"init first please!"<<endl<<endl;
				continue;
			}
			add_useless_dir();
		}
		else if(strcmp(cmd,"read-compress")==0)
		{
			if(init_flag==0||read_flag==1)
			{
				cout <<"init first please!"<<endl<<endl;
				continue;
			}
			char name[1000];
			cout <<"src name:";
			getchar();
			gets(name);
			if(pre_uncompress_work(name))
			{
				show_dir();
				read_flag=1;
			}
		}
		else if(strcmp(cmd,"uncompress-file")==0)
		{
			if(init_flag==0)
			{
				cout <<"init first please!"<<endl<<endl;
				continue;
			}
			if(read_flag==0)
			{
				cout <<"read uncompress file first!"<<endl;
				continue;
			}
			char src[1000],dst[1000];
			getchar();
			cout <<"src name:";
			gets(src);
			cout <<"dst name:";
			gets(dst);
			if(uncompress_work_file(src,dst))
			{
				cout <<src<<"  has already be uncompressed!"<<endl;
			}
		}
		else if(strcmp(cmd,"uncompress-dir")==0)
		{
			if(init_flag==0)
			{
				cout <<"init first please!"<<endl<<endl;
				continue;
			}
			if(read_flag==0)
			{
				cout <<"read uncompress file first!"<<endl;
				continue;
			}
			char src[1000],dst[1000];
			getchar();
			cout <<"src path name:";
			gets(src);
			cout <<"dst path name:";
			gets(dst);
			if(uncompress_work_dir(src,dst))
			{
				cout <<dst<<"  has already be uncompressed!"<<endl;
			}
		}
		else if(strcmp(cmd,"cls")==0)
		{
			system("cls");
		}
		else if(strcmp(cmd,"cf")==0)
		{
			cout <<"ѹ�������ļ�"<<endl;
			char src[1000],dst[1000];
			getchar();
			cout <<"src name:";
			gets(src);
			cout <<"dst name:";
			gets(dst);
			compress_file(src,dst);
			cout <<"compress successfully!"<<endl<<endl;
		}
		else if(strcmp(cmd,"ucf")==0)
		{
			cout <<"��ѹ�������ļ�"<<endl;
			char src[1000],dst[1000];
			getchar();
			cout <<"src name:";
			gets(src);
			cout <<"dst name:";
			gets(dst);
			uncompress_file(src,dst);
			cout <<"uncompress successfully!"<<endl<<endl;
		}
		else if(strcmp(cmd,"help")==0)
		{
			help_info();
			cout <<endl;
		}
	}
	return 0;
}

