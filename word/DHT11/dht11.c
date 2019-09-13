#include "dht11.h"
#include <stdio.h>

uint8 DHT11_ReadData(void)
{       
    uint8 i,temp=0; 
    for(i=0;i<8;i++)     
    { 
      while(!rDHT11);//每一bit数据都以50us低电平时隙开始
      Delay_us(33);//精确延时33us
      if(rDHT11)//数据位为1
      {
        temp=temp<<1 | 0x01;
        while(rDHT11);
      }
      else//数据位为0
	temp<<=1;
    }
    return temp;
} 
 
uint8 DHT11_GetValue(uint8 *p)
{ 
    uint8 temp,temp1,temp2;
    //总线空闲状态为高电平,主机把总线拉低等待DHT11响应,主机把总线拉低必须大于18毫秒,保证DHT11能检测到起始信号。
    GPIO_ResetBits(GPIOA,PA12);
    Delay_ms(20);//精确延时20ms
    GPIO_SetBits(GPIOA,PA12);
    //主机发送开始信号结束后,延时等待20-40us后, 读取DHT11的响应信号。
    Delay_us(40);//精确延时40us
    //总线为低电平,说明DHT11发送响应信号
    if(!rDHT11)
    {  
      //判断从机发出的低电平响应信号是否结束   
      while(!rDHT11); 
      //DHT11发送响应信号后,再把总线拉高80us
      while(rDHT11); 
      p[0]=DHT11_ReadData();//8bit湿度整数数据
      temp1=DHT11_ReadData();//忽略无效数据
      p[1]=DHT11_ReadData();//8bit温度整数数据
      temp2=DHT11_ReadData();//忽略无效数据
      temp=DHT11_ReadData();//8bit校验和
      GPIO_SetBits(GPIOA,PA12);     
      if(temp==(p[0]+p[1]+temp1+temp2))	
      {
	return 1;
      }
    }
    return 0;
}

