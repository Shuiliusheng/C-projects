#include"code_help.h"

bool read_record(char filename[],int n)
{
	FILE *read=NULL;
	read=fopen(filename,"r");
	if(read==NULL)
		return false;
	fgets(Record[n].time,13,read);
	Record[n].time[strlen(Record[n].time)-1]='\0';

	fgets(Record[n].lg,15,read);
	Record[n].lg[strlen(Record[n].lg)-1]='\0';

	fgets(Record[n].declarion,100,read);
	Record[n].declarion[strlen(Record[n].declarion)-1]='\0';
	
	char input[200];
	Record[n].ability[0]='\0';
	while(!feof(read))
	{
		fgets(input,200,read);
		input[strlen(input)-1]='\0';
		if(strcmp(input,"#")==0)
		{
			break;
		}
		strcat(Record[n].ability,input);
		strcat(Record[n].ability,"\n");
	}
	fgets(input,10,read);
	Record[n].kind=input[0];

	if(input[0]=='1')
	{
		fscanf(read,"%s",Record[n].h);
		fscanf(read,"%s",Record[n].cpp);
	}
	fclose(read);
	return true;
}

bool init()
{
	int i=0;
	char name[100];
	mkdir("code_help");
	for(i=0;i<MAX;i++)
	{
		sprintf(name,"code_help\\%d.rec",i);
		if(!read_record(name,i))
			break;
	}
	num_record=i;
	return true;
}


int main()
{
	char command[30];

	init();//初始化加载
	cout <<"Welcome Your Using!"<<endl;
	cout <<"**代码记录共"<<num_record<<"个"<<endl;
	while(1)
	{
		cout <<">>";
		gets(command);
		char str1[30],str2[30];
		depart_str(command,str1,str2);
		
		int n=0;
		sscanf(str2,"%d",&n);
		if(strcmp(command,"help")==0)
		{
			help();
		}
		else if(strcmp(command,"help add")==0)
		{
			help_add();
		}
		else if(strcmp(command,"help find")==0)
		{
			help_find();
		}
		else if(strcmp(command,"help del")==0)
		{
			help_del();
		}
		else if(strcmp(command,"help look")==0)
		{
			help_look();
		}
		else if(strcmp(command,"help code")==0)
		{
			help_code();
		}
		else if(strcmp(command,"help down")==0)
		{
			help_down();
		}
		else if(strcmp(command,"add")==0)
		{
			cout <<endl;

			if(num_record>=MAX)
				cout <<"records are full!"<<endl;
			else
			{
				add_record();
				num_record++;
			}
			cout <<endl;
		}
		else if(strcmp(command,"find")==0)
		{
			char Key_word[200]={0};
			cout <<endl<<"**find**"<<endl;
			cout <<"Language: ";
			gets(command);
			cout <<"Key Word: ";
			gets(Key_word);
			cout <<endl<<"搜索结果:"<<endl;
			find_record(command,Key_word);
			cout <<endl;
		}
		else if(strcmp(str1,"del")==0&&strcmp(command,"del")!=0)
		{
			cout <<endl;
			del_record(n);
			cout <<"记录"<<n<<"删除成功!"<<endl<<endl;
		}
		else if(strcmp(command,"look all")==0)
		{
			cout <<endl;
			for(int i=0;i<num_record;i++)
			{
				look_record(i);
				cout <<endl;
			}
		}
		else if(strcmp(str1,"look")==0&&strcmp(command,"look")!=0)
		{
			cout <<endl;
			look_record(n);
			cout <<endl;
		}
		else if(strcmp(str1,"code")==0&&strcmp(command,"code")!=0)
		{
			cout <<endl;
			code_record(n);
			cout <<endl;
		}
		else if(strcmp(str1,"down")==0&&strcmp(command,"down")!=0)
		{
			cout <<endl;
			down_record(n);
			cout <<endl;
		}
		else if(strcmp(command,"exit")==0)
		{
			break;
		}
	}
	return 0;
}

char Key[100][100]={0};
int num_key=0;

void get_key(char str[])
{
	int l=strlen(str);
	int i=0,n=0;
	for(i=0;i<l;i++)
	{
		if(str[i]==' ')
		{
			Key[num_key][i]='\0';
			num_key++;
			get_key(&str[i+1]);
			break;
		}
		Key[num_key][i]=str[i];
	}
	if(i==l)
	{
		Key[num_key][i]='\0';
		num_key++;
	}
}

