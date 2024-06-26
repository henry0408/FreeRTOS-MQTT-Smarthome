#include "net_system.h"
#include <string.h>
#include "esp8266.h"


//网卡设备链表
pNetDevice g_ptNetDevice;

//注册某个网卡
void NetDeviceRegister(pNetDevice ptNetDevice)
{
	//头插法
	ptNetDevice->pNext = g_ptNetDevice;
	g_ptNetDevice = ptNetDevice;
}

//注册所有网卡
void AddNetDevices()
{
	AddESP8266NetDevices();
}

//获取某个网卡
pNetDevice GetNetDevice(char *name)
{
	pNetDevice tmp = g_ptNetDevice;
	while(tmp)
	{
		if( strcmp(tmp->name, name) == 0 )
		{
			return tmp;
		}
		tmp = tmp->pNext;
	}
	return NULL;
}

