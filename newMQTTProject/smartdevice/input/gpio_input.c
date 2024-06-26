#include "gpio_input.h"
#include "input_buffer.h"
#include "stm32f1xx_hal.h"
#include "driver_key.h"

static unsigned int timers[2];

/**********************************************************************
 * �������ƣ� GetGPIOKeyTimerVal
 * ���������� ���GPIO�����Ķ�ʱ���ĳ�ʱʱ��
 * ��������� index-�ĸ���ʱ��
 * ��������� ��
 * �� �� ֵ�� ��ʱ���ĳ�ʱʱ��

 ***********************************************************************/ 
unsigned int GetGPIOKeyTimerVal(int index)
{
	return timers[index];
}

/**********************************************************************
 * �������ƣ� SetGPIOKeyTimer
 * ���������� ����GPIO�����Ķ�ʱ���ĳ�ʱʱ��
 * ��������� index-�ĸ���ʱ��
 * ��������� val-��ʱʱ��
 * ��������� ��
 * �� �� ֵ�� ��ʱ���ĳ�ʱʱ��

 ***********************************************************************/ 
void SetGPIOKeyTimer(int index, unsigned int val)
{
	timers[index] = val;
}

/**********************************************************************
 * �������ƣ� ProcessGPIOKeyTimer
 * ���������� ����GPIO�����Ķ�ʱ��
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��ʱ���ĳ�ʱʱ��
 ***********************************************************************/ 
void ProcessGPIOKeyTimer(void)
{
	int i;
	InputEvent event;

	for (i = 0; i < 2; i++)
	{
		/* �ж�ʱ���Ƿ�ʱ */
		if ((timers[i] != 0) && (HAL_GetTick() >= timers[i]))
		{
			/* �����ʱ: �ϱ����� */
			/* ����InputEvent */
			//event.time  = KAL_GetTime();  /* kal */
			event.type = INPUT_EVENT_KEY;
			event.iKey  = (i == 0) ? K1_CODE : K2_CODE;

			/* ��ȡGPIO��ƽ, GPIOΪ0��ʾ����, ��ʱѹ����1,�ɿ���ѹ����0 */
			event.iPressure = (i == 0) ? !K1_STATUS : !K2_STATUS;

			/* ���뻷�λ����� */
			PutInputEvent(&event);

			/* ��ֹ��ʱ�� */
			timers[i] = 0;
		}
	}
}


//typedef struct InputDevice
//{
//	char name[10];														//�豸����
//	int (*Init)();														//��ʼ���豸
//	int (*GetInputEvent)(pInputEvent event);	//��ȡ�����¼�	
//	struct InputDevice* pNext;								//ָ��ָ��������һ���ڵ�
//	
//}InputDevice, *pInputDevice;


int GPIOKeyInit()
{
	KEY_GPIO_ReInit();
	return 0;
}

/* ��ʼ�����������豸 */
static InputDevice g_tKeyDevice = 
{
	"gpio_key",
	GPIOKeyInit,
	NULL
};

void AddInputDeviceGPIOKey()
{
	InputDeviceRegister(&g_tKeyDevice);
}
