#ifndef __NET_SYSTEM_H
#define __NET_SYSTEM_H

//SNTP����ʱ��ṹ��
typedef struct NetTime
{
    char year[5];
    char month[4];
    char day[3];
    char hour[3];
    char min[3];
    char sec[3];
}NetTime, *ptNetTime;

//�����豸�ṹ�����
typedef struct NetDevice
{
	char* name;
	char ip[20];
	char mac[6];
	int (*Init)();
	int (*Connect)(char *SSID, char *password);
	int (*GetInfo)(struct NetDevice *ptDev, char *ip);	//��ȡ���ӵ�IP
	int (*CreateTransfer)(char *Type, int iLocalPort);
	void (*CloseTransfer)();
	/* source, dest, data */
	int (*Send)(unsigned char *Data, int piLen, int time_out);
	int (*Recv)(unsigned char *Data, int *piLen, int iTimeoutMS);
	//Type��"UDP"/"TCP"��iLocalPort�����ض˿ڣ�*pSrcIP�����Ͷ�IP��piLen�����ݳ���
	
	//��ȡSNTPʱ��
	int (*GetTime)(struct NetDevice *ptDev, ptNetTime time);
	struct NetDevice* pNext;
	
}NetDevice, *pNetDevice;
//���ĳһ�������豸������
void NetDeviceRegister(pNetDevice ptNetDevice);

//������������豸
void AddNetDevices(void);

//��ȡ����
pNetDevice GetNetDevice(char *name);


#endif