bool find_word(char word[],int n)
{
	int l_word=strlen(word);
	int l_ability=strlen(Record[n].ability);
	for(int i=0;i<l_ability;i++)
	{
		int j=0;
		for(j=0;j<l_word;j++)
		{
			if(word[j]!=Record[n].ability[i+j])
				break;
		}
		if(j==l_word)
		{
			return true;
		}
	}
	return false;
}


void find_record(char lg[],char Key_word[])
{
	num_key=0;
	get_key(Key_word);

	int lg_flag=1,key_flag=1;
	lg_flag=strcmp(lg,"all");
	key_flag=strcmp(Key_word,"all");
	
	for(int i=0;i<num_record;i++)
	{
		if(lg_flag==0||strcmp(lg,Record[i].lg)==0)
		{
			if(key_flag==0)
				look_record(i);
			else
			{
				int j=0;
				for(j=0;j<num_key;j++)
				{
					if(!find_word(Key[j],i))
						break;
				}
				if(j==num_key)
					look_record(i);
			}
		}
	}
	
}

void depart_filename(char filename[])
{
	char temp[200];
	int l=strlen(filename);
	int n=0,i=0;
	for(i=l-1;i>=0;i--)
		if(filename[i]=='\\')
			break;
	for(i=i+1;i<=l;i++)
	{
		temp[n++]=filename[i];
	}
	strcpy(filename,temp);
}

void add_record()
{
	FILE *p=NULL;
	char str[200];
	sprintf(str,"code_help\\%d.rec",num_record);
	p=fopen(str,"w");
	if(p==NULL)
	{
		cout <<str<<" cann't be created!"<<endl;
	}
	get_time(Record[num_record].time);
	fprintf(p,"%s\n",Record[num_record].time);
	cout <<"输入代码语言: ";
	gets(Record[num_record].lg);
	fprintf(p,"%s\n",Record[num_record].lg);

	cout <<"输入类或函数的声明: "<<endl;
	gets(Record[num_record].declarion);
	fputs(Record[num_record].declarion,p);
	fputs("\n",p);
	
	cout <<"输入类或函数功能介绍（至多600字）:"<<endl;
	Record[num_record].ability[0]='\0';
	while(1)
	{
		gets(str);		
		fputs(str,p);
		fputs("\n",p);
		if(strcmp(str,"#")==0)
		{
			break;
		}
		strcat(Record[num_record].ability,str);
		strcat(Record[num_record].ability,"\n");
	}
	cout <<endl<<"(0:直接输入 | 1:文件输入 .h及.cpp文件) 请选择功能: ";
	gets(str);
	fprintf(p,"%s\n",str);
	Record[num_record].kind=str[0];
	if(str[0]=='0')
	{
		cout <<"请输入: 输入结束最后一行应为#"<<endl;
		FILE *code;
		sprintf(str,"code_help\\%d.code",num_record);
		code=fopen(str,"w");
		while(1)
		{
			gets(str);
			if(strcmp(str,"#")==0)
			{
				break;
			}
			fputs(str,code);
			fputs("\n",code);
		}
		fclose(code);
	}
	else if(str[0]=='1')
	{

		while(1)
		{
			char dst[100];
			cout <<"请输入.h文件具体位置及名称: ";
			gets(str);
			sprintf(dst,"code_help\\%d.h",num_record);
			if(!copy_file(str,dst))
				continue;
			else
				break;
		}
		cout <<str<<" load successfully!"<<endl<<endl;
		depart_filename(str);
		fprintf(p,"%s\n",str);
		while(1)
		{
			char dst[100];
			cout <<"请输入.cpp文件具体位置及名称: ";
			gets(str);
			sprintf(dst,"code_help\\%d.cpp",num_record);
			if(!copy_file(str,dst))
				continue;
			else
				break;
		}
		cout <<str<<" load successfully!"<<endl;
		depart_filename(str);
		fprintf(p,"%s\n",str);
	}
	fclose(p);
}

void get_time(char str[])
{
	time_t temp;
	struct tm *t;
	time(&temp);
	t=localtime(&temp);
	sprintf(str,"%d/%d/%d",t->tm_year+1900,t->tm_mon+1,t->tm_mday);
}

