#include <stdio.h>
#include <string.h>
#include <time.h>
#include <input_system.h>
#include <input_buffer.h>

#include "net_system.h"
#include "stm32f1xx_hal.h"
#include "mqtt_test1.h"
#include "dht11_device.h"

#include "onenet.h"

void mqtt_test1(void)
{
	int port = 1234;		//默认用1234号端口与PC相连
	
	HAL_Delay(2000);
	//添加网卡设备
	AddNetDevices();
	
	//获取网卡设备
	pNetDevice net_dev;
	net_dev = GetNetDevice("esp8266");
	
	//初始化网卡设备
	net_dev->Init();
	
	//连接WiFi(如果没连上则一直重连)
	while (1)
	{
		if(net_dev->Connect("HUAWEI_2C71", "Chy08042000") == 0)
		{
			printf("Connect WIFI ok\r\n");
			break;
		}
		else
		{
			printf("Connect WIFI err\r\n");
			HAL_Delay(1000);
		}
	}
	
	//获取IP
	char ip[50];
	if(net_dev->GetInfo(net_dev, ip) == 0)
	{
		printf("Board IP = %s, port = %d\r\n", ip, port);
	}
	else
	{
		printf("GetInfo err\r\n");
		return;
	}
		
	
	//连接MQTT服务器
	printf("Connect MQTTs Server...\r\n");
	net_dev->CloseTransfer();
	while(1)
	{
		if( net_dev->CreateTransfer("TCP", 1883) == 0 )
		{
			printf("Create MQTT Transfer ok\r\n");
			break;
		}
		else
		{
			printf("Create MQTT Transfer err\r\n");
			//return;
		}
	}
	
	
	while(1)
	{
		printf("Device Login...\r\n");
		if(OneNet_DevLink() == 0)
		{
			printf("Device Login Success\r\n");
			break;
		}
		else
		{
			printf("Device Login Fail\r\n");
		}
	}
	
	PDHT11Dev dht11 = GetDHT11Device(1);
	if(dht11)
	{
		printf("Get DHT11 device ok\r\n");
	}
	else
	{
		printf("Get DHT11 device fail\r\n");
	}
	//int err;

	/* 初始化DHT11设备 */
	dht11->DHT11Init();

	while (1)	
	{
		//vTaskSuspendAll();
		int err = dht11->GetValue(dht11);
		//printf("T=%d\tH=%d\r\n", dht11->value.temperature, dht11->value.humidity);
		//HAL_Delay(5000);
		
		OneNet_SendData(dht11->value.temperature, dht11->value.humidity);
		//xTaskResumeAll();
		//vTaskDelay(5000);
		HAL_Delay(5000);
	}
	
}


//void net_test2(void)
//{
//	int port = 1234;		//默认用1234号端口与PC相连
//	
//	HAL_Delay(2000);
//	//添加网卡设备
//	AddNetDevices();
//	
//	//获取网卡设备
//	pNetDevice net_dev;
//	net_dev = GetNetDevice("esp8266");
//	
//	//初始化网卡设备
//	net_dev->Init();
//	
//	//连接WiFi(如果没连上则一直重连)
//	while (1)
//	{
//		if(net_dev->Connect("HUAWEI_2C71", "Chy08042000") == 0)
//		{
//			printf("Connect WIFI ok\r\n");
//			break;
//		}
//		else
//		{
//			printf("Connect WIFI err\r\n");
//			HAL_Delay(1000);
//		}
//	}
//	
//	//获取IP
//	char ip[50];
//	if(net_dev->GetInfo(net_dev, ip) == 0)
//	{
//		printf("Board IP = %s, port = %d\r\n", ip, port);
//	}
//	else
//	{
//		printf("GetInfo err\r\n");
//		return;
//	}
//	
//	//创建UDP传输
//	//先关闭（上次）传输
//	net_dev->CloseTransfer();
//	if( net_dev->CreateTransfer("UDP", port) == 0 )
//	{
//		printf("Create Transfer ok\r\n");
//	}
//	else
//	{
//		printf("Create Transfer err\r\n");
//		return;
//	}
//	
////	//读取网络数据，存到data，长度存到data_len
////	unsigned char data[200];
////	int data_len;
////	while(1)
////	{
////		/* 读取网络数据 */
////		if (0 == net_dev->Recv(data, &data_len, 100))			//int ESP8266Recv(unsigned char *Data, int *piLen, int iTimeoutMS)
////		{
////			data[data_len] = '\0';
////			printf("Get NetData: %s\r\n", data);
////		}
////		
////	}
//	AddInputDevices();
//	InitInputDevices();
//	InputEvent event;
//	while (1)
//	{
//		if (GetInputEvent(&event) == 0)
//		{
//			if (event.type == INPUT_EVENT_KEY)
//			{
//				printf("get key input event:\r\n");
//				printf("type: %d\r\n", event.type);
//				//printf("time: %d\r\n", event.time);
//				printf("key : %d\r\n", event.iKey);
//				printf("pressure : %d\r\n", event.iPressure);
//			}
//			else if(event.type == INPUT_EVENT_NET)
//			{
//				printf("get net input event:\r\n");
//				printf("type: %d\r\n", event.type);
//				//printf("time: %d\r\n", event.time);
//				printf("str : %s\r\n", event.str);
//			}
//		} 
//	}
//	
//}



