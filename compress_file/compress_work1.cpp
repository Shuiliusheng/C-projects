#include"stdafx.h"
extern char Dir[30000][1000];
extern int num_dir;

extern FIle File[30000];
extern int num_file;

extern char useless_Dir[1000][500];
extern char useless_File[1000][1000];
extern int num_ud;
extern int num_uf;

extern char workpath[1000];
extern char buffer[1000];

char data[MAX_CP+10000];
char tempdata[MAX_CP+10000];
FILE *save=NULL;

unsigned int get_filesize(FILE *p)//获取文件大小
{
	unsigned int curpos,length;
	curpos=ftell(p);
	fseek(p,0L,SEEK_END);
	length=ftell(p);
	fseek(p,curpos,SEEK_SET);
	return length;
}

void group_filename(char name[],int n)//重组文件名，使其带有相对路径
{
	strcpy(name,workpath);
	strcat(name,"\\");
	if(File[n].ndir!=0)
	{
		strcat(name,Dir[File[n].ndir]);
		strcat(name,"\\");
	}
	strcat(name,File[n].name);
}

char src[MAX_CP]={0};
char dst[MAX_CP]={0};
int compress_file(char src_file[],FILE *wfile)//压缩文件
{
	FILE *rfile=NULL;
	rfile=fopen(src_file,"rb");
	if(rfile==NULL)
	{
		cout <<src_file<<" is not exist!"<<endl;
		return -1;
	}
	if(wfile==NULL)
	{
		return -1;
	}
	int lsrc=0,ldst=0;
	unsigned int size=0;
	unsigned int rawsize=get_filesize(rfile);
	int block=0;
	while(!feof(rfile))
	{
		lsrc=fread(src,sizeof(char),MAX_CP,rfile);
		size+=lsrc;
		show_percent(size,rawsize);
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
		block++;
	}
	fclose(rfile);
	return block;
}


bool compress_file(char dst[])//压缩所有文件
{
	int i=0;
	remove(dst);
	save=fopen(dst,"ab");
	if(save==NULL)
	{
		cout <<dst<<" can't be created"<<endl;
		return false;
	}
	char filename[1000];
	for(i=0;i<num_file;i++)
	{
		group_filename(filename,i);
		File[i].place_c=ftell(save);
		cout <<" compress : "<<filename<<endl;
		int t=compress_file(filename,save);
		if(t==-1)
		{
			fclose(save);
			remove(dst);
			return false;
		}
		File[i].block=t;
	}
	unsigned int record_place=ftell(save);
	fwrite(&num_dir,sizeof(int),1,save);//记录目录和文件信息
	for(i=1;i<num_dir;i++)
	{
		fputs(Dir[i],save);
		fprintf(save,"\n");
	}
	fprintf(save,"%d\n",num_file);
	for(i=0;i<num_file;i++)
	{
		fprintf(save,"%d %d %d\n",File[i].ndir,File[i].place_c,File[i].block);
		fputs(File[i].name,save);
		fprintf(save,"\n");
	}
	char temp1[30];
	sprintf(temp1,"%u\n",record_place);
	fwrite(temp1,sizeof(char),20,save);
	cout <<endl;
	fclose(save);
	return true;
}

