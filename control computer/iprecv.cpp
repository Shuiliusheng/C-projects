// udp实验程序.cpp : Defines the entry point for the console application.
//服务器端

#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#define buffer_length 1000

void main()
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
		return;
	}


	
	sSocket=socket(AF_INET,SOCK_DGRAM,0);
	if(sSocket==INVALID_SOCKET)
	{
		printf("socket() failed:%d\n",WSAGetLastError());
		return;
	}

    //初始化服务器SOCKET地址结构
	seradd.sin_family=AF_INET;
	seradd.sin_port=htons(5051);
	seradd.sin_addr.s_addr=htonl(INADDR_ANY);
	//seradd.sin_addr.s_addr= inet_addr("127.0.0.1");
	
	if(bind(sSocket,(LPSOCKADDR)&seradd,sizeof(seradd))==SOCKET_ERROR)
	{
		printf("地址绑定时出错:%d\n",WSAGetLastError());
		return;
	}

    ilen=sizeof(cliadd);

	//初始化接收缓冲区
    memset(recv_buf,0,sizeof(recv_buf));
	int n=1;
	char str[1030];
	while(1)
	{
		//在此函数中cli为传出参数
		irecv=recvfrom(sSocket,recv_buf,buffer_length,0,(struct sockaddr*)&cliadd,&ilen);
		if(irecv==SOCKET_ERROR)
		{
			printf("接收出错%d\n",WSAGetLastError());
			return;
		}
        else if(irecv==0)//发送方发送回车结束
			break;
		else
		{
			printf("No %d:",n++);
			printf("ip:[%s],port:[%d]\n",inet_ntoa(cliadd.sin_addr),ntohs(cliadd.sin_port));
			gets(recv_buf);
			sendto(sSocket,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&cliadd,ilen);
		}		
	}

	closesocket(sSocket);
	WSACleanup();
}
