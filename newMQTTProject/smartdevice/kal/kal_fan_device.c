#include "kal_fan_device.h"
#include "cal_fan_device.h"


int KAL_FANInit(void)
{
	return CAL_FANInit();
}

int KAL_FanSetSpeed(int iSpeed)
{
	return CAL_FanSetSpeed(iSpeed);
}

