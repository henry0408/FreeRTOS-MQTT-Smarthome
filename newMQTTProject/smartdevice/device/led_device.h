#ifndef __LED_DEVICE_
#define __LED_DEVICE_
#include <stdio.h>


#define LED_WHITE 	0
#define LED_BLUE 	1
#define LED_GREEN 	2

typedef struct LEDDevice {
	int which;

	/* 初始化LED设备, 成功则返回0 */
	int (*Init)();

	/* 控制LED设备, iStatus取值: 1-亮,0-灭 */
	int (*Control)(struct LEDDevice *ptLEDDevice, int iStatus);

	/* 未实现 */
	void (*SetColor)(struct LEDDevice *ptLEDDevice, int iColor);

	/* 未实现 */
	void (*SetBrightness)(struct LEDDevice *ptLEDDevice, int iBrightness);
}LEDDevice, *PLEDDevice;


int LEDInit(void);

int LEDDeviceControl(struct LEDDevice *ptLEDDevice, int iStatus);

PLEDDevice GetLEDDevice(int which);



#endif

