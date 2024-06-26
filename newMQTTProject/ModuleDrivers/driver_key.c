//#include "driver_key.h"
//#include "main.h"
//#include <input_system.h>
//#include <kal_time.h>
//#include "gpio_key.h"
//#include "input_buffer.h"
#include <input_system.h>
//#include <kal_time.h>
#include <gpio_input.h>
#include <input_buffer.h>
#include "driver_key.h"

static volatile uint8_t key1_val = KEY_RELEASED;     // 按键KEY1的键值，按下为0松开为1
static volatile uint8_t key2_val = KEY_RELEASED;     // 按键KEY2的键值，按下为0松开为1

//重新初始化按键的GPIO
void KEY_GPIO_ReInit(void)
{
	//使能时钟
	KEY1_GPIO_CLK_EN();
	KEY2_GPIO_CLK_EN();
	
	//重写结构体
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;     // 双边沿触发中断
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	
	GPIO_InitStruct.Pin = KEY1_PIN;
	HAL_GPIO_Init(KEY1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = KEY2_PIN;
	HAL_GPIO_Init(KEY1_PORT, &GPIO_InitStruct);
	
	//设置中断优先级
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 2);
	
	//使能中断
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

//外部中断线10~15的中断服务函数
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(KEY1_PIN); // 使用HAL库的外部中断统一服务函数，参数是外部中断线
	HAL_GPIO_EXTI_IRQHandler(KEY2_PIN);
}

//外部中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	InputEvent event;
//	if(KEY1_PIN == GPIO_Pin)
//	{
//		/* 构造InputEvent */
//		event.type = INPUT_EVENT_KEY;
//		event.iKey = K1_CODE;
//		event.iPressure = !K1_STATUS; 

//		/* 放入环形缓冲区 */
//		PutInputEvent(&event);
//	}
//	else if(KEY2_PIN == GPIO_Pin)
//	{
//		
//		/* 构造InputEvent */
//		event.type = INPUT_EVENT_KEY;
//		event.iKey = K2_CODE;
//		event.iPressure = !K2_STATUS; 

//		/* 放入环形缓冲区 */
//		PutInputEvent(&event);
//	}
	
	
	/* 消除抖动 */
	if(KEY1_PIN == GPIO_Pin)
	{
		/* 启动定时器, 修改超时时间 */
		SetGPIOKeyTimer(0, HAL_GetTick()+20);
	}
	else if(KEY2_PIN == GPIO_Pin)
	{
		/* 启动定时器, 修改超时时间 */
		SetGPIOKeyTimer(1, HAL_GetTick()+20);
	}
}

//获取键值
uint8_t KEY1_Value(void)
{
	return key1_val;
}

uint8_t KEY2_Value(void)
{
	return key2_val;
}


