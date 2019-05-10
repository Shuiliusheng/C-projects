#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#define buffer_length 102400
#define DATA_BUFFER  512

void recvfile()
{
	int i=0;
	WSADATA wsaData;
	SOCKET oldSocket,newSocket;

	//客户地址长度
	int iLen=0;

	//接收的数据长度
	int ircv =0;

	//客户和服务器的SOCKET地址结构
	struct sockaddr_in ser,cli;

	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
	{
		cout<<"failed to load winsock"<<endl;
		return;
	}

	char name[100];
	
	cout<<"server waiting"<<endl;
	cout<<"---------------"<<endl;



	//创建服务器端帧听SOCKET
	oldSocket=socket(AF_INET,SOCK_STREAM,0);
	if(oldSocket==INVALID_SOCKET)
	{
		cout<<"socket() failed:"<<WSAGetLastError()<<endl;
		return;
	}


	//以下是建立服务器端的SOCKET地址结构
	ser.sin_family=AF_INET;
	ser.sin_port=htons(8080);
	//使用系统指定的ip地址INADDR_ANY
	ser.sin_addr.s_addr=htonl(INADDR_ANY);


	if(bind(oldSocket,(LPSOCKADDR)&ser,sizeof(ser))==SOCKET_ERROR)
	{
		cout<<"bind() failed:"<<WSAGetLastError()<<endl;
		return;
	}

	//进入侦听状态
	if(listen(oldSocket,5)==SOCKET_ERROR)
	{
		cout<<"listen() failed:"<<WSAGetLastError()<<endl;
		return;
	}

	//接收客户端的连接
	iLen=sizeof(cli);

	newSocket=accept(oldSocket,(struct sockaddr*)&cli,&iLen);//产生一个新的SOCKET

	if(newSocket==INVALID_SOCKET)
	{
		cout<<"accept() failed:"<<WSAGetLastError()<<endl;//
		return ;
	}

	//进入一个无限循环，等待客户发送数据

	
	cout <<"请输入要保存的文件名:"<<endl;
	gets(name);
	FILE *p=fopen(name,"wb");
	double n=0;
	while(1)
	{	  
		char temp[30];
		memset(temp,0,30);	
		ircv=recv(newSocket,temp,20,0);
		n+=ircv;
		fwrite(temp,1,ircv,p);
		printf("%0.2lfKB\r",n/1024);
		if(ircv!=20)
		{
			fclose(p);
			printf("%0.2lfKB\n",n/1024);
			printf("over!!!\n\n");
			break;
		}
	}
	closesocket(newSocket);
	closesocket(oldSocket);
	WSACleanup();
}

void sendf(char *name)
{
	WSADATA wsaData;
	SOCKET oldSocket,newSocket;

	//客户地址长度
	int iLen=0;

	//接收的数据长度
	int ircv =0;

	//客户和服务器的SOCKET地址结构
	struct sockaddr_in ser,cli;

	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
	{
		cout<<"failed to load winsock"<<endl;
		return;
	}

	//创建服务器端帧听SOCKET
	oldSocket=socket(AF_INET,SOCK_STREAM,0);
	if(oldSocket==INVALID_SOCKET)
	{
		cout<<"socket() failed:"<<WSAGetLastError()<<endl;
		return;
	}


	//以下是建立服务器端的SOCKET地址结构
	ser.sin_family=AF_INET;
	ser.sin_port=htons(8080);
	//使用系统指定的ip地址INADDR_ANY
	ser.sin_addr.s_addr=htonl(INADDR_ANY);


	if(bind(oldSocket,(LPSOCKADDR)&ser,sizeof(ser))==SOCKET_ERROR)
	{
		cout<<"bind() failed:"<<WSAGetLastError()<<endl;
		return;
	}

	//进入侦听状态
	if(listen(oldSocket,5)==SOCKET_ERROR)
	{
		cout<<"listen() failed:"<<WSAGetLastError()<<endl;
		return;
	}

	//接收客户端的连接
	iLen=sizeof(cli);

	newSocket=accept(oldSocket,(struct sockaddr*)&cli,&iLen);//产生一个新的SOCKET

	if(newSocket==INVALID_SOCKET)
	{
		cout<<"accept() failed:"<<WSAGetLastError()<<endl;//
		return ;
	}

	//进入一个无限循环，等待客户发送数据

	
	FILE *p=NULL;
	p=fopen(name,"rb");
	if(p==NULL)
	{
		printf("file is not exist!\n\n");
		return ;
	}
	
	fseek(p,0,SEEK_SET);
	fseek(p,0,SEEK_END);
	long num=ftell(p);// longBytes就是文件的长度
	fseek(p,0,SEEK_SET);

	char temp[30];
	memset(temp,0,30);
	double n=0;
	while(1)
	{
		if(!feof(p))
		{
			int t=fread(temp,1,20,p);
			int t1=send(newSocket,temp,t,0);
			memset(temp,0,30);
			n+=t1;
			if(t1!=20)
				break;
		}
		else
		{			
			break;	
		}
		printf("%0.2lf%%\r",100*n/num);
	}
	printf("over!!!!\n\n");
	fclose(p);
	closesocket(newSocket);
	closesocket(oldSocket);
	WSACleanup();
}

