#ifndef __TRANSPORT_H
#define __TRANSPORT_H

#define SOCK_TCPS         0
#define SOCK_TCPC         2

int transport_sendPacketBuffer(unsigned char* buf, int buflen);//通过网络以TCP的形式发送数据
int transport_sendPacketBuffer1(unsigned char* buf, int buflen);
int transport_getdata(unsigned char* buf, int count);//TCP方式从服务器端读取数据
int transport_getdata1(unsigned char* buf, int count);
int transport_open(void);//打开一个网络端口并建立链接
int transport_open1(void);
int transport_close(void);//关闭网络接口
int transport_close1(void);

#endif
