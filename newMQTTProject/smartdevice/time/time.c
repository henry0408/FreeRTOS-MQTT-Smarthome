#include "time.h"
#include "stm32f1xx_hal.h"

int GetTime(void)
{
	return HAL_GetTick();
}

