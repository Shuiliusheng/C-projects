#include<iostream>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<direct.h>
using namespace std;
#define MAX 10000
typedef struct{
	char lg[15];
	char time[13];
	char declarion[100];
	char ability[600];//最多三百个汉字
	char kind;
	char h[100];
	char cpp[100];
}RECORD;

RECORD Record[10000];//记录
int num_record=0;

void depart_str(char src[],char dst1[],char dst2[]);

bool read_record(char filename[],int n);
bool init();


void help();
void help_add();
void help_find();
void help_del();
void help_look();
void help_code();
void help_down();

void get_time(char str[]);
bool copy_file(char src[],char dst[]);
void add_record();

void get_key(char str[]);
bool find_word(char word[],int n);
void find_record(char lg[],char Key_word[]);

void clear_record(int n);
void swap_record(int src,int dst);
void del_record(int n);

void print_file(char filename[]);
void look_record(int n);

void code_record(int n);

void down_record(int n);

void help()
{
	cout <<endl;
	cout <<"**help information**"<<endl;
	cout <<"**  add :add a code record (detail: help add)"<<endl;
	cout <<"** find :find some code records with some requirements (detail:help find)"<<endl;
	cout <<"**  del :del a code record (deltai: help del)"<<endl;
	cout <<"** look :print a code record detail introduce (deltai: help look)"<<endl;
	cout <<"** code :print the codes of the record (deltai: help code)"<<endl;
	cout <<"** down :save the codes of the record (deltai: help down)"<<endl;
	cout <<endl;
}

void help_add()
{
	cout <<endl;
	cout <<"** command: add **"<<endl;
	cout <<"** use method: add"<<endl;
	cout <<"** ability: add one code record,the record information according "<<endl;
	cout <<"** ability: tips to input"<<endl;
	cout <<"** ability: declarion requires giving the fuction or class declarion"<<endl;
	cout <<"** ability: language requires giving the code language like: C,C++,C#,Jave,C#,Qt,Python"<<endl;
	cout <<"** ability: ability requires giving some information of the code abilities"<<endl;
	cout <<"** ability: like >>**********"<<endl;
	cout <<"** ability:      >>**********"<<endl;
	cout <<"** ability:      >># ---> the end flag"<<endl;
	cout <<endl;
}
void help_find()
{
	cout <<endl;
	cout <<"** command: find **"<<endl;
	cout <<"** use method: find"<<endl;
	cout <<"** ability: find some records according to some requirements users given"<<endl;
	cout <<"** ability: code language (C,C++,Java,C#,Qt,Python)/all"<<endl;
	cout <<"** ability: some key words of the introduce of records, like: 加法 浮点型/all"<<endl;
	cout <<endl;
}
void help_del()
{
	cout <<endl;
	cout <<"** command: del **"<<endl;
	cout <<"** use method: del record num"<<endl;
	cout <<"** ability: delete the record user want"<<endl;
	cout <<endl;
}
void help_look()
{
	cout <<endl;
	cout <<"** command: look **"<<endl;
	cout <<"** use method: look record num"<<endl;
	cout <<"** ability: print the detail info of the record to screen"<<endl;
	cout <<endl;
}
void help_code()
{
	cout <<endl;
	cout <<"** command: code **"<<endl;
	cout <<"** use method: code record num"<<endl;
	cout <<"** ability: print the code of the record to the screen"<<endl;
	cout <<endl;
}
void help_down()
{
	cout <<endl;
	cout <<"** command: down **"<<endl;
	cout <<"** use method: down record num"<<endl;
	cout <<"** ability: save the code of the record to a place user want"<<endl;
	cout <<endl;
}

