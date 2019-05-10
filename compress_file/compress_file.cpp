// compress_file.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int compress_flag=0;
int init_flag=0;
int read_flag=0;

void help_info()
{
	cout <<endl<<"      help information"<<endl<<endl;
	cout <<" cls : 清屏"<<endl;
	cout <<" exit : 退出"<<endl;
	cout <<" init : 压缩和解压文件时的初始化工作"<<endl;
	cout <<" add-dir : 添加要被压缩的路径（在当前工作路径下的路径）"<<endl;
	cout <<" add-file: 添加要被压缩的文件（在当前工作路径下的文件）"<<endl;
	cout <<" del-file: 删除添加的文件"<<endl;
	cout <<" del-dir : 删除添加的路径"<<endl;
	cout <<" add-useless-file: 添加不需要压缩的文件名称"<<endl;
	cout <<" add-useless-dir : 添加不需要压缩的路径名称"<<endl;
	cout <<" compress: 压缩，如果没有添加路径和文件，则压缩此工作路径下所有文件"<<endl;
	cout <<" read-compress: 读取压缩的文件，解压缩之前的步骤"<<endl;
	cout <<" uncompress-file: 解压缩压缩文件中的文件"<<endl;
	cout <<" uncompress-dir : 解压缩压缩文件中的路径"<<endl;
	cout <<" show-add: 显示当前有的路径和文件"<<endl;
	cout <<" cf : 压缩单个文件"<<endl;
	cout <<" ucf: 解压缩单个文件"<<endl;
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
			cout <<"压缩单个文件"<<endl;
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
			cout <<"解压缩单个文件"<<endl;
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