bool copy_file(char src[],char dst[])
{
	FILE *read=NULL;
	FILE *save=NULL;
	read=fopen(src,"r");
	save=fopen(dst,"w");
	if(read==NULL)
	{
		cout <<src<<" cann't be found!"<<endl;
		return false;
	}
	if(save==NULL)
	{
		cout <<dst<<" cann't be created!"<<endl;
		return false;
	}
	while(!feof(read))
	{
		char c=fgetc(read);
		if(c!=-1)
			fputc(c,save);
	}
	fclose(read);
	fclose(save);
	return true;
}

void depart_str(char src[],char dst1[],char dst2[])
{
	int l=strlen(src);
	int i=0,n=0;
	for(i=0;i<l;i++)
	{
		if(src[i]==' ')
		{
			dst1[i]='\0';
			i++;
			break;			
		}
		dst1[i]=src[i];
	}
	for(i;i<=l;i++)
		dst2[n++]=src[i];
}

void clear_record(int n)
{
	char name[100];
	sprintf(name,"code_help\\%d.rec",n);
	remove(name);
	sprintf(name,"code_help\\%d.code",n);
	remove(name);
	sprintf(name,"code_help\\%d.cpp",n);
	remove(name);
	sprintf(name,"code_help\\%d.h",n);
	remove(name);
}

void swap_record(int src,int dst)
{
	char name[100],name1[100];
	sprintf(name,"code_help\\%d.rec",src);
	sprintf(name1,"code_help\\%d.rec",dst);
	rename(name,name1);
	sprintf(name,"code_help\\%d.cpp",src);
	sprintf(name1,"code_help\\%d.cpp",dst);
	rename(name,name1);
	sprintf(name,"code_help\\%d.h",src);
	sprintf(name1,"code_help\\%d.h",dst);
	rename(name,name1);
	sprintf(name,"code_help\\%d.code",src);
	sprintf(name1,"code_help\\%d.code",dst);
	rename(name,name1);

	Record[dst]=Record[src];

}

void del_record(int n)
{
	if(n>=num_record)
		return ;
	clear_record(n);
	if(n!=num_record-1)
	{
		swap_record(num_record-1,n);	
	}
	num_record--;
}

void look_record(int n)
{
	if(n>=num_record)
		return;
	cout <<"记录编号:"<<n<<endl;
	cout <<"录入时间:"<<Record[n].time<<endl;
	cout <<"代码语言:"<<Record[n].lg<<endl;
	cout <<"具体声明:"<<Record[n].declarion<<endl;
	cout <<"功能简介:\n"<<Record[n].ability<<endl;
}

void print_file(char filename[])
{
	FILE *p=NULL;
	p=fopen(filename,"r");
	if(p==NULL)
	{
		cout <<filename<<" cann't be read"<<endl;
		return ;
	}
	while(!feof(p))
	{
		char c=fgetc(p);
		if(c!=-1)
			cout <<c;
	}
	fclose(p);
}

void code_record(int n)
{
	if(n>=num_record)
		return;
	char str[100];
	if(Record[n].kind=='0')
	{
		sprintf(str,"code_help\\%d.code",n);
		print_file(str);
		cout <<endl;
	}
	else
	{
		cout <<endl;
		cout <<"*******"<<Record[n].h<<"*******"<<endl;
		sprintf(str,"code_help\\%d.h",n);
		print_file(str);
		cout <<endl<<endl<<"*******"<<Record[n].cpp<<"*******"<<endl;
		sprintf(str,"code_help\\%d.cpp",n);
		print_file(str);
		cout <<endl;
	}
}

void down_record(int n)
{
	if(n>=num_record)
		return;
	char dst[300],src[300];
	cout <<"输入代码需要保存的路径:"<<endl;
	gets(dst);

	if(Record[n].kind=='0')
	{
		sprintf(src,"code_help\\%d.code",n);
		sprintf(dst,"%s\\code%d.cpp",dst,n);
		copy_file(src,dst);
		cout <<"保存成功"<<endl;
		cout <<endl;
	}
	else
	{
		sprintf(src,"code_help\\%d.h",n);
		char dst1[300];
		sprintf(dst1,"%s\\%s",dst,Record[n].h);
		copy_file(src,dst1);
		
		sprintf(src,"code_help\\%d.cpp",n);
		sprintf(dst,"%s\\%s",dst,Record[n].cpp);
		copy_file(src,dst);
		cout <<"保存成功"<<endl;
		cout <<endl;
	}
}