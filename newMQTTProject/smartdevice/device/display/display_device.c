#include "display_device.h"
#include <stdio.h>
#include <string.h>

static PDisplayDevice g_ptDisplayDevices;

//注册一个设备（添加到设备链表）
void DisplayDeviceRegister(PDisplayDevice ptDisplayDevice)
{
	ptDisplayDevice->pNext = g_ptDisplayDevices;
	g_ptDisplayDevices = ptDisplayDevice;
}

//显示一个设备（遍历链表，找到名字相同的设备）
PDisplayDevice __GetDisplayDevice(char *name)
{
	PDisplayDevice pTmp = g_ptDisplayDevices;
	while (pTmp)
	{
		if (strcmp(pTmp->name, name) == 0)
			return pTmp;
		else
			pTmp = pTmp->pNext;
	}
	return NULL;
}

