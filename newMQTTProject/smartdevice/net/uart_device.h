#ifndef __UART_DEV_H
#define __UART_DEV_H

#include "ring_buffer.h"

typedef struct UART_Device
{
	char* name;
	int (*Init)();
	int (*Write)(char* buffer, int len);
	int (*ReadByte)(char* buffer);
	void (*ClearBuffer)();
}UART_Device, *pUART_Device;


pUART_Device GetATInterfaceDevice(void);


ring_buffer* GetUART3RingBuffer(void);


#endif

