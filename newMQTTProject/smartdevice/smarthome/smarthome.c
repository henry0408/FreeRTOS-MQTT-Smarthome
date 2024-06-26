#include <stdio.h>
#include <string.h>
#include <input_buffer.h>
#include <input_system.h>
#include <led_device.h>
#include <dht11_device.h>
#include <fan_device.h>
#include <display_system.h>
#include <font_system.h>
#include <show_text.h>
#include <net_system.h>
#include <time.h>
#include <message.h>
#include "smarthome.h"

#include "stm32f1xx_hal.h"

/**********************************************************************
 * �������ƣ� SystemInit
 * ���������� ϵͳ��ʼ��:��ʼ��������ϵͳ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 ***********************************************************************/
static void SubSystemInit(void)
{
	pNetDevice pNetDev;

	HAL_Delay(2000);
	
	/* 1.��ʼ��LED */
	PLEDDevice p1 = GetLEDDevice(LED_WHITE);
	PLEDDevice p2 = GetLEDDevice(LED_BLUE);
	PLEDDevice p3 = GetLEDDevice(LED_GREEN);

	p1->Init(p1);
	p2->Init(p2);
	p3->Init(p3);

	/* 2. ��ʼ��FAN */
	pFANDevice p4 = GetFANDevice();
	p4->Init(p4);

	/* 3. ��ʼ����ʾ�豸 */
	PDisplayDevice ptDevice;
	char *name = "OLED";

	/* 3.1 ���������ʾ�豸 */
	AddDisplayDevices();

	/* 3.2 ȡ��OLED��ʾ�豸 */
	ptDevice = GetDisplayDevice(name);

	if (!ptDevice)
	{
		printf("Can not get DisplayDevice %s\r\n", name);
		return;
	}

	/* 3.3 ��ʼ����ʾ�豸 */
	ptDevice->Init(ptDevice);

	/* 3.4 �����Ļ */
	memset(ptDevice->FBBase, 0, ptDevice->iSize);

	/* 3.5 Flush��Ļ */
	ptDevice->Flush(ptDevice);

	/* 4. ��ʼ���ֿ� */

	/* 4.1 ����ֿ� */
	AddFontLibs();

	/* 4.2 ѡ��Ĭ���ֿ� */
	SetDefaultFontLib("ascii");

	/* 4.3 ��ʼ��Ĭ���ֿ� */
	InitDefaultFontLib();

	/* 5. ��ʼ������ϵͳ */

	/* 5.1 ������������豸 */
	AddInputDevices();

	/* 5.2 ��ʼ�����������豸 */
	InitInputQueue();
	InitInputDevices();

	/* 6. ��ʼ������ϵͳ */

	/* 6.1 ������� */
	AddNetDevices();

	/* 6.2 ���ESP8266���� */
	pNetDev = GetNetDevice("esp8266");

	/* 6.3 ��ʼ������ */
	pNetDev->Init(pNetDev);	
}

/**********************************************************************
 * �������ƣ� WifiConnect
 * ���������� ����WIFI�ȵ�
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 ***********************************************************************/


//��������֪ͨ�źţ���wifi���ӳɹ�֮�����DisplayNetTimeInfo���Ի�ȡ�����豸
TaskHandle_t xRecvTask;

void WifiConnect(void)
{
	pNetDevice pNetDev;
	
	/* ���ESP8266���� */
	pNetDev = GetNetDevice("esp8266");
	while (1)
	{
		/* ����WIFI�ȵ�: ����ʵ��ʱ��Ҫ�޸�����: wifi�ȵ���,���� */
		if (pNetDev->Connect(SSID, PASSWORD) == 0)
		{
			printf("Connect WIFI ok\r\n");
			xTaskNotifyGive( xRecvTask );			//����֪ͨ
			break;
		}
		else
		{
			printf("Connect WIFI err\r\n");
			HAL_Delay(1000);
		}
	}
}

/**********************************************************************
 * �������ƣ� DisplayNetTimeInfo
 * ���������� ��ȡSNTP������ʱ�䣬����OLED��ʾ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 ***********************************************************************/
