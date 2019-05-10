#include<winsock2.h>
#include<stdio.h>
#include<string.h>
//ws2_32.lib
//receive
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
	//scanf("%d",&recefile);
	recefile=1;
	char name[40]="f://2.mp4"; 
	if(recefile==1)
	{
		printf("input the name need be conservation\n");
		//scanf("%s",name);
		p=fopen(name,"wb");
	}
	char s[515],num;
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
	int n=-1,g=0;
	while(recefile)
	{
		n++;
		recv(accSock,s,515,0);
		int c=(int)s[1]*100+(int)s[0];
		if(n<100)
			printf("%d,%d--",c,n);
		
		/*if(s[0]=='c'&&s[1]=='c'&&s[2]=='c'&&s[4]=='c')
		{
			fclose(p);
			printf("%d\n",strlen(s));
			printf("%s\n",s);
			printf("\n%d,,%d\n",n,c);
			printf("receive successfully\n");
			break;
		}*/
		if(n>0)
		{			
			if(!(c<g))
				c=g;
		}
		if(n!=0&&c!=g)
		{
			printf("DDD0  %d---%d----%d\n",n,g,c);
			b=fwrite(&s[2],1,c,p);
			fclose(p);
			break;
		}
		g=c;
		b=fwrite(&s[2],1,c,p);
		
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
