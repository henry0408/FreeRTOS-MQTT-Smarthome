#ifndef __CAL_LED_DEVICE_
#define __CAL_LED_DEVICE_

#include "led_device.h"
#include "driver_led.h"

int CAL_LEDInit(void);
int CAL_LEDDeviceControl(struct LEDDevice *ptLEDDevice, int iStatus);

#endif
