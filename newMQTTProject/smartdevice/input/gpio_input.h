#ifndef __GPIO_INPUT_
#define __GPIO_INPUT_

#include "input_system.h"
#include "stdio.h"

//���尴�������֣�InputEvent�е�iKey��
#define K1_CODE 1
#define K2_CODE 2

//int GPIOKeyInit(void);

void SetGPIOKeyTimer(int index, unsigned int val);
void AddInputDeviceGPIOKey(void);

#endif
