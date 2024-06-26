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
	/* 保存数据 */
	char *buf = g_ESP8266DataBuff;
	int i = g_DataBuffIndex;
	int m;
	InputEvent event;

	buf[i] = c;
	g_DataBuffIndex++;

	/* 解析: +IPD,n:xxxxxxxxxx */
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
				/* 计算长度 */
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
				/* 接收完数据 */
				/* 构造InputEvent并上报 */
				/* 构造InputEvent */
				//event.time	= KAL_GetTime();  /* kal */
				event.type = INPUT_EVENT_NET;
				strncpy(event.str, buf, g_DataLen);
				event.str[g_DataLen] = '\0';
				
				/* 放入环形缓冲区 */
				PutInputEvent(&event);

				/* 恢复初始状态 */
				g_status = INIT_STATUS;
				g_DataBuffIndex = 0;
				g_DataLen = 0;
			}
			break;
		}
		
	}

	
}


//初始化不需要任何操作
int NetDeviceInit()
{
	//网络子系统里，我们已经初始化了需要的硬件
	
	//回调函数会被用于处理ESP8266返回到USART3的数据，把解析到的数据转换成InputEvnet
	SetNetInputProcessCallback(ESP8266DataProcessCallback);
	return 0;
}


//typedef struct InputDevice
//{
//	char name[10];														//设备名称
//	int (*Init)();														//初始化设备
//	int (*GetEvent)(pInputEvent event);				//获取输入事件	
//	struct InputDevice* pNext;								//指针指向链表下一个节点
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

