#include "gpio_input.h"
#include "input_buffer.h"
#include "stm32f1xx_hal.h"
#include "driver_key.h"

static unsigned int timers[2];

/**********************************************************************
 * 函数名称： GetGPIOKeyTimerVal
 * 功能描述： 获得GPIO按键的定时器的超时时间
 * 输入参数： index-哪个定时器
 * 输出参数： 无
 * 返 回 值： 定时器的超时时间

 ***********************************************************************/ 
unsigned int GetGPIOKeyTimerVal(int index)
{
	return timers[index];
}

/**********************************************************************
 * 函数名称： SetGPIOKeyTimer
 * 功能描述： 设置GPIO按键的定时器的超时时间
 * 输入参数： index-哪个定时器
 * 输入参数： val-超时时刻
 * 输出参数： 无
 * 返 回 值： 定时器的超时时间

 ***********************************************************************/ 
void SetGPIOKeyTimer(int index, unsigned int val)
{
	timers[index] = val;
}

/**********************************************************************
 * 函数名称： ProcessGPIOKeyTimer
 * 功能描述： 处理GPIO按键的定时器
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 定时器的超时时间
 ***********************************************************************/ 
void ProcessGPIOKeyTimer(void)
{
	int i;
	InputEvent event;

	for (i = 0; i < 2; i++)
	{
		/* 判断时间是否超时 */
		if ((timers[i] != 0) && (HAL_GetTick() >= timers[i]))
		{
			/* 如果超时: 上报按键 */
			/* 构造InputEvent */
			//event.time  = KAL_GetTime();  /* kal */
			event.type = INPUT_EVENT_KEY;
			event.iKey  = (i == 0) ? K1_CODE : K2_CODE;

			/* 读取GPIO电平, GPIO为0表示按下, 此时压力是1,松开后压力是0 */
			event.iPressure = (i == 0) ? !K1_STATUS : !K2_STATUS;

			/* 放入环形缓冲区 */
			PutInputEvent(&event);

			/* 禁止定时器 */
			timers[i] = 0;
		}
	}
}


//typedef struct InputDevice
//{
//	char name[10];														//设备名称
//	int (*Init)();														//初始化设备
//	int (*GetInputEvent)(pInputEvent event);	//获取输入事件	
//	struct InputDevice* pNext;								//指针指向链表下一个节点
//	
//}InputDevice, *pInputDevice;


int GPIOKeyInit()
{
	KEY_GPIO_ReInit();
	return 0;
}

/* 初始化按键输入设备 */
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
