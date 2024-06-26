#include "fan_device.h"
#include "kal_fan_device.h"

int FANInit(void)
{
	return KAL_FANInit();
}
int FanSetSpeed(int iSpeed)
{
	return KAL_FanSetSpeed(iSpeed);
}

struct FANDevice g_tFanDevice = {
	FANInit,
	FanSetSpeed
};

pFANDevice GetFANDevice(void)
{
	return &g_tFanDevice;
}

