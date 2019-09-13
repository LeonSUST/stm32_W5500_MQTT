#include "mqtt.h"
#include "MQTTPacket.h"
#include "transport.h"
#include "types.h"
#include "cJSON.h"
#include "socket.h"
#include "w5500.h"
#include "ult.h"
#include "device.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

extern uint8 domain_ip[4];

//MQTT发布消息函数
int do_mqtt_publish(char *pTopic,char *pMessage)
{
	int32_t len,rc;
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;//配置部分可变头部的值
	unsigned char buf[100];
	unsigned char BUF[100];
	MQTTString topicString = MQTTString_initializer;
	int msglen = strlen(pMessage);//计算发布消息的长度
	int buflen = sizeof(buf);
	int BUFLEN = sizeof(BUF);
	
	transport_open1();//打开发布消息的SOCKET2016.12.29
	
	
	
	data.clientID.cstring = "me";//客户端标识，用于区分每个客户端
	data.keepAliveInterval = 60;//保活计时器，定义了服务器收到客户端消息的最大时间间隔
	data.cleansession = 1;//该标志置1服务器必须丢弃之前保持的客户端的信息，将该连接视为“不存在”
	data.username.cstring = "admin";
	data.password.cstring = "password";
	len = MQTTSerialize_connect(buf, buflen, &data); /*1 构造连接报文*/
	/*2016年12.29改*/
	rc = transport_sendPacketBuffer1(buf, len);//发送连接请求
	if(rc != len)
	{
		printf("publish connect transport_sendPacketBuffer error\n\r");
	}
	
	{
		unsigned char sessionPresent, connack_rc;
		do
		{
			while(MQTTPacket_read(buf, buflen, transport_getdata1) != CONNACK)//对接收到的报文进行解析
			{
				;
			}
		}
		while(MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0);
		printf("PUBLISH CONNACK is ok %s\r\n",buf);
	}
	memset(buf,0,buflen);
  strcat(pTopic,data.clientID.cstring);//合并字符串为真正主题
	topicString.cstring = pTopic;
	len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)pMessage, msglen); /*2 构造发布消息的报文*/
	
	rc = transport_sendPacketBuffer1(buf,len);//发送消息
	memset(buf,0,buflen);
  //transport_close1();//关闭相应SOCKET2016.12.30
	if (rc == len)
		printf("Successfully published\n\r");
	else
		printf("Publish failed\n\r");
	return 0;
}

