#include "transport.h"
#include "socket.h"
#include "usart.h"
#include "w5500.h"
#include "types.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// uint8 server_ip[4]={115,29,225,215};			配置远程服务器IP地址

extern uint8_t domain_ip[4];

/*订阅消息发送数据函数*/
int transport_sendPacketBuffer(unsigned char* buf, int buflen)
{
  return send(SOCK_TCPS,buf,buflen);
}

/*发布消息发送数据函数*/
int transport_sendPacketBuffer1(unsigned char* buf, int buflen)
{
  return send(SOCK_TCPC,buf,buflen);
}

/*订阅消息接收数据函数*/
int transport_getdata(unsigned char* buf, int count)
{
  return recv(SOCK_TCPS,buf,count);
}

/*发布消息接收数据函数*/
int transport_getdata1(unsigned char* buf, int count)
{
  return recv(SOCK_TCPC,buf,count);
}

/*订阅消息打开相应SOCKET函数*/
int transport_open(void)
{
  int32_t ret;
  //新建一个Socket并绑定本地端口5000
  ret = socket(SOCK_TCPS,Sn_MR_TCP,50000,0x00);
  if(ret != 1){
    printf("%d:Socket Error\r\n",SOCK_TCPS);
    while(1);
  }else{
    printf("%d:Opened\r\n",SOCK_TCPS);
  }
	
  //连接MQTT服务器
	while(getSn_SR(SOCK_TCPS)!=SOCK_ESTABLISHED)
	{
		 ret = connect(SOCK_TCPS,domain_ip,61613);//端口必须为1883
	}
  if(ret != SOCK_OK){
    printf("%d:Socket Connect Error\r\n",SOCK_TCPS);
    while(1);
  }else{
    printf("%d:Connected\r\n",SOCK_TCPS);
  }		
	return 0;
}

/*发布消息打开相应SOCKET函数*/
int transport_open1(void)
{
  int32_t ret;
  //新建一个Socket并绑定本地端口5000
	//printf("打开端口 5001");
  ret = socket(SOCK_TCPC,Sn_MR_TCP,5001,0x00);
	//printf("端口5001打开");
  if(ret != 1){
    printf("%d:Socket1 Error1\r\n",SOCK_TCPC);
    while(1);
  }else{
    printf("%d:socket1 Opened\r\n",SOCK_TCPC);
  }

  //连接TCP服务器
	while(getSn_SR(SOCK_TCPC)!=SOCK_ESTABLISHED)
	{
		 ret = connect(SOCK_TCPC,domain_ip,61613);//端口必须为1883
	}
  if(ret != SOCK_OK){
    printf("%d:Socket Connect1 Error\r\n",SOCK_TCPC);
    while(1);
  }else{
    printf("%d:Connected1\r\n",SOCK_TCPC);
  }		
	return 0;
}

/*订阅消息关闭相应SOCKET函数*/
int transport_close(void)
{
 disconnect(SOCK_TCPS);
	printf("close0\n\r");
/*....等待完全断开连接.....*/
	while(getSn_SR(SOCK_TCPC)!=SOCK_CLOSED)
	{
		;
	}
  return 0;
}

/*发布消息关闭相应SOCKET函数*/
int transport_close1(void)
{
   disconnect(SOCK_TCPC);
	printf("close1\n\r");
/*....等待完全断开连接.....*/
	while(getSn_SR(SOCK_TCPC)!=SOCK_CLOSED)
	{
		;
	}
  return 0;
}
