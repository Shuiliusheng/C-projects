#include<winsock2.h>
#include<stdio.h>
#include<string.h>
//ws2_32.lib
#pragma comment(lib,"ws2_32.lib")
int main()
{
	WSADATA wsaData;
	PHOSTENT hostinfo;
	SOCKET server,accSock;
	SOCKADDR_IN addr,addrout;
	FILE *p;
	int b,recefile=0;
	printf("recefile:input:1,else 0\n");
	scanf("%d",&recefile);

	char name[40];
	if(recefile==1)
	{
		printf("input the name need be conservation\n");
		scanf("%s",name);
		p=fopen(name,"wb");
	}
	char s[101],num;
	int port,len=sizeof(SOCKADDR),count=0;
	char buf[128],*ip;
	if(WSAStartup(0x202,&wsaData)!=0)
	{
		printf("initiate winsock dll error\n");
		return -1;
	}
	server=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	addr.sin_port=htons(6666);
	bind(server,(SOCKADDR*)&addr,sizeof(addr));
	gethostname(buf,sizeof(buf));
	hostinfo=gethostbyname(buf);
	ip=inet_ntoa(*(IN_ADDR*)*hostinfo->h_addr_list);
	listen(server,1);
	printf("server %s waiting for connection...\n",ip);
	accSock=accept(server,(SOCKADDR*)&addrout,&len);

	ip=inet_ntoa(addrout.sin_addr);
	printf("11:%s\n",ip);
	port=htons(addrout.sin_port);

	printf("[S]accept client %s:%d\n",ip,port);
	int n=0,g=0;
	while(recefile)
	{
		n++;
		recv(accSock,s,101,0);
		
		if(s[0]=='1'&&s[1]=='1'&&s[2]=='s'&&s[4]=='c')
		{
			fclose(p);
			printf("%d\n",strlen(s));
			printf("%s\n",s);

			printf("receive successfully\n");
			break;
		}
		num=s[0];
		int c=num;
		b=fwrite(&s[1],1,c,p);
	}
	printf("please input quit\n");
	while(1)
	{		
		scanf("%s",buf);
		send(accSock,buf,strlen(buf)+1,0);
		char str[30];
		recv(accSock,str,strlen(str)+1,0);		
		if(strcmp(str,"quit")==0)break;
		printf("%s\n",str);
	}
	closesocket(accSock);
	closesocket(server);
	WSACleanup();
	return 0;
}
