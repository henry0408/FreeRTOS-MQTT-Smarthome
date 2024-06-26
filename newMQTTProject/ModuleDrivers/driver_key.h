#ifndef __DRIVER_KEY_H
#define __DRIVER_KEY_H

#include "stm32f1xx_hal.h"

/*
    按键状态宏定义
*/
#define KEY_PRESSED		0
#define KEY_RELEASED	1

/*********************
 * 引脚宏定义
**********************/
#define KEY1_PIN				GPIO_PIN_14
#define KEY2_PIN				GPIO_PIN_15

#define KEY1_PORT				GPIOE
#define KEY2_PORT				GPIOE

/*********************
 * 函数宏定义
**********************/
#define KEY1_GPIO_CLK_EN()	__HAL_RCC_GPIOE_CLK_ENABLE()
#define KEY2_GPIO_CLK_EN()	__HAL_RCC_GPIOE_CLK_ENABLE()

#define K1_STATUS							HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN)
#define K2_STATUS							HAL_GPIO_ReadPin(KEY2_PORT, KEY2_PIN)

void KEY_GPIO_ReInit(void);

uint8_t KEY1_Value(void);
	
uint8_t KEY2_Value(void);


#endif
