#ifndef _ONENET_H_
#define _ONENET_H_

#include "stdint.h"



_Bool OneNET_RegisterDevice(void);

_Bool OneNet_DevLink(void);

//void OneNet_SendData(void);
void OneNet_SendData(uint8_t temp, uint8_t humi);

void OneNET_Subscribe(void);

void OneNet_RevPro(unsigned char *cmd);

#endif
