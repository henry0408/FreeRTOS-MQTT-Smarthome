#include "kal_led_device.h"
#include "cal_led_device.h"

int KAL_LEDInit(void)
{
	return CAL_LEDInit();
}

int KAL_LEDDeviceControl(struct LEDDevice *ptLEDDevice, int iStatus)
{
	return CAL_LEDDeviceControl(ptLEDDevice,iStatus);
}

