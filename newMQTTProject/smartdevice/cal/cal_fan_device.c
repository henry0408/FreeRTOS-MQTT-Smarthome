#include "cal_fan_device.h"

int CAL_FANInit(void)
{
	//GPIO�Ѿ���ʼ�������
	return 0;
}

int CAL_FanSetSpeed(int iSpeed)
{
	return HAL_FanSetSpeed(iSpeed);
}
