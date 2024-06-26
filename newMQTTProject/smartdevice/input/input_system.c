#include "input_system.h"
#include "gpio_input.h"
#include "net_input.h"

/* �������豸�������г�ʼ�� */

static pInputDevice g_ptInputDevices;	//�����豸����

/*ע��ĳ�������豸*/
void InputDeviceRegister(pInputDevice ptInputDevice)
{
	ptInputDevice->pNext = g_ptInputDevices;	//ͷ�巨
	g_ptInputDevices = ptInputDevice;
}


/* ��ʼ�����������豸 */
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

//����豸���豸����
void AddInputDevices(void)
{
	AddInputDeviceGPIOKey();
	AddInputDeviceNet();
}


