#include "display_device.h"
#include <stdio.h>
#include <string.h>

static PDisplayDevice g_ptDisplayDevices;

//ע��һ���豸����ӵ��豸����
void DisplayDeviceRegister(PDisplayDevice ptDisplayDevice)
{
	ptDisplayDevice->pNext = g_ptDisplayDevices;
	g_ptDisplayDevices = ptDisplayDevice;
}

//��ʾһ���豸�����������ҵ�������ͬ���豸��
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

