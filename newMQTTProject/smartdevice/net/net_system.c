#include "net_system.h"
#include <string.h>
#include "esp8266.h"


//�����豸����
pNetDevice g_ptNetDevice;

//ע��ĳ������
void NetDeviceRegister(pNetDevice ptNetDevice)
{
	//ͷ�巨
	ptNetDevice->pNext = g_ptNetDevice;
	g_ptNetDevice = ptNetDevice;
}

//ע����������
void AddNetDevices()
{
	AddESP8266NetDevices();
}

//��ȡĳ������
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

