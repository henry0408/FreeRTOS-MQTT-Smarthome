#include "esp8266.h"
#include "net_system.h"
#include "at_command.h"
#include <string.h>

#define TIME_OUT 10		//Ĭ�ϳ�ʱʱ��Ϊ10ms

int ESP8266Init()
{
	//AT���ʼ��
	ATInterfaceInit();
	
	// 1. ���� WiFi ģʽ
	//AT+CWMODE=3						// softAP+station mode
	ATCommandSend("AT+CWMODE=3", TIME_OUT);
	
	//���õ�����
	//AT+CIPMUX=0
	ATCommandSend("AT+CIPMUX=0",TIME_OUT);
	return 0;
}

//����WiFi
int ESP8266Connect(char *SSID, char *password)
{
	//2. ����·����
	//AT+CWJAP="Programmers","100asktech" // SSID and password of router
	//��Ӧ��OK
	
	char cmd[100];
	sprintf(cmd,"AT+CWJAP=\"%s\",\"%s\"", SSID, password);
	return ATCommandSend(cmd, TIME_OUT*1000);
	
	
	
}

//��ȡ����IP��ַ
int ESP8266GetInfo(struct NetDevice *ptDev, char *ip_buf)
{
//	3. ��ѯ ESP8266 �豸�� IP ��ַ
//	AT+CIFSR
//	��Ӧ��
//	+CIFSR:APIP,"192.168.4.1"
//	+CIFSR:APMAC,"1a:fe:34:a5:8d:c6"
//	+CIFSR:STAIP,"192.168.3.133"
//	+CIFSR:STAMAC,"18:fe:34:a5:8d:c6"
//	OK
	
	char info_buf[200];
	char* ip;
	//����ATָ��������ȡ����Ϣ
	if(ATDataSendandReceive("AT+CIFSR", info_buf, TIME_OUT*1000) == 0)
	{
		/* ������IP */
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

//�������ӣ�TCP/UDP��
int ESP8266CreateTransfer(char *Type, int iLocalPort)
{
//	5. ���� UDP ����, ���һ����������Ϊ2ʱ��ʾUDPͨ�ŵ�Զ�˿ɸı�
//	AT+CIPSTART="UDP","192.168.101.110",8080,1112,2
//	��Ӧ��
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

//�ر�����
void ESP8266CloseTransfer()
{
	ATCommandSend("AT+CIPCLOSE", TIME_OUT);
}

//��������
//int ESP8266Send(char *Type, char *pDestIP, int iDestPort, char *Data, int iLen)
//{
//	return 0;
//}

//��������
int ESP8266Send(unsigned char *Data, int piLen, int time_out)
{
	
	return ATDataSend(Data, piLen, time_out);
}

//��������
int ESP8266Recv(unsigned char *Data, int *piLen, int iTimeoutMS)
{
	return ATDataRecv(Data, piLen, iTimeoutMS);
}

//��ȡSNTPʱ��
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
			//����ʱ���SNTP������IP
			ret = ATCommandSend("AT+CIPSNTPCFG=1,8,\"120.25.115.20\"", TIME_OUT*1000);
			if(ret == 0)	//���óɹ�
			{
				current_mode = MODE_GETDATA;
			}
		}
		else if(current_mode == MODE_GETDATA)
		{
			//��ȡʱ��
			char buf[100] = {0};
			char *base_addr = NULL;	//���������ַ����ַ�����λ�õ�
			ret = ATDataSendandReceive("AT+CIPSNTPTIME?", buf, TIME_OUT*1000);
			if(!ret)
			{
					base_addr = strstr(buf, "+CIPSNTPTIME:");
					if(base_addr)
					{
							base_addr = base_addr + strlen("+CIPSNTPTIME:") + 4;    // �·��׵�ַ
							memcpy(time->month, base_addr, 3);
							time->month[3] = '\0';
							
							base_addr = base_addr + 4;  // �����׵�ַ
							memcpy(time->day, base_addr, 2);
							time->day[2] = '\0';
							
							base_addr = base_addr + 3; // ʱ�׵�ַ
							memcpy(time->hour, base_addr, 2);
							time->hour[2] = '\0';
							
							base_addr = base_addr + 3; // ���׵�ַ
							memcpy(time->min, base_addr, 2);
							time->min[2] = '\0';
							
							base_addr = base_addr + 3; // ���׵�ַ
							memcpy(time->sec, base_addr, 2);
							time->sec[2] = '\0';
							
							base_addr = base_addr + 3; // ����׵�ַ
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


