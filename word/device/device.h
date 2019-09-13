#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "stm32f10x.h"
#include "types.h"
#define DEVICE_ID "W5500"

#define LED_RED GPIO_Pin_0
#define LED_GREEN GPIO_Pin_1
#define LED_YELLOW GPIO_Pin_2
#define LED_BLUE GPIO_Pin_3

#define FW_VER_HIGH   	1
#define FW_VER_LOW    	0

#define LED0  					GPIO_Pin_0	// out
#define LED1						GPIO_Pin_1	// out
#define LED2		    		GPIO_Pin_2	// 
#define LED3						GPIO_Pin_3	// 

#define WIZ_SCS			    GPIO_Pin_12	// out
#define WIZ_SCLK				GPIO_Pin_13	// out
#define WIZ_MISO				GPIO_Pin_14	// in
#define WIZ_MOSI				GPIO_Pin_15	// out

#define WIZ_RESET		    GPIO_Pin_9	// out
#define WIZ_INT			    GPIO_Pin_6	// in

/* Private function prototypes -----------------------------------------------*/

void GPIO_Configuration(void);
void NVIC_Configuration(void);
typedef  void (*pFunction)(void);
void set_network(void);
void write_config_to_eeprom(void);
void set_default(void);
void Reset_W5500(void);

void LED_ON(uint16 led);
void LED_OFF(uint16 led);

void TIM2_Configuration(void);
void TIM2_NVIC_Configuration(void);

void reboot(void);
void get_config(void);
#endif

