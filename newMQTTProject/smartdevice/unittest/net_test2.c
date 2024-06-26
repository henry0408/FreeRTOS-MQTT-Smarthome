//#include <stdio.h>
//#include <string.h>
//#include <time.h>
//#include <input_system.h>
//#include <input_buffer.h>

//#include "net_test2.h"
//#include "net_system.h"
//#include "stm32f1xx_hal.h"



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



