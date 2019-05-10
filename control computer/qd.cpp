#include<windows.h>

int main()
{
	ShellExecute(NULL,"open","D:\\rcopy.exe",NULL,NULL,SW_HIDE);
	return 0;
}
