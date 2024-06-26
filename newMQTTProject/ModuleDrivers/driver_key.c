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

static volatile uint8_t key1_val = KEY_RELEASED;     // ����KEY1�ļ�ֵ������Ϊ0�ɿ�Ϊ1
static volatile uint8_t key2_val = KEY_RELEASED;     // ����KEY2�ļ�ֵ������Ϊ0�ɿ�Ϊ1

//���³�ʼ��������GPIO
void KEY_GPIO_ReInit(void)
{
	//ʹ��ʱ��
	KEY1_GPIO_CLK_EN();
	KEY2_GPIO_CLK_EN();
	
	//��д�ṹ��
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;     // ˫���ش����ж�
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	
	GPIO_InitStruct.Pin = KEY1_PIN;
	HAL_GPIO_Init(KEY1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = KEY2_PIN;
	HAL_GPIO_Init(KEY1_PORT, &GPIO_InitStruct);
	
	//�����ж����ȼ�
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 2);
	
	//ʹ���ж�
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

//�ⲿ�ж���10~15���жϷ�����
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(KEY1_PIN); // ʹ��HAL����ⲿ�ж�ͳһ���������������ⲿ�ж���
	HAL_GPIO_EXTI_IRQHandler(KEY2_PIN);
}

//�ⲿ�жϻص�����
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	InputEvent event;
//	if(KEY1_PIN == GPIO_Pin)
//	{
//		/* ����InputEvent */
//		event.type = INPUT_EVENT_KEY;
//		event.iKey = K1_CODE;
//		event.iPressure = !K1_STATUS; 

//		/* ���뻷�λ����� */
//		PutInputEvent(&event);
//	}
//	else if(KEY2_PIN == GPIO_Pin)
//	{
//		
//		/* ����InputEvent */
//		event.type = INPUT_EVENT_KEY;
//		event.iKey = K2_CODE;
//		event.iPressure = !K2_STATUS; 

//		/* ���뻷�λ����� */
//		PutInputEvent(&event);
//	}
	
	
	/* �������� */
	if(KEY1_PIN == GPIO_Pin)
	{
		/* ������ʱ��, �޸ĳ�ʱʱ�� */
		SetGPIOKeyTimer(0, HAL_GetTick()+20);
	}
	else if(KEY2_PIN == GPIO_Pin)
	{
		/* ������ʱ��, �޸ĳ�ʱʱ�� */
		SetGPIOKeyTimer(1, HAL_GetTick()+20);
	}
}

//��ȡ��ֵ
uint8_t KEY1_Value(void)
{
	return key1_val;
}

uint8_t KEY2_Value(void)
{
	return key2_val;
}


