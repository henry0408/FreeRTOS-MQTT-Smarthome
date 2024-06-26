//#include <kal_time.h>
#include <fan_device.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"

void fan_test(void)
{
	pFANDevice f1 = GetFANDevice();
	f1->Init(f1);
	while (1)
	{
		f1->SetSpeed(1);	//正转

		HAL_Delay(2000);

		f1->SetSpeed(-1);	//反转
		
		HAL_Delay(2000);
		
		f1->SetSpeed(0);	//停止
		
		HAL_Delay(2000);
		
	}
}

