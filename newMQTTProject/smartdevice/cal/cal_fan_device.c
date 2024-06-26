#include "cal_fan_device.h"

int CAL_FANInit(void)
{
	//GPIO已经初始化完毕了
	return 0;
}

int CAL_FanSetSpeed(int iSpeed)
{
	return HAL_FanSetSpeed(iSpeed);
}
