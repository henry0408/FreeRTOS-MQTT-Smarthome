#ifndef __NET_SYSTEM_H
#define __NET_SYSTEM_H

//SNTP网络时间结构体
typedef struct NetTime
{
    char year[5];
    char month[4];
    char day[3];
    char hour[3];
    char min[3];
    char sec[3];
}NetTime, *ptNetTime;

//网卡设备结构体抽象
typedef struct NetDevice
{
	char* name;
	char ip[20];
	char mac[6];
	int (*Init)();
	int (*Connect)(char *SSID, char *password);
	int (*GetInfo)(struct NetDevice *ptDev, char *ip);	//获取连接的IP
	int (*CreateTransfer)(char *Type, int iLocalPort);
	void (*CloseTransfer)();
	/* source, dest, data */
	int (*Send)(unsigned char *Data, int piLen, int time_out);
	int (*Recv)(unsigned char *Data, int *piLen, int iTimeoutMS);
	//Type："UDP"/"TCP"，iLocalPort：本地端口，*pSrcIP：发送端IP，piLen：数据长度
	
	//获取SNTP时间
	int (*GetTime)(struct NetDevice *ptDev, ptNetTime time);
	struct NetDevice* pNext;
	
}NetDevice, *pNetDevice;
//添加某一个网卡设备到链表
void NetDeviceRegister(pNetDevice ptNetDevice);

//添加所有网卡设备
void AddNetDevices(void);

//获取网卡
pNetDevice GetNetDevice(char *name);


#endif

