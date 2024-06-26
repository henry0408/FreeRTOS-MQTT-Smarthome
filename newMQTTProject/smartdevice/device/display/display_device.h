#ifndef __DISPLAY_DEVICE_H
#define __DISPLAY_DEVICE_H


//创建显示设备结构体

typedef struct DisplayDevice
{
	char *name;
	void *FBBase; /* CPU能直接读写的显存的基地址 */
	int iXres;    /* X方向分辨率 */
	int iYres;    /* Y方向分辨率 */
	int iBpp;     /* 每个像素使用多少个Bit(Bit per pixel)	*/
	int (*Init)(struct DisplayDevice *ptDev);   /* 硬件初始化 */
	void (*Flush)(struct DisplayDevice *ptDev); /* 把FBBase的数据刷到LCD的显存里 */
	int (*SetPixel)(struct DisplayDevice *ptDev, int iX, int iY, unsigned int dwColor); /* 设置某个像素，dwColor=0xRRGGBB */
	
	struct DisplayDevice *pNext;	//链表指针指向下一个设备节点
	int iSize;		//显存大小（bytes）
}DisplayDevice, *PDisplayDevice;

//获得哪个设备
PDisplayDevice GetDisplayDevice(char *name);

//注册一个设备
void DisplayDeviceRegister(PDisplayDevice ptDisplayDevice);

//显示一个设备（遍历链表，找到名字相同的设备）
PDisplayDevice __GetDisplayDevice(char *name);

#endif
