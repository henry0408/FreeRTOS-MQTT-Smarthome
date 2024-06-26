#ifndef __DISPLAY_DEVICE_H
#define __DISPLAY_DEVICE_H


//������ʾ�豸�ṹ��

typedef struct DisplayDevice
{
	char *name;
	void *FBBase; /* CPU��ֱ�Ӷ�д���Դ�Ļ���ַ */
	int iXres;    /* X����ֱ��� */
	int iYres;    /* Y����ֱ��� */
	int iBpp;     /* ÿ������ʹ�ö��ٸ�Bit(Bit per pixel)	*/
	int (*Init)(struct DisplayDevice *ptDev);   /* Ӳ����ʼ�� */
	void (*Flush)(struct DisplayDevice *ptDev); /* ��FBBase������ˢ��LCD���Դ��� */
	int (*SetPixel)(struct DisplayDevice *ptDev, int iX, int iY, unsigned int dwColor); /* ����ĳ�����أ�dwColor=0xRRGGBB */
	
	struct DisplayDevice *pNext;	//����ָ��ָ����һ���豸�ڵ�
	int iSize;		//�Դ��С��bytes��
}DisplayDevice, *PDisplayDevice;

//����ĸ��豸
PDisplayDevice GetDisplayDevice(char *name);

//ע��һ���豸
void DisplayDeviceRegister(PDisplayDevice ptDisplayDevice);

//��ʾһ���豸�����������ҵ�������ͬ���豸��
PDisplayDevice __GetDisplayDevice(char *name);

#endif
