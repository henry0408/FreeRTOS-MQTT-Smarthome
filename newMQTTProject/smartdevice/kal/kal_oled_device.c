#include "kal_oled_device.h"
#include "cal_oled_device.h"

int KAL_OLEDDeviceInit()
{
	return CAL_OLEDDeviceInit();
}

int KAL_OLEDDeviceFlush(struct DisplayDevice *ptDev)
{
	return CAL_OLEDDeviceFlush(ptDev);
}

