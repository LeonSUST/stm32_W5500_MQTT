#include "usart.h"
#include "device.h"
#include "spi2.h"
#include "ult.h"
#include "socket.h"
#include "w5500.h"
#include "24c16.h"
#include "md5.h"
#include "mqtt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern uint8 txsize[];										// �����ⲿ����������Socket���ͻ����С
extern uint8 rxsize[];										// �����ⲿ����������Socket���ջ����С

uint8_t domain_ip[4]={192,168,1,101};//115,29,225,215
uint8_t domain_name[]="www.embed-net.com";

uint8 buffer[2048];// ����һ��2KB�����飬�������Socket��ͨ������

int main(void)
{
	static char meassage[200];
	int rc=0;
  /***** MCUʱ�ӳ�ʼ�� *****/							  
	Systick_Init(72);
	/***** �жϿ��������� *****/
	NVIC_Configuration();
	/*****TIM2 ��ʱ����****/
	TIM2_Configuration();
	/* ʵս��ʱ�����ж����ȼ� */
	TIM2_NVIC_Configuration();
	/***** GPIO��SPI��ʼ�� *****/
	GPIO_Configuration();			
	WIZ_SPI_Init();
	/***** ���ڳ�ʼ�� *****/
	USART1_Init();
	/***** ��ʼ��eeprom *****/
  at24c16_init();
	/***** Ӳ����W5500 *****/
	Reset_W5500();
	/***** W5500��IP��Ϣ��ʼ�� *****/
	set_default();// ����Ĭ��MAC��IP��GW��SUB��DNS
	set_network();// ���ó�ʼ��IP��Ϣ����ӡ����ʼ��8��Socket
	
	/***** ��W5500��Keepalive���� *****/
	//setkeepalive(0);
	
	printf("W5500 Init Complete!\r\n");
  printf("Start TCP Client Test!\r\n");

  srand(0);	
 
	while(1)
	{
		if(rc==0)
		{
			memset(meassage,0,sizeof(meassage));
		  do_mqtt_subscrib("wizled",meassage);//��������pyboard_led
			rc++;
			printf("%d\n\r",rc);
	  }
  }
}


