#include "transport.h"
#include "socket.h"
#include "usart.h"
#include "w5500.h"
#include "types.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// uint8 server_ip[4]={115,29,225,215};			����Զ�̷�����IP��ַ

extern uint8_t domain_ip[4];

/*������Ϣ�������ݺ���*/
int transport_sendPacketBuffer(unsigned char* buf, int buflen)
{
  return send(SOCK_TCPS,buf,buflen);
}

/*������Ϣ�������ݺ���*/
int transport_sendPacketBuffer1(unsigned char* buf, int buflen)
{
  return send(SOCK_TCPC,buf,buflen);
}

/*������Ϣ�������ݺ���*/
int transport_getdata(unsigned char* buf, int count)
{
  return recv(SOCK_TCPS,buf,count);
}

/*������Ϣ�������ݺ���*/
int transport_getdata1(unsigned char* buf, int count)
{
  return recv(SOCK_TCPC,buf,count);
}

/*������Ϣ����ӦSOCKET����*/
int transport_open(void)
{
  int32_t ret;
  //�½�һ��Socket���󶨱��ض˿�5000
  ret = socket(SOCK_TCPS,Sn_MR_TCP,50000,0x00);
  if(ret != 1){
    printf("%d:Socket Error\r\n",SOCK_TCPS);
    while(1);
  }else{
    printf("%d:Opened\r\n",SOCK_TCPS);
  }
	
  //����MQTT������
	while(getSn_SR(SOCK_TCPS)!=SOCK_ESTABLISHED)
	{
		 ret = connect(SOCK_TCPS,domain_ip,61613);//�˿ڱ���Ϊ1883
	}
  if(ret != SOCK_OK){
    printf("%d:Socket Connect Error\r\n",SOCK_TCPS);
    while(1);
  }else{
    printf("%d:Connected\r\n",SOCK_TCPS);
  }		
	return 0;
}

/*������Ϣ����ӦSOCKET����*/
int transport_open1(void)
{
  int32_t ret;
  //�½�һ��Socket���󶨱��ض˿�5000
	//printf("�򿪶˿� 5001");
  ret = socket(SOCK_TCPC,Sn_MR_TCP,5001,0x00);
	//printf("�˿�5001��");
  if(ret != 1){
    printf("%d:Socket1 Error1\r\n",SOCK_TCPC);
    while(1);
  }else{
    printf("%d:socket1 Opened\r\n",SOCK_TCPC);
  }

  //����TCP������
	while(getSn_SR(SOCK_TCPC)!=SOCK_ESTABLISHED)
	{
		 ret = connect(SOCK_TCPC,domain_ip,61613);//�˿ڱ���Ϊ1883
	}
  if(ret != SOCK_OK){
    printf("%d:Socket Connect1 Error\r\n",SOCK_TCPC);
    while(1);
  }else{
    printf("%d:Connected1\r\n",SOCK_TCPC);
  }		
	return 0;
}

/*������Ϣ�ر���ӦSOCKET����*/
int transport_close(void)
{
 disconnect(SOCK_TCPS);
	printf("close0\n\r");
/*....�ȴ���ȫ�Ͽ�����.....*/
	while(getSn_SR(SOCK_TCPC)!=SOCK_CLOSED)
	{
		;
	}
  return 0;
}

/*������Ϣ�ر���ӦSOCKET����*/
int transport_close1(void)
{
   disconnect(SOCK_TCPC);
	printf("close1\n\r");
/*....�ȴ���ȫ�Ͽ�����.....*/
	while(getSn_SR(SOCK_TCPC)!=SOCK_CLOSED)
	{
		;
	}
  return 0;
}
