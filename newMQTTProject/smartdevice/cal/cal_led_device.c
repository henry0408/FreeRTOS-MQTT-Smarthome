#include "cal_led_device.h"


int CAL_LEDInit(void)
{
	//�Ѿ���gpio.c�г�ʼ������
	return 0;
}

int CAL_LEDDeviceControl(struct LEDDevice *ptLEDDevice, int iStatus)
{
	return HAL_LEDDeviceControl(ptLEDDevice, iStatus);
}

