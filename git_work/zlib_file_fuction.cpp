#include "stdafx.h"

#define MAX_CP 10000
//MAX_CP为最大一次压缩输入的数据长度，byte
//zlib中的基础函数
//compress
//uncompress

bool compress_str(char src[],int lsrc,char dst[],int &ldst)//压缩字符串
{
	errno_t err; //错误变量的定义
	ldst=MAX_CP;
	err = compress((Bytef*)dst, (uLongf*)&ldst, (const Bytef*)src, (uLongf)lsrc);
	if (err != Z_OK)
	{
		ldst=-1;
		return false;
	}
	return true;
}

bool uncompress_str(char src[],int lsrc,char dst[],int &ldst)//解压缩字符串
{
	errno_t err; //错误变量的定义
	ldst=MAX_CP;
	err = uncompress((Bytef*)dst, (uLongf*)&ldst, (Bytef*)src, (uLongf)lsrc);
	if (err != Z_OK)
	{
		ldst=-1;
		return false;
	}
	return true;
}

void compress_file(char src_file[],char dst_file[])//压缩文件
{
	FILE *rfile=NULL;
	rfile=fopen(src_file,"rb");
	if(rfile==NULL)
	{
		cout <<src_file<<" is not exist!"<<endl;
		return ;
	}
	FILE *wfile=NULL;
	wfile=fopen(dst_file,"wb");
	if(wfile==NULL)
	{
		cout <<dst_file<<" can't be created"<<endl;
		return ;
	}

	char src[MAX_CP]={0};
	char dst[MAX_CP]={0};
	int lsrc=0,ldst=0;
	while(!feof(rfile))
	{
		lsrc=fread(src,sizeof(char),MAX_CP,rfile);
		if(compress_str(src,lsrc,dst,ldst))
		{
			fwrite(&ldst,sizeof(int),1,wfile);
			fwrite(dst,sizeof(char),ldst,wfile);
		}
		else
		{
			int t=lsrc*-1;
			fwrite(&t,sizeof(int),1,wfile);
			fwrite(src,sizeof(char),lsrc,wfile);
		}
	}
	fclose(rfile);
	fclose(wfile);
}

void uncompress_file(char src_file[],char dst_file[])//解压缩文件
{
	FILE *rfile=NULL;
	rfile=fopen(src_file,"rb");
	if(rfile==NULL)
	{
		cout <<src_file<<" is not exist!"<<endl;
		return ;
	}
	FILE *wfile=NULL;
	wfile=fopen(dst_file,"wb");
	if(wfile==NULL)
	{
		cout <<dst_file<<" can't be created"<<endl;
		return ;
	}
	char src[MAX_CP]={0};
	char dst[MAX_CP*2]={0};
	int lsrc=0,ldst=0;
	while(!feof(rfile))
	{
		int t=0;
		fread(&t,sizeof(int),1,rfile);
		if(t<0)
			lsrc=t*-1;
		else
			lsrc=t;
		lsrc=fread(src,sizeof(char),lsrc,rfile);
		if(t>0)
		{
			if(uncompress_str(src,lsrc,dst,ldst))
			{
				fwrite(dst,sizeof(char),ldst,wfile);
			}
			else
			{
				cout <<"re zip is wrong!"<<endl;
				break;
				//return ;
			}
		}
		else
		{
			fwrite(src,sizeof(char),lsrc,wfile);
		}
	}
	fclose(rfile);
	fclose(wfile);
}
   