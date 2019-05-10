#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

#define ENTER 13
#define BACKSPACE 8

#define MOVE 0xE0
#define UP 0x48
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define END 79
#define HOME 71
#define PGUP 73
#define PGDOWN 81

#define ESC 27
#define FUC_F 0
#define F1 59
#define F2 60

#define CTRL_E 5
#define CTRL_F 6
#define CTRL_O 15


void add_to_buf(FILE *p);
bool start();
void set_cursor(int x,int y);
void dispear_char_line(int y);
void show_str(int y,int num,char str[]);
void show_buf();
void input_str(int &num,char buf[],int input);
void dispear_screen();
int find_str(int &x,int &y);