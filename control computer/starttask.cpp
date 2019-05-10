#include<windows.h>
#include<stdio.h>
int main()
{
	char dest[100]="v_use.exe";
	while(1)
	{
		int flag=0;
		system("tasklist >task.txt");
		SetFileAttributes("task.txt",FILE_ATTRIBUTE_HIDDEN);
		FILE *p=fopen("task.txt","rb");
		while(!feof(p))
		{
			char task[150]={0};
			fscanf(p,"%s",task);
			if(strcmp(task,dest)==0)
			{
				FILE *f=fopen("testuse.txt","rb");
				int t=0;
				if(f!=NULL)
					fscanf(f,"%d",&t);
				printf("%d t\n",t);
				if(t==0)
					system("taskkill -f -im v_use.exe");
				flag=1;
				fclose(f);
				break;
			}
		}
		fclose(p);
		if(flag==0)
		{
			ShellExecute(NULL,"open","v_use.exe",NULL,NULL,SW_HIDE);
		}
		Sleep(10000);
	}
	return 0;
}