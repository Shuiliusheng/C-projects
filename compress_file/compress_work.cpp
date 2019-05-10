#include"stdafx.h"
/*
ѹ������Ҫ������
��Ҫ˼�룺
���ļ������buffer�У����buffer�е����ݳ������趨��
MAXC_CP��ֵ����ᱻѹ����Ȼ����뵽ѹ��������ļ��С�

Ϊ���ܹ�ʵ�ָֻ���Ŀ¼���ؽ���
�ļ���Ҫ��¼�����ݣ�
1. �ļ���ʼ���ڵ�ѹ������׵�ַ
2. �ļ�ռ���˶��ٸ�ѹ����
3. �ļ���ѹ��֮ǰ��buffer�еĵ�ַ

ͬʱ��Ŀ¼��Ϣ���ļ���Ϣ������ѹ���ļ��Ľ�β�У�
���ڶ�ȡ����¼��Ϣ����ʼλ�ã����������ļ�ĩβ��20��
�ֽ���

*/

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

unsigned int record_place=0;

unsigned int total_size=0;

void group_filename(char name[],int n)//�����ļ�����ʹ��������·��
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

unsigned int get_filesize(FILE *p)//��ȡ�ļ���С
{
	unsigned int curpos,length;
	curpos=ftell(p);
	fseek(p,0L,SEEK_END);
	length=ftell(p);
	fseek(p,curpos,SEEK_SET);
	return length;
}

void copy_str_to_data(char str[],int &datasize)//���ַ���������buffer��
{
	int l=strlen(str);
	for(int i=0;i<l;i++)
		data[datasize++]=str[i];
}

void compress_data_to_file(int &datasize)//��data�е�����ѹ�����ҷ����ļ���
{
	int lsrc=datasize,ldst=0;
	if(compress_str(data,lsrc,tempdata,ldst))
	{
		fwrite(&ldst,sizeof(int),1,save);
		fwrite(tempdata,sizeof(char),ldst,save);
	}
	else
	{
		int t=lsrc*-1;
		fwrite(&t,sizeof(int),1,save);
		fwrite(data,sizeof(char),lsrc,save);
	}
	datasize=0;
	memset(data,0,20000);
}

bool compress_file(char dst[])//ѹ�������ļ�
{
	remove(dst);
	save=fopen(dst,"ab");
	if(save==NULL)
	{
		cout <<dst<<" can't be created"<<endl;
		return false;
	}
	record_place=0;
	total_size=0;


	int i=0;
	char filename[1000];
	FILE *p;
	
	unsigned int num_block=0;
	unsigned int filesize=0;
	int datasize=0;
	char temp[1000];
	for(i=0;i<num_file;i++)
	{
		cout <<"compress "<<File[i].name<<endl;
		p=NULL;
		num_block=1;
		group_filename(filename,i);
		p=fopen(filename,"rb");
		if(p==NULL)
		{
			cout <<filename<<" can't read correctly!"<<endl;
			return false;
		}

		File[i].place_raw=datasize;//��¼ԭʼdata�е�λ��
		File[i].place_c=ftell(save);//��¼ѹ��������λ��
		unsigned int file_size=get_filesize(p);
		unsigned int now_size=0;
		while(!feof(p))
		{
			int size=fread(&data[datasize],sizeof(char),1000,p);
			datasize+=size;
			now_size+=size;
			show_percent(now_size,file_size);
			if(datasize>=MAX_CP)
			{
				total_size+=datasize;
				compress_data_to_file(datasize);
				num_block++;
			}
		}
		fclose(p);
		File[i].block=num_block;//��¼�����
	}
	if(datasize!=0)
	{
		compress_data_to_file(datasize);//ʣ�������ѹ��
	}
	record_place=ftell(save);

	fwrite(&num_dir,sizeof(int),1,save);//��¼Ŀ¼���ļ���Ϣ
	for(i=1;i<num_dir;i++)
	{
		fputs(Dir[i],save);
		fprintf(save,"\n");
		//fprintf(save,"%s\n",Dir[i]);
	}
	fprintf(save,"%d\n",num_file);
	for(i=0;i<num_file;i++)
	{
		fprintf(save,"%d %d %d %d\n",File[i].ndir,File[i].place_raw,File[i].place_c,File[i].block);
		fputs(File[i].name,save);
		fprintf(save,"\n");
		//fprintf(save,"%s\n",File[i].name);
	}
	//fseek(save,0L,SEEK_SET);
	char temp1[30];
	sprintf(temp1,"%u\n",record_place);
	fwrite(temp1,sizeof(char),20,save);
	fclose(save);
	cout <<endl;
	return true;
}