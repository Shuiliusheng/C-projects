#include<winsock2.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//ws2_32.lib
#pragma comment(lib,"ws2_32.lib")
int main()
{
	FILE *p;
	char s[128];
	int a;
	int sendfile=0;
	printf("sendfile:input:1,else 0\n");
	scanf("%d",&sendfile);
	printf("1$$$%d\n",sendfile);

	char name[40];
	char ip[25];
	if(sendfile==1)
	{
		printf("1$$$%d\n",sendfile);
		printf("please input the filename need to be sent\n");
		scanf("%s",name);
		printf("2$$$%d\n",sendfile);
		p=fopen(name,"rb");
		if(!p)
			printf("sdd\n");
	}
	printf("input the receive IP :\n");
	
	scanf("%s",ip);
	printf("3$$$%d\n",sendfile);
	WSADATA wsaData;
	SOCKET client;
	SOCKADDR_IN addr;
	char buf[101];
	if(WSAStartup(0x202,&wsaData)!=0)
	{
		printf("initiate winsock dll error\n");
		return -1;
	}
	printf("4$$$%d\n",sendfile);
	client=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=inet_addr(ip);
	addr.sin_port=htons(6666);
	printf("5$$$%d\n",sendfile);
	if(!connect(client,(SOCKADDR*)&addr,sizeof(addr))==SOCKET_ERROR)
	{
		printf("connet failed:%d\n",WSAGetLastError());
		WSACleanup();
		return -1;
	}
	printf("****%d\n",sendfile);
	while(sendfile)
	{
		int n=0;
		printf("[i] message:");
		while(1)
		{
			n++;
			if(!feof(p))
			{
				a=fread(&s[1],1,100,p);
				s[0]=a;	
				send(client,s,a+1,0);

			}

			else
			{
				char str[106];
				for(int v=0;v<106;v++)
					str[v]='c';
				str[105]='\0';
				int b=100-a;///前一组数据传送和接受不够故而需要从b位置开始设判断位
				str[b]='1';
				str[b+1]='1';
				str[b+2]='s';
				str[0]='1';
				str[1]='1';
				str[2]='s';
				send(client,str,106,0);
				printf("%d,,,%s\n",a,str);
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
