// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <windows.h>
#include <direct.h> 
#include <io.h>
#include"zlib.h"
#pragma comment(lib, "zlib.lib")
using namespace std;

typedef struct{
	unsigned int ndir;
	char name[300];
	unsigned int place_c;
	unsigned int place_raw;
	unsigned int block;
}FIle;

bool compress_str(char src[],int lsrc,char dst[],int &ldst);
bool uncompress_str(char src[],int lsrc,char dst[],int &ldst);

void compress_file(char src_file[],char dst_file[]);
void uncompress_file(char src_file[],char dst_file[]);

void init();

bool judge_dir_useless(char dir[]);
bool judge_file_useless(char filename[]);

void printDir(char* path,int dir);

void show_dir();
void precompress_dir_file();
void add_compress_dir();
void add_compress_file();

void del_add_dir(char name[]);
void del_add_file(char name[]);

bool judge_file_exist(char name[]);
bool judge_dir_exist(char path[]);

void add_useless_dir();
void add_useless_file();

bool compress_file(char dst[]);
unsigned int get_filesize(FILE *p);

bool compress_file(char dst[]);
unsigned int get_filesize(FILE *p);

bool pre_uncompress_work(char filename[]);
bool uncompress_work_dir(char name[],char dstpath[]);
bool uncompress_work_file(char filename[],char dst[]);


void show_percent(unsigned int now,unsigned int total);
#define MAX_CP 10000

// TODO: 在此处引用程序需要的其他头文件
