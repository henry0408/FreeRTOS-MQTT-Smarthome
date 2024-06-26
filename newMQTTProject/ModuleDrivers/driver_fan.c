
#include "fan_device.h"

#include "driver_fan.h"
#include "main.h"

int HAL_FanSetSpeed(int iSpeed)
{

	if (iSpeed == 0)
	{
		/* INA,INB都输出0 */
		HAL_GPIO_WritePin(INA_GPIO_Port, INA_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(INB_GPIO_Port, INB_Pin, GPIO_PIN_RESET);
	}
	//正转
	else if (iSpeed > 0)
	{
		/* INA输出0,INB输出1 */
		HAL_GPIO_WritePin(INA_GPIO_Port, INA_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(INB_GPIO_Port, INB_Pin, GPIO_PIN_SET);
	}
	
	//反转
	else if (iSpeed < 0)
	{
		/* INA输出1,INB输出0 */
		HAL_GPIO_WritePin(INA_GPIO_Port, INA_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(INB_GPIO_Port, INB_Pin, GPIO_PIN_RESET);
	}
	return 0;	
}

