#ifndef __FAN_DEVICE_
#define __FAN_DEVICE_
#include <stdio.h>

typedef struct FANDevice{
	int (*Init)();
	int (*SetSpeed)(int iSpeed);
	
}FANDevice, *pFANDevice;

int FANInit(void);
int FanSetSpeed(int iSpeed);

pFANDevice GetFANDevice(void);

#endif