void DisplayNetTimeInfo(void)
{
	pNetDevice pNetDev = NULL;
	PDisplayDevice ptDisplayDevice = NULL;
	NetTime time;
	
	enum state
	{
			STATE_GETNET,
			STATE_GETTIME
	};
	
	//��ʼ״̬Ϊ�ȴ���������WiFi״̬
	enum state current_state = STATE_GETNET;
	
	while(1)
	{
		if(current_state == STATE_GETNET)
		{
			int ret = ulTaskNotifyTake(pdTRUE, 0);	//�ȴ�WifiConnect��������������֪ͨ
			if(ret > 0)
			{
				pNetDev = GetNetDevice("esp8266");
				ptDisplayDevice = GetDisplayDevice("OLED");
				current_state = STATE_GETTIME;			//��ȡ����ֱ�ӽ�����ʾ����״̬
			}
			else
			{
				vTaskDelay(1);
			}
		}
		else if(current_state == STATE_GETTIME)
		{
			pNetDev->GetTime(pNetDev, &time);
			/* �����Ļ */
			memset(ptDisplayDevice->FBBase, 0, ptDisplayDevice->iSize);
			ptDisplayDevice->Flush(ptDisplayDevice);
			
			/* ��ʾ������ */
			ShowTextInDisplayDevice(ptDisplayDevice, 16, 31, time.year);
			ShowTextInDisplayDevice(ptDisplayDevice, 48, 31, "/");
			ShowTextInDisplayDevice(ptDisplayDevice, 56, 31, time.month);
			ShowTextInDisplayDevice(ptDisplayDevice, 80, 31, "/");
			ShowTextInDisplayDevice(ptDisplayDevice, 88, 31, time.day);
			
			/* ��ʾʱ���� */
			ShowTextInDisplayDevice(ptDisplayDevice, 24, 48, time.hour);
			ShowTextInDisplayDevice(ptDisplayDevice, 40, 48, ":");
			ShowTextInDisplayDevice(ptDisplayDevice, 48, 48, time.min);
			ShowTextInDisplayDevice(ptDisplayDevice, 64, 48, ":");
			ShowTextInDisplayDevice(ptDisplayDevice, 72, 48, time.sec);
		}
	}
	
}


/**********************************************************************
 * �������ƣ� DisplayNetInfoAndCreateTransfer
 * ���������� ��ʾ������Ϣ(IP�Ͷ˿�), ����������
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 ***********************************************************************/

//��������֪ͨ�źţ���wifi���ӳɹ��Լ���ʼ����֮�����DHT11_UpdateValue���Ի�ȡDHT11��ʪ��
TaskHandle_t xDHT11Task;

void DisplayNetInfoAndCreateTransfer(void)
{
	pNetDevice pNetDev;
	char ip[20];
	int port = LOCAL_PORT;

	PDisplayDevice ptDevice;
	char str[100];
	
	/* ���ESP8266���� */
	pNetDev = GetNetDevice("esp8266");
	
	/* ��õ���IP */
	if (pNetDev->GetInfo(pNetDev, ip) == 0)
	{
		printf("Board IP = %s, port = %d\r\n", ip, port);
	}
	else
	{
		printf("GetInfo err\r\n");
		return;
	}

	/* �������紫��
	 * �ȹر�, ��Ϊ���ǵĿ�������ܶ�δ�������, �ȹر��ϴδ���
	 */
	pNetDev->CloseTransfer(pNetDev);
	if (pNetDev->CreateTransfer("UDP", port) == 0)
	{
		printf("Create Transfer ok\r\n");
		xTaskNotifyGive( xDHT11Task );			//����֪ͨ
	}
	else
	{
		printf("Create Transfer err\r\n");
	}

	/* ��LCD����ʾ���� */
	/* ���ָ������ʾ�豸 */
	ptDevice = GetDisplayDevice("OLED");
	
	/* ����Ļ����ʾ���� */
	/* �����Ļ */
	memset(ptDevice->FBBase, 0, ptDevice->iSize);
	ptDevice->Flush(ptDevice);
	
	/* ��ʾip */
	ShowTextInDisplayDevice(ptDevice, 0, 15, "IP:");
	ShowTextInDisplayDevice(ptDevice, 0, 31, ip);

	/* ��ʾport */
	sprintf(str, "Port:%d", port);
	ShowTextInDisplayDevice(ptDevice, 0, 47, str);

	/* ��ʾ���� */
	ShowTextInDisplayDevice(ptDevice, 0, 63, "Welcome back");

	
}

