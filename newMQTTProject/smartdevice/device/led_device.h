#ifndef __LED_DEVICE_
#define __LED_DEVICE_
#include <stdio.h>


#define LED_WHITE 	0
#define LED_BLUE 	1
#define LED_GREEN 	2

typedef struct LEDDevice {
	int which;

	/* ��ʼ��LED�豸, �ɹ��򷵻�0 */
	int (*Init)();

	/* ����LED�豸, iStatusȡֵ: 1-��,0-�� */
	int (*Control)(struct LEDDevice *ptLEDDevice, int iStatus);

	/* δʵ�� */
	void (*SetColor)(struct LEDDevice *ptLEDDevice, int iColor);

	/* δʵ�� */
	void (*SetBrightness)(struct LEDDevice *ptLEDDevice, int iBrightness);
}LEDDevice, *PLEDDevice;


int LEDInit(void);

int LEDDeviceControl(struct LEDDevice *ptLEDDevice, int iStatus);

PLEDDevice GetLEDDevice(int which);



#endif

