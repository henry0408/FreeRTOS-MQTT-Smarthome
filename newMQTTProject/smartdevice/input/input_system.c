#include "input_system.h"
#include "gpio_input.h"
#include "net_input.h"

/* 对输入设备函数进行初始化 */

static pInputDevice g_ptInputDevices;	//输入设备链表

/*注册某个输入设备*/
void InputDeviceRegister(pInputDevice ptInputDevice)
{
	ptInputDevice->pNext = g_ptInputDevices;	//头插法
	g_ptInputDevices = ptInputDevice;
}


/* 初始化所有输入设备 */
int InitInputDevices (void)
{
	pInputDevice pDev = g_ptInputDevices;
	while(pDev)
	{
		pDev->Init();
		pDev = pDev->pNext;
	}
	return 0;
}

//添加设备到设备链表
void AddInputDevices(void)
{
	AddInputDeviceGPIOKey();
	AddInputDeviceNet();
}


