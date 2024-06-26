#include "net_input.h"
#include "input_system.h"
#include "input_buffer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "driver_usart.h"

typedef void (*NetInputProcessCallback)(char c);
extern void SetNetInputProcessCallback(NetInputProcessCallback func);


enum AT_STATUS {
	INIT_STATUS,
	LEN_STATUS,
	DATA_STATUS
};

static enum AT_STATUS g_status = INIT_STATUS;


static char g_ESP8266DataBuff[50];
static int g_DataBuffIndex = 0;
static int g_DataLen = 0;


static void ESP8266DataProcessCallback(char c)
{	
	/* �������� */
	char *buf = g_ESP8266DataBuff;
	int i = g_DataBuffIndex;
	int m;
	InputEvent event;

	buf[i] = c;
	g_DataBuffIndex++;

	/* ����: +IPD,n:xxxxxxxxxx */
	switch (g_status)
	{
		case INIT_STATUS:
		{
			if (buf[0] != '+')
			{
				g_DataBuffIndex = 0;
			}			
			else if (i == 4)
			{
				if (strncmp(buf, "+IPD,", 5) == 0)
				{
					g_status = LEN_STATUS;
				}
				g_DataBuffIndex = 0;
			}
			break;
		}

		case LEN_STATUS:
		{
			if (buf[i] == ':')
			{
				/* ���㳤�� */
				for (m = 0; m < i; m++)
				{
					g_DataLen = g_DataLen * 10 + buf[m] - '0';
				}
				g_status = DATA_STATUS;
				g_DataBuffIndex = 0;
			}
			break;
		}

		case DATA_STATUS:
		{
			if (g_DataBuffIndex == g_DataLen)
			{
				/* ���������� */
				/* ����InputEvent���ϱ� */
				/* ����InputEvent */
				//event.time	= KAL_GetTime();  /* kal */
				event.type = INPUT_EVENT_NET;
				strncpy(event.str, buf, g_DataLen);
				event.str[g_DataLen] = '\0';
				
				/* ���뻷�λ����� */
				PutInputEvent(&event);

				/* �ָ���ʼ״̬ */
				g_status = INIT_STATUS;
				g_DataBuffIndex = 0;
				g_DataLen = 0;
			}
			break;
		}
		
	}

	
}


//��ʼ������Ҫ�κβ���
int NetDeviceInit()
{
	//������ϵͳ������Ѿ���ʼ������Ҫ��Ӳ��
	
	//�ص������ᱻ���ڴ���ESP8266���ص�USART3�����ݣ��ѽ�����������ת����InputEvnet
	SetNetInputProcessCallback(ESP8266DataProcessCallback);
	return 0;
}


//typedef struct InputDevice
//{
//	char name[10];														//�豸����
//	int (*Init)();														//��ʼ���豸
//	int (*GetEvent)(pInputEvent event);				//��ȡ�����¼�	
//	struct InputDevice* pNext;								//ָ��ָ��������һ���ڵ�
//	
//}InputDevice, *pInputDevice;
		
static InputDevice g_tNetDevice = 
{
	"esp8266",
	NetDeviceInit,
	NULL
};

void AddInputDeviceNet(void)
{
	InputDeviceRegister(&g_tNetDevice);
}

