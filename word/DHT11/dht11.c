#include "dht11.h"
#include <stdio.h>

uint8 DHT11_ReadData(void)
{       
    uint8 i,temp=0; 
    for(i=0;i<8;i++)     
    { 
      while(!rDHT11);//ÿһbit���ݶ���50us�͵�ƽʱ϶��ʼ
      Delay_us(33);//��ȷ��ʱ33us
      if(rDHT11)//����λΪ1
      {
        temp=temp<<1 | 0x01;
        while(rDHT11);
      }
      else//����λΪ0
	temp<<=1;
    }
    return temp;
} 
 
uint8 DHT11_GetValue(uint8 *p)
{ 
    uint8 temp,temp1,temp2;
    //���߿���״̬Ϊ�ߵ�ƽ,�������������͵ȴ�DHT11��Ӧ,�������������ͱ������18����,��֤DHT11�ܼ�⵽��ʼ�źš�
    GPIO_ResetBits(GPIOA,PA12);
    Delay_ms(20);//��ȷ��ʱ20ms
    GPIO_SetBits(GPIOA,PA12);
    //�������Ϳ�ʼ�źŽ�����,��ʱ�ȴ�20-40us��, ��ȡDHT11����Ӧ�źš�
    Delay_us(40);//��ȷ��ʱ40us
    //����Ϊ�͵�ƽ,˵��DHT11������Ӧ�ź�
    if(!rDHT11)
    {  
      //�жϴӻ������ĵ͵�ƽ��Ӧ�ź��Ƿ����   
      while(!rDHT11); 
      //DHT11������Ӧ�źź�,�ٰ���������80us
      while(rDHT11); 
      p[0]=DHT11_ReadData();//8bitʪ����������
      temp1=DHT11_ReadData();//������Ч����
      p[1]=DHT11_ReadData();//8bit�¶���������
      temp2=DHT11_ReadData();//������Ч����
      temp=DHT11_ReadData();//8bitУ���
      GPIO_SetBits(GPIOA,PA12);     
      if(temp==(p[0]+p[1]+temp1+temp2))	
      {
	return 1;
      }
    }
    return 0;
}

