#ifndef __TRANSPORT_H
#define __TRANSPORT_H

#define SOCK_TCPS         0
#define SOCK_TCPC         2

int transport_sendPacketBuffer(unsigned char* buf, int buflen);//ͨ��������TCP����ʽ��������
int transport_sendPacketBuffer1(unsigned char* buf, int buflen);
int transport_getdata(unsigned char* buf, int count);//TCP��ʽ�ӷ������˶�ȡ����
int transport_getdata1(unsigned char* buf, int count);
int transport_open(void);//��һ������˿ڲ���������
int transport_open1(void);
int transport_close(void);//�ر�����ӿ�
int transport_close1(void);

#endif
