#ifndef __KAL_LED_DEVICE_
#define __KAL_LED_DEVICE_

#include "led_device.h"

int KAL_LEDInit(void);


//extern struct LEDDevice *ptLEDDevice;
int KAL_LEDDeviceControl(struct LEDDevice *ptLEDDevice, int iStatus);

#endif