/**********************************************************************
 * �������ƣ� SmartHomeControlDevices
 * ���������� ���ܼҾӿ����߼�
 * ��������� strJson-Json��ʽ�Ĳ���,����: {"dev":"lamp1","status":"1"}
 * ��������� ��
 * �� �� ֵ�� ��
 ***********************************************************************/
static void SmartHomeControlDevices(char *strJson)
{
	char dev[10];
	int status;
	char *pTmp;
	int i;
	int which;

	PLEDDevice ptLED;
	pFANDevice ptFAN;

	/* ȡ��dev��ֵ */
	pTmp = strstr(strJson, "\"dev\":\"");
	if (!pTmp)
		return;
	pTmp += strlen("\"dev\":\"");
	for (i = 0; pTmp[i] != '\"'; i++)
		dev[i] = pTmp[i];
	dev[i] = '\0';

	/* ȡ��status��ֵ */
	pTmp = strstr(strJson, "\"status\":\"");
	if (!pTmp)
		return;
	pTmp += strlen("\"status\":\"");
	status = (pTmp[0] == '-') ? (0 - (pTmp[1] - '0')) : (pTmp[0] - '0');

	if (strncmp(dev, "lamp", 4) == 0)
	{
		/* ����LED */
		which = dev[4] - '0' - 1;
		ptLED = GetLEDDevice(which);
		if (ptLED)
			ptLED->Control(ptLED, status);
	}
	else if (strncmp(dev, "fan", 3) == 0)
	{
		/* ����FAN */
		ptFAN = GetFANDevice();
		if (ptFAN)
			ptFAN->SetSpeed(status);
	}
	else if (strncmp(dev, "home", 4) == 0)
	{
		/* home */
		if (!status)
		{
			/* �ؼ�: ������LED������ */
			for (i = 0; i < 3; i++)
			{
				ptLED = GetLEDDevice(i);
				if (ptLED)
					ptLED->Control(ptLED, 1);
			}

			ptFAN = GetFANDevice();
			if (ptFAN)
				ptFAN->SetSpeed( 1);
		}
		else
		{
			/* ���: �ر�����LED������ */
			for (i = 0; i < 3; i++)
			{
				ptLED = GetLEDDevice(i);
				if (ptLED)
					ptLED->Control(ptLED, 0);
			}
			ptFAN = GetFANDevice();
			if (ptFAN)
				ptFAN->SetSpeed( 0);
		}
	}
	
}

/**********************************************************************
 * �������ƣ� DisplayStartingInfo
 * ���������� ��ʾ������Ϣ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 ***********************************************************************/
static void DisplayStartingInfo(void)
{	
	PDisplayDevice ptDevice;
	char *name = "OLED";

	/* ȡ��OLED��ʾ�豸 */
	ptDevice = GetDisplayDevice(name);

	/* �����Ļ */
	memset(ptDevice->FBBase, 0, ptDevice->iSize);
	
	/* ����Ļ����ʾ: Starting */
	//ShowTextInDisplayDevice(ptDevice, 0, 16, VERSION);
	ShowTextInDisplayDevice(ptDevice, 0, 34, "Starting...");
}

/**********************************************************************
 * �������ƣ� DisplayWifiStatus
 * ���������� ��ʾWIF״̬
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 ***********************************************************************/
static void DisplayWifiStatus(void)
{	
	PDisplayDevice ptDevice;
	char *name = "OLED";

	/* ȡ��OLED��ʾ�豸 */
	ptDevice = GetDisplayDevice(name);
	
	/* ����Ļ����ʾ: Starting */
	ShowTextInDisplayDevice(ptDevice, 0, 54, "WIFI Connecting ...");
}


