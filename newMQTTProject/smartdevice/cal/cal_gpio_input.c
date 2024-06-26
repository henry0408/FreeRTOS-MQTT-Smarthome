#include "cal_gpio_input.h"

#include "driver_key.h"

int CAL_GPIOKeyInit(void)
{
	KEY_GPIO_ReInit();
	return 0;
}


