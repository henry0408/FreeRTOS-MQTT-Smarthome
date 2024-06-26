#include "esp8266.h"
#include "net_system.h"
#include "at_command.h"
#include <string.h>

#define TIME_OUT 10		//默认超时时间为10ms

int ESP8266Init()
{
	//AT层初始化
	ATInterfaceInit();
	
	// 1. 配置 WiFi 模式
	//AT+CWMODE=3						// softAP+station mode
	ATCommandSend("AT+CWMODE=3", TIME_OUT);
	
	//配置单连接
	//AT+CIPMUX=0
	ATCommandSend("AT+CIPMUX=0",TIME_OUT);
	return 0;
}

//连接WiFi
int ESP8266Connect(char *SSID, char *password)
{
	//2. 连接路由器
	//AT+CWJAP="Programmers","100asktech" // SSID and password of router
	//响应：OK
	
	char cmd[100];
	sprintf(cmd,"AT+CWJAP=\"%s\",\"%s\"", SSID, password);
	return ATCommandSend(cmd, TIME_OUT*1000);
	
	
	
}

//获取网卡IP地址
int ESP8266GetInfo(struct NetDevice *ptDev, char *ip_buf)
{
//	3. 查询 ESP8266 设备的 IP 地址
//	AT+CIFSR
//	响应：
//	+CIFSR:APIP,"192.168.4.1"
//	+CIFSR:APMAC,"1a:fe:34:a5:8d:c6"
//	+CIFSR:STAIP,"192.168.3.133"
//	+CIFSR:STAMAC,"18:fe:34:a5:8d:c6"
//	OK
	
	char info_buf[200];
	char* ip;
	//发送AT指令，并保存获取的信息
	if(ATDataSendandReceive("AT+CIFSR", info_buf, TIME_OUT*1000) == 0)
	{
		/* 解析出IP */
		ip = strstr(info_buf, "+CIFSR:STAIP,\"");
		if (ip)
		{
			ip += strlen("+CIFSR:STAIP,\"");
			int i;
			for (i = 0; ip[i] != '\"'; i++)
			{
				ptDev->ip[i] = ip[i];
			}
			ptDev->ip[i] = '\0';
			strcpy(ip_buf, ptDev->ip);
			return 0;
		}
	}
	return -1;
}

//建立连接（TCP/UDP）
int ESP8266CreateTransfer(char *Type, int iLocalPort)
{
//	5. 创建 UDP 传输, 最后一个参数设置为2时表示UDP通信的远端可改变
//	AT+CIPSTART="UDP","192.168.101.110",8080,1112,2
//	响应：
//	CONNNECT	
//	OK
	
	char cmd[100];
	if(strcmp(Type, "udp") == 0 || strcmp(Type, "UDP") == 0 )
	{
		sprintf(cmd, "AT+CIPSTART=\"UDP\",\"192.168.8.5\",8080,%d,2", iLocalPort);
	}
	else if( strcmp(Type, "tcp") == 0 || strcmp(Type, "TCP") == 0 )
	{
		//sprintf(cmd, "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",%d", iLocalPort);
		sprintf(cmd, "AT+CIPSTART=\"TCP\",\"183.230.40.96\",%d", iLocalPort);
		
	}
	
	
	return ATCommandSend(cmd, TIME_OUT*1000);
}

//关闭连接
void ESP8266CloseTransfer()
{
	ATCommandSend("AT+CIPCLOSE", TIME_OUT);
}

//发送数据
//int ESP8266Send(char *Type, char *pDestIP, int iDestPort, char *Data, int iLen)
//{
//	return 0;
//}

//发送数据
int ESP8266Send(unsigned char *Data, int piLen, int time_out)
{
	
	return ATDataSend(Data, piLen, time_out);
}

//接收数据
int ESP8266Recv(unsigned char *Data, int *piLen, int iTimeoutMS)
{
	return ATDataRecv(Data, piLen, iTimeoutMS);
}

//获取SNTP时间
int ESP8266GetTime(struct NetDevice *ptDev, ptNetTime time)
{
	enum mode
	{
		MODE_CONFIG,
		MODE_GETDATA
	};
	
	enum mode current_mode = MODE_CONFIG;
	int ret;
	while(1)
	{
		if(current_mode == MODE_CONFIG)
		{
			//配置时域和SNTP服务器IP
			ret = ATCommandSend("AT+CIPSNTPCFG=1,8,\"120.25.115.20\"", TIME_OUT*1000);
			if(ret == 0)	//配置成功
			{
				current_mode = MODE_GETDATA;
			}
		}
		else if(current_mode == MODE_GETDATA)
		{
			//获取时间
			char buf[100] = {0};
			char *base_addr = NULL;	//用于设置字符在字符串中位置的
			ret = ATDataSendandReceive("AT+CIPSNTPTIME?", buf, TIME_OUT*1000);
			if(!ret)
			{
					base_addr = strstr(buf, "+CIPSNTPTIME:");
					if(base_addr)
					{
							base_addr = base_addr + strlen("+CIPSNTPTIME:") + 4;    // 月份首地址
							memcpy(time->month, base_addr, 3);
							time->month[3] = '\0';
							
							base_addr = base_addr + 4;  // 日期首地址
							memcpy(time->day, base_addr, 2);
							time->day[2] = '\0';
							
							base_addr = base_addr + 3; // 时首地址
							memcpy(time->hour, base_addr, 2);
							time->hour[2] = '\0';
							
							base_addr = base_addr + 3; // 分首地址
							memcpy(time->min, base_addr, 2);
							time->min[2] = '\0';
							
							base_addr = base_addr + 3; // 秒首地址
							memcpy(time->sec, base_addr, 2);
							time->sec[2] = '\0';
							
							base_addr = base_addr + 3; // 年份首地址
							memcpy(time->year, base_addr, 4);
							time->year[4] = '\0';
							
							if(strstr(time->year, "1970"))
							{
									current_mode = MODE_CONFIG;
									continue;
							}
							current_mode = MODE_GETDATA;
							return 0;
					}
			}
		}
	}
}

NetDevice ESP8266NetDeice = 
{
	"esp8266",
	"0.0.0.0",	//ip
	{0,0,0,0,0,0},	//mac
	ESP8266Init,
	ESP8266Connect,
	ESP8266GetInfo,
	ESP8266CreateTransfer,
	ESP8266CloseTransfer,
	ESP8266Send,
	ESP8266Recv,
	ESP8266GetTime
};



void AddESP8266NetDevices(void)
{
	NetDeviceRegister(&ESP8266NetDeice);
}