/**********************************************************************
 * �������ƣ� SmartHomeTask
 * ���������� ���ܼҾ�������
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 ***********************************************************************/


void SmartHomeTask(void)
{
	char strJson[100];
	InputEvent event;
	
	/* ��ʼ�������豸/��ϵͳ 		*/
	SubSystemInit();

	/* ��ʾ������Ϣ */
	DisplayStartingInfo();

	/* ��ʾWIFI��Ϣ */
	DisplayWifiStatus();

	vTaskSuspendAll();
	/* ����WIFI			   */
	WifiConnect();
	
	
	/* ��OLED����ʾIP���˿�	*/
	DisplayNetInfoAndCreateTransfer();
	xTaskResumeAll();
	
	
	while (1)
	{
		/* ��ȡInputEvent 	   */
		if (0 == GetInputEvent(&event))
		{
			/* ת��ΪJson��ʽ�Ĳ���  */
			if (0 == ConvertInputEventToJson(&event, strJson))
			{
				/* �����豸 			 */
				SmartHomeControlDevices(strJson);
			}
		}
	}
}

//��ʾ��ʪ�Ⱥ���
//void DHT11_UpdateValue(void)
//{
//	pNetDevice pNetDev = NULL;
//	PDHT11Dev dht11 = NULL;
//	
//	enum state
//	{
//			STATE_WAIT,
//			STATE_UPDATE
//	};
//	
//	//��ʼ״̬Ϊ�ȴ���������WiFi״̬
//	enum state current_state = STATE_WAIT;
//	
//	while(1)
//	{
//		if(current_state == STATE_WAIT)
//		{
//			int ret = ulTaskNotifyTake(pdTRUE, 0);	//�ȴ�WifiConnect��������������֪ͨ
//			if(ret > 0)
//			{
//				pNetDev = GetNetDevice("esp8266");
//				/* ��ʼ��DHT11�豸 */
//				dht11 = GetDHT11Device(1);
//				dht11->DHT11Init();
//				
//				current_state = STATE_UPDATE;			//��ȡ����ֱ�ӽ�����ʾ����״̬
//			}
//			else
//			{
//				vTaskDelay(1);
//			}
//		}
//		else if(current_state == STATE_UPDATE)
//		{
//			vTaskSuspendAll();
//			int err = dht11->GetValue(dht11);
//			printf("T=%d\tH=%d\r\n", dht11->value.temperature, dht11->value.humidity);
//			
//			unsigned char send_message[10] = "Sendtest";
//			pNetDev->Send(send_message, 8, 100);
//			
//			xTaskResumeAll();
//			vTaskDelay(5000);
//		}
//	}

//}

void DHT11_UpdateValue(void)
{
	pNetDevice pNetDev = NULL;
	PDHT11Dev dht11 = NULL;
	
	enum state
	{
			STATE_WAIT,
			STATE_UPDATE
	};
	
	//��ʼ״̬Ϊ�ȴ���������WiFi״̬
	enum state current_state = STATE_WAIT;
	
	while(1)
	{
		if(current_state == STATE_WAIT)
		{
			int ret = ulTaskNotifyTake(pdTRUE, 0);	//�ȴ�WifiConnect��������������֪ͨ
			if(ret > 0)
			{
				pNetDev = GetNetDevice("esp8266");
				/* ��ʼ��DHT11�豸 */
				dht11 = GetDHT11Device(1);
				dht11->DHT11Init();
				
				current_state = STATE_UPDATE;			//��ȡ����ֱ�ӽ�����ʾ����״̬
			}
			else
			{
				vTaskDelay(1);
			}
		}
		else if(current_state == STATE_UPDATE)
		{
			vTaskSuspendAll();
			int err = dht11->GetValue(dht11);
			printf("T=%d\tH=%d\r\n", dht11->value.temperature, dht11->value.humidity);
			
			unsigned char send_message[10] = "Sendtest";
			pNetDev->Send(send_message, 8, 100);
			
			xTaskResumeAll();
			vTaskDelay(5000);
		}
	}

}