int main()
{
	WSADATA wsadata;
	SOCKET sSocket;

	int ilen;
	int irecv;
	char recv_buf[buffer_length];
	//服务器和客户SOCKET地址结构
	struct sockaddr_in seradd,cliadd;


	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
	{
		printf("failed to load winsocket\n");
		return 0;
	}


	
	sSocket=socket(AF_INET,SOCK_DGRAM,0);
	if(sSocket==INVALID_SOCKET)
	{
		printf("socket() failed:%d\n",WSAGetLastError());
		return 0;
	}

    //初始化服务器SOCKET地址结构
	seradd.sin_family=AF_INET;
	seradd.sin_port=htons(8081);
	seradd.sin_addr.s_addr=htonl(INADDR_ANY);

	
	if(bind(sSocket,(LPSOCKADDR)&seradd,sizeof(seradd))==SOCKET_ERROR)
	{
		printf("地址绑定时出错:%d\n",WSAGetLastError());
		return 0;
	}

    ilen=sizeof(cliadd);

	//初始化接收缓冲区
    memset(recv_buf,0,sizeof(recv_buf));
	int n=1;
	char str[1030]={0};
	irecv=recvfrom(sSocket,recv_buf,buffer_length,0,(struct sockaddr*)&cliadd,&ilen);
	if(irecv==SOCKET_ERROR)
	{
		printf("接收出错%d\n",WSAGetLastError());
		return 0;
	}
	
	printf("%s>",recv_buf);
	while(1)
	{
		
		gets(str);

		sendto(sSocket,str,sizeof(str),0,(struct sockaddr *)&cliadd,ilen);
		memset(recv_buf,0,buffer_length);
		
		if(strcmp(str,"cd")==0)
		{
			memset(str,0,1030);
			gets(str);
			sendto(sSocket,str,sizeof(str),0,(struct sockaddr *)&cliadd,ilen);
		}
		else if(strcmp(str,"cls")==0)
		{
			for(int e=0;e<40;e++)
				printf("           |\n");
		}
		else if(strcmp(str,"file")==0)
		{
			memset(str,0,1030);
			gets(str);
			sendto(sSocket,str,sizeof(str),0,(struct sockaddr *)&cliadd,ilen);
			recvfile();
		}
		else if(strcmp(str,"sendfile")==0)
		{
			char temp[500];
			memset(str,0,1030);
			printf("sendfile name:");
			gets(temp);
			printf("sendfile store name:");
			gets(str);
			sendto(sSocket,str,sizeof(str),0,(struct sockaddr *)&cliadd,ilen);
			sendf(temp);
		}
		else if(strcmp(str,"down")==0)
		{
			printf("thanks for your using!\n\n");
			return 0;
		}
		else
		{
			irecv=recvfrom(sSocket,recv_buf,buffer_length,0,(struct sockaddr*)&cliadd,&ilen);
			if(irecv==SOCKET_ERROR)
			{
				printf("接收出错%d\n",WSAGetLastError());
				return 0;
			}
			printf("%s",recv_buf);
		}
		memset(recv_buf,0,sizeof(recv_buf));
		recvfrom(sSocket,recv_buf,buffer_length,0,(struct sockaddr*)&cliadd,&ilen);
		printf("%s>",recv_buf);
	}

	closesocket(sSocket);
	WSACleanup();
	return 0;
}