//MQTT订阅消息函数
int do_mqtt_subscrib(char *pTopic,char *pMessage)
{
	/*定义一个MQTTPacket_connectData类型结构体变量，该结构体包含可变头部内容*/
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	int rc = 0;
	
	int j = 0;
	int i = 0;
	
	unsigned char dup;
	int qos;
	unsigned char retained;
	unsigned short mssageid;
	int payloadlen_in;
	unsigned char* payload_in;
	MQTTString receivedTopic;
		
	char *led;
	char led_value;
	unsigned char buf[200];
	int buflen = sizeof(buf);
	int msgid = 1;//该值为消息标识符
	/*定义了一个MQTTString结构体类型变量*/
	MQTTString topicString = MQTTString_initializer;
	int req_qos = 0;
	//int num = 0;
	int len = 0;
	rc = transport_open();//打开socket端口
	if(rc < 0)
	{
		printf("transport_open error\n\r");
		return rc;
	}
	data.clientID.cstring = "ABCD";//客户端标识，用于区分每个客户端
	data.keepAliveInterval = 60;//服务器保持连接时间，超过该时间后，服务器会主动断开连接，单位为秒
	data.cleansession = 1;//该标志置1服务器必须丢弃之前保持的客户端的信息，将该连接视为“不存在”
	data.username.cstring = "admin";
	data.password.cstring = "password";
	len = MQTTSerialize_connect(buf, buflen, &data);//对于Connect Flag字节的构造在该函数中进行并将对应值放入 buf缓存中
	rc = transport_sendPacketBuffer(buf, len);//发送连接请求
	if(rc != len)
	{
		printf("connect transport_sendPacketBuffer error\n\r");
	}
	
	/*等待CONNACK数据包的到达connack_rc中存放的是Return Code的值该值为0时表示连接建立成功*/
	{
		unsigned char sessionPresent, connack_rc;
		do
		{
			while(MQTTPacket_read(buf, buflen, transport_getdata) != CONNACK)//对接收到的报文进行解析
			{
				;
			}
		}
		while(MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0);
		printf("CONNACK is ok %s\r\n",buf);
	}
	
	/* subscribe 发送订阅主题*/
	topicString.cstring = pTopic;
	len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);//构造订阅主题报文
	rc = transport_sendPacketBuffer(buf, len);
	if(rc != len)
	{
		printf("connect transport_sendPacketBuffer error\n\r");
	}
	/*等待SUBACK 订阅确认报文的到来*/	
	{
		unsigned short submsgid;
		int subcount;
		int granted_qos;
		do
		{
			while(MQTTPacket_read(buf, buflen, transport_getdata) != SUBACK)
			{
				;
			}
			rc = MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
		}
		while (granted_qos != 0);
		printf("SUBACK is ok\r\n");
	}
	
	/* loop getting msgs on subscribed topic */
	topicString.cstring = pTopic;
	memset(buf,0,buflen);
	/*开启定时器*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	/*接收数据会阻塞，除非服务器断开连接后才返回*/
	while(getSn_SR(SOCK_TCPS)==SOCK_ESTABLISHED)
	{
		/*发送PINGREQ-PING请求 该消息是一个从客户端发送到服务器的“are you alive”消息（类似心跳包）*/
		i=MQTTSerialize_pingreq(buf,buflen);
		j=transport_sendPacketBuffer(buf, i);
		if(j!=i)
		{
			printf("connect transport_sendPacketBuffer error\n\r");
		}
		memset(buf,0,buflen);
		
		/*判断是否接收到发布消息的数据包*/
		if (MQTTPacket_read(buf, buflen, transport_getdata) ==PUBLISH )
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*当订阅消息有更新时，先暂时关闭中断*/
			rc = MQTTDeserialize_publish(&dup, &qos, &retained, &mssageid, &receivedTopic,
			&payload_in, &payloadlen_in, buf, buflen);
			printf("message arrived %d: %s\n\r", payloadlen_in, payload_in);
			strcpy(pMessage,(const char *)payload_in);
			printf("message is %s",pMessage);
			if(rc==1)
			{
				//解析JSON格式字符串并点亮相应的LED灯
				cJSON *root = cJSON_Parse(pMessage);
				if(root != NULL)
				{
					led = cJSON_GetObjectItem(root,"led")->valuestring;
					printf("led = %s\n\r",led);
					led_value = cJSON_GetObjectItem(root,"value")->valueint;
					if(!strcmp(led,"red"))
					{
						if(led_value)
						{
							LED_ON(LED_RED);
							printf("Red On\n\r");
						}
						else
						{
							LED_OFF(LED_RED);
							printf("Red Off\n\r");
						}
					}
					else if(!strcmp(led,"green"))
					{
						if(led_value)
						{
							printf("Green On\n\r");
							LED_ON(LED_GREEN);
						}
						else
						{
							LED_OFF(LED_GREEN);
							printf("Green Off\n\r");
						}
					}
					else if(!strcmp(led,"blue"))
					{
						if(led_value)
						{
							LED_ON(LED_BLUE);
							printf("Blue On\n\r");
						}
						else
						{
							LED_OFF(LED_BLUE);
							printf("Blue Off\n\r");
						}
					}
					else if(!strcmp(led,"yellow"))
					{
						if(led_value)
						{
							LED_ON(LED_YELLOW);
							printf("Yellow On\n\r");
						}
						else
						{
							LED_OFF(LED_YELLOW);
							printf("Yellow Off\n\r");
						}
					}
					cJSON_Delete(root); // 释放内存空间 
				}
				else
				{
					printf("Error before: [%s]\n\r",cJSON_GetErrorPtr());
				}
			}
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);//重新开启定时器
		}
		
		
	}
	printf("网络断开");
	printf("getSn_SR is %d",getSn_SR(SOCK_TCPS));
	return 0;
}
