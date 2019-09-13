#ifndef __MQTT_H
#define __MQTT_H



int do_mqtt_publish(char *pTopic,char *pMessage);
int do_mqtt_subscrib(char *pTopic,char *pMessage);

#endif
