// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#include <direct.h> 
#include <time.h>
#include <stdio.h>
#include <tchar.h>
#include "zlib.h"
#include<iostream>
#include <io.h>
#include <stdio.h>
#include<windows.h>
using namespace std;
#pragma comment(lib, "zlib.lib")

typedef struct{
	int dir;
	int write_time;
	char filename[200];
}File;

typedef struct{
	char time[30];
	int log_num;
	char info[500];
}LOG;

//zlib_file_fuction.cpp
void uncompress_file(char src_file[],char dst_file[]);
void compress_file(char src_file[],char dst_file[]);
bool uncompress_str(char src[],int lsrc,char dst[],int &ldst);
bool compress_str(char src[],int lsrc,char dst[],int &ldst);
void get_now_time(char str_time[]);

//gitwork.cpp
void copy_path(char *path,int dir);
void printDir(char* path,int dir);
void save_detail_log(char filename[]);
void show_read();
void read_log();
void write_log();
void init_git();  
void create_log(char dir_log[]);
void init_log(int n,char dir_log[]);
void input_info();
void create_first(char dir_log[]);
void show_log();
bool read_dir_file(int log);//读取某一次的detail_log文件

int find_dir(int n);
void find_different_dir();
int find_file(int n);
void find_different_file();
void save_change_file(char dir_log[]);
void make_operation_log(char dir_log[]);
void find_different();

#define MK_D 1
#define DEL_D 2
#define MK_F 3
#define DEL_F 4
#define CLEAR 5

//recovery_log.cpp
bool recovery_log(int log);
bool recovery_one_log(int log);
bool work_operator_log(int log);
bool recover_first_log();
void del_git();
void del_log(char path[]);
bool read_dir_file_2(int log);

//constrain.cpp
void show_constraint();
void add_dir_constraint();
void add_file_constraint();
bool add_constraint_from_file(char filename[]);
bool save_constraint_to_file(char filename[]);
bool judge_dir_constraint(char dir[]);
bool judge_file_constraint(char filename[]);
void del_dir_constraint(char dir[]);
void del_file_constraint(char filename[]);


//delete_log.cpp

bool delete_log(int log);
// TODO: 在此处引用程序需要的其他头文件
