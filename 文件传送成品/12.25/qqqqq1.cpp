#include<winsock2.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//ws2_32.lib
//send file
#pragma comment(lib,"ws2_32.lib")
int main()
{
	FILE *p;
	char s[515];
	int a;
	int sendfile=0;
	printf("sendfile:input:1,else 0\n");
	//scanf("%d",&sendfile);
	sendfile=1;

	char name[60]="f://1.mp4";
	char ip[25]="127.0.0.1"; 
	if(sendfile==1)
	{
		printf("please input the filename need to be sent\n");
		//scanf("%s",name);
		p=fopen(name,"rb");
		if(!p)
			printf("the file is not exit!\n");
	}
	//printf("input the receive IP :\n");
	
	//scanf("%s",ip);
	WSADATA wsaData;
	SOCKET client;
	SOCKADDR_IN addr;
	char buf[101];
	if(WSAStartup(0x202,&wsaData)!=0)
	{
		printf("initiate winsock dll error\n");
		return -1;
	}
	client=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=inet_addr(ip);
	addr.sin_port=htons(6666);
	if(!connect(client,(SOCKADDR*)&addr,sizeof(addr))==SOCKET_ERROR)
	{
		printf("connet failed:%d\n",WSAGetLastError());
		WSACleanup();
		return -1;
	}

	while(sendfile)
	{
		int n=0;
		printf("\" %s \" is sending,please wait for a moment!\n",name);
		printf("[i] message:");
		while(1)
		{
			if(!feof(p))
			{
				n++;
				a=fread(&s[2],1,512,p);
				s[0]=a%100;
				s[1]=(a-s[0])/100;
				s[514]='\0';
				send(client,s,a+3,0);
			}			
			else
			{
				//printf("aa%d\n",a);
				//printf("nnn%d\n",n);
				/*char str[515];
				for(int v=0;v<514;v++)
					str[v]='c';
				str[514]='\0';
				send(client,str,515,0);*/
				printf("send success\n");
				break;
			}
		}
		fclose(p);
		break;
	}
	
	while(1)
	{
		recv(client,buf,sizeof(buf),0);
		if(strcmp(buf,"quit")==0)
		{
			char st[6]="quit";
			send(client,st,strlen(st)+1,0);
			break;
		}
		else
		{
			printf("wait quit\n");
			char str[]="please input:quit";
			send(client,str,strlen(str)+1,0);
		}
	}		
	closesocket(client);
	WSACleanup();
	return 0;
}
