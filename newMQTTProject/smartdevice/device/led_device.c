#include "led_device.h"
#include "kal_led_device.h"


int LEDInit(void)
{
	return KAL_LEDInit();
}

int LEDDeviceControl(struct LEDDevice *ptLEDDevice, int iStatus)
{
	return KAL_LEDDeviceControl(ptLEDDevice, iStatus);
}


//一个数组，记录所有的led device的结构体
static LEDDevice g_tLEDDevices[] = {
	{LED_WHITE, LEDInit, LEDDeviceControl},
	{LED_BLUE,  LEDInit, LEDDeviceControl},
	{LED_GREEN, LEDInit, LEDDeviceControl},
};


PLEDDevice GetLEDDevice(int which)
{
	if(which<0 || which >2)
	{
		return NULL;
	}
	else
	{
		return &g_tLEDDevices[which];
	}
}

