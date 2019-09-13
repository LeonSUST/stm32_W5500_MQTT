#ifndef __DHT11_H
#define __DHT11_H

#include "ult.h"
#include "stm32f10x.h"
#include "config.h"

//-----------------DHT11Òý½Å£¬PA12---------------------
#define	PA12            GPIO_Pin_12
#define DHT11         (1<<1)
#define DHT11_SET(x)  GPIOA->ODR=(GPIOA->ODR&~DHT11)|(x ? DHT11:0)

#define rDHT11        GPIO_ReadInputDataBit(GPIOA ,GPIO_Pin_12)
//----------------------------------------------------
uint8 DHT11_ReadData(void);
uint8 DHT11_GetValue(uint8 *p);

#endif
