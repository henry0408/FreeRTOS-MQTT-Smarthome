#include "cal_led_device.h"


int CAL_LEDInit(void)
{
	//已经在gpio.c中初始化好了
	return 0;
}

int CAL_LEDDeviceControl(struct LEDDevice *ptLEDDevice, int iStatus)
{
	return HAL_LEDDeviceControl(ptLEDDevice, iStatus);
}

