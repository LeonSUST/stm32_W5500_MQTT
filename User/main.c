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

extern uint8 txsize[];										// 引用外部变量，声明Socket发送缓存大小
extern uint8 rxsize[];										// 引用外部变量，声明Socket接收缓存大小

uint8_t domain_ip[4]={192,168,1,101};//115,29,225,215
uint8_t domain_name[]="www.embed-net.com";

uint8 buffer[2048];// 定义一个2KB的数组，用来存放Socket的通信数据

int main(void)
{
	static char meassage[200];
	int rc=0;
  /***** MCU时钟初始化 *****/							  
	Systick_Init(72);
	/***** 中断控制器配置 *****/
	NVIC_Configuration();
	/*****TIM2 定时配置****/
	TIM2_Configuration();
	/* 实战定时器的中断优先级 */
	TIM2_NVIC_Configuration();
	/***** GPIO、SPI初始化 *****/
	GPIO_Configuration();			
	WIZ_SPI_Init();
	/***** 串口初始化 *****/
	USART1_Init();
	/***** 初始化eeprom *****/
  at24c16_init();
	/***** 硬重启W5500 *****/
	Reset_W5500();
	/***** W5500的IP信息初始化 *****/
	set_default();// 设置默认MAC、IP、GW、SUB、DNS
	set_network();// 配置初始化IP信息并打印，初始化8个Socket
	
	/***** 打开W5500的Keepalive功能 *****/
	//setkeepalive(0);
	
	printf("W5500 Init Complete!\r\n");
  printf("Start TCP Client Test!\r\n");

  srand(0);	
 
	while(1)
	{
		if(rc==0)
		{
			memset(meassage,0,sizeof(meassage));
		  do_mqtt_subscrib("wizled",meassage);//订阅主题pyboard_led
			rc++;
			printf("%d\n\r",rc);
	  }
  }
}


