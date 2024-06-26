#include "dht11_device.h"
#include "kal_dht11_device.h"
#include <stdio.h>


static int DHT11Init()
{
	return KAL_DHT11DeviceInit();
}
static int GetValue(struct DHT11Dev *ptDHT11Dev)
{
	return KAL_DHT11Device_GetValue(ptDHT11Dev);
}

//初始化DHT11结构体
DHT11Dev g_tDHT11Devices = {
	1, 
	{0}, 
	DHT11Init, 
	GetValue
};

//获取DHT11设备
PDHT11Dev GetDHT11Device(int which)
{
    if(which <= 1)
		{
			return &g_tDHT11Devices;
		}
    return NULL;
}
