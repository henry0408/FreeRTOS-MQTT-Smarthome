#include "oled_device.h"
#include "display_device.h"
#include <kal_oled_device.h>
#include <config.h>
#include <driver_oled.h>
#include <driver_i2c.h>


static unsigned char gOLEDFramebuffer[1024];		//�����СΪ128*64 bits/8 = 1024 byte

static int OLEDDeviceInit(struct DisplayDevice *ptDev)   /* Ӳ����ʼ�� */
{
	I2C_GPIO_ReInit();
	OLED_Init();
	return 0;;
	
}
static void OLEDDeviceFlash(struct DisplayDevice *ptDev) /* ��FBBase������ˢ��LCD���Դ��� */
{
	OLED_Copy(ptDev->FBBase);
}

static int OLEDDeviceSetPixel(struct DisplayDevice *ptDev, int iX, int iY, unsigned int dwColor)
{
	unsigned char* buf = ptDev->FBBase;
	if(iX >= ptDev->iXres || iY>= ptDev->iYres)
	{
		return -1;
	}
	//�ҵ���������
	int page = iY/8;
	unsigned char* byte = buf + page*128 + iX;	//buf[page*128 + iX]
	int bit = iY % 8;
	
	if(dwColor)
	{
		*byte |= (1<<bit);
	}
	else
	{
		*byte &= ~(1<<bit);
	}
	return 0;
}


//ΪOLED����һ��DisplayDevice�ṹ��
static DisplayDevice g_tOLEDDevice= {
	"OLED",
	gOLEDFramebuffer,
	128,
	64,
	1,
	OLEDDeviceInit,
	OLEDDeviceFlash,
	OLEDDeviceSetPixel
};


void AddDisplayDeviceOLED(void)
{
	DisplayDeviceRegister(&g_tOLEDDevice);
}


