
#include "stm32f10x_it.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mqtt.h"
#include "MQTTPacket.h"
#include "transport.h"
#include "dht11.h"
#include "socket.h"
#include "w5500.h"

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1)
  {
  }
}


void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}


void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

void TIM2_IRQHandler(void)
{
	unsigned char buf[20];
	int buflen = sizeof(buf);
	float temperature,humidity;
	uint8 temp_rh[2];//temp_rh[0]是湿度 temp_rh[1]是温度
	static char meassage[100];
	int j = 0;
	int i = 0;
	static int timePUB = 0;
	static int timeSUB = 0;
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		timePUB++;
		timeSUB++;
		if(timePUB ==30000)
		{
			DHT11_GetValue(temp_rh);//读取温湿度数据
			temperature = temp_rh[1];
			printf("temperature is %f",temperature);
			humidity = temp_rh[0];
			printf("humidity is %f\r\n",humidity);
			sprintf(meassage,"{\"temperature\":%.1f,\"humidity\":%.1f}",temperature,humidity);
			do_mqtt_publish("wiz_publish",meassage);//发布的主题pyboard_value
			timePUB = 0;
		}
		if(timeSUB ==60000)
		{
			{
			  printf("发送PINGREQ报文\r\n");
			  timeSUB = 0;
			  i=MQTTSerialize_pingreq(buf,buflen);
			  j=transport_sendPacketBuffer(buf, i);
			  if(j!=i)
			  {
				  printf("PING connect transport_sendPacketBuffer error\n\r");
			  }
		  }
		}
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  		 
	}		 	
}


void DebugMon_Handler(void)
{
}


void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}
/*
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		ISR();//中断处理

		EXTI_ClearITPendingBit(EXTI_Line6);       
	}                  
}
*/

