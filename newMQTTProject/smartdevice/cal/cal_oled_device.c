#include "cal_oled_device.h"
#include "driver_oled.h"
#include "driver_i2c.h"

int CAL_OLEDDeviceInit()
{
	I2C_GPIO_ReInit();
	OLED_Init();
	return 0;
}

int CAL_OLEDDeviceFlush(struct DisplayDevice *ptDev)
{
	OLED_Copy(ptDev->FBBase);
	return 0;
}

