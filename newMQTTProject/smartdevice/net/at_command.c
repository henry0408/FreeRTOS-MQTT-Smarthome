#include "at_command.h"
#include "uart_device.h"
#include <string.h>
#include "time.h"
#include <stdlib.h>
#include <stdio.h>

static pUART_Device g_pt_ATInterfaceDev;		//�����豸

void ATInterfaceInit(void)
{
	//��ô���/I2C�豸
	g_pt_ATInterfaceDev = GetATInterfaceDevice();
	
	//�����豸��ʼ��
	g_pt_ATInterfaceDev->Init();
	
}

//��buf�ӿ�ʼ��len�ķ�Χ�ڣ���û��'OK\r\n'
int isReturnOK(char* buf, int len)	
{
	buf[len] = '\0';	//��ȡbuf����Ϊstring
	if( strstr(buf, "OK\r\n") )
	{
		return 0;	//����У�����0
	}
	return -1;	//û�У�����-1
}


int isReturnERROR(char* buf, int len)		//��buf�ӿ�ʼ��len�ķ�Χ�ڣ���û��'ERROR\r\n'
{
	buf[len] = '\0';	//��ȡbuf����Ϊstring
	if( strstr(buf, "ERROR\r\n") )
	{
		return 0;	//����У�����0
	}
	return -1;	//û�У�����-1
	
}


//ͨ��������ESP8266����ָ��󣬻�Ҫ�ж��ڳ�ʱʱ�����Ƿ��յ�OK����ERROR
int ATCommandSend(char* cmd, int time_out)
{
	//printf("Send ATCommand: %s\r\n", cmd);
	
	g_pt_ATInterfaceDev->ClearBuffer();							//��ջ��λ�����
	g_pt_ATInterfaceDev->Write(cmd, strlen(cmd));
	g_pt_ATInterfaceDev->Write("\r\n", 2);
	
	int prev_time;
	int current_time;
	
	prev_time = GetTime();
	
	char line_buf[100];
	int i = 0;
	//��ȡESP8266ͨ�����ڻ������з��ص�����
	while(time_out)
	{
		if(g_pt_ATInterfaceDev->ReadByte(&line_buf[i]) == 0)	//һ���ֽ�һ���ֽڵĶ�ȡ�����������ݴ���buf
		{
			//printf("%c", line_buf[i]);
			//�ж�buf������
			if((line_buf[i-1] == '\r') && (line_buf[i] == '\n'))	//����һ�е�ĩβ
			{
				if(isReturnOK(line_buf,i+1) == 0)							//��������������OK��˵��AT����ͳɹ�
				{
					//printf(" OK\r\n");
					return 0;
				}
				else if(isReturnERROR(line_buf,i+1) == 0)			//��������������ERROR��˵��AT�����ʧ��
				{
					//printf(" ERROR\r\n");
					return -1;
				}
				else			//����������ͨ���ݵĻس����У������ǲ�ͬ���ݣ�����ʼ������ȡ��һ�У���ȡ�������ݷ���buf
				{
					i = 0;
				}
				
			}
			i++;			//������ֽڵ����ݷ���buf��������ȡ��һ���ֽ�
		}

		//��ȡ���ڵ�ʱ��
		current_time = GetTime();
		
		//���ѵ�ʱ��
		if (current_time > prev_time)
		{
			time_out--;			//���³�ʱʱ��
			prev_time = current_time;
		}
			
	}
	printf("timeout\r\n");
	return -2;
}

//PC��ESP8266�����������ݣ�ESP8266�����������ݣ�����Ӧ�û��ڴ�����ʾ+IPD,n:xxxxxxxxxx // received n bytes, data=xxxxxxxxxxx	
int ATDataRecv(unsigned char *Data, int *piLen, int time_out)
{
	*piLen = 0;
	//+IPD,n:xxxxxxxxxx�����Ƿֳ�3�����֣�����״̬��
	enum STATUS
	{
		STATUS_INIT,
		STATUS_LEN,
		STATUS_DATA
	};
	
	enum STATUS current_status = STATUS_INIT;		//�����ʼ״̬
	
	int prev_time;
	int current_time;
	prev_time = GetTime();
	
	char line_buf[100];
	int i = 0;
	//��ȡESP8266ͨ�����ڻ������з��ص�����
	while(time_out)
	{
		if(g_pt_ATInterfaceDev->ReadByte(&line_buf[i]) == 0)		//һ���ֽ�һ���ֽڵĶ�ȡ�����������ݴ���buf
		{
			switch(current_status)
			{
				case STATUS_INIT:
				{
					line_buf[i+1] = '\0';		//�ض�
					if(strstr( line_buf, "+IPD," ))
					{
						current_status = STATUS_LEN;
						i = 0;
					}
					else
					{
						i++;
					}
					break;
				}				
					
				case STATUS_LEN:
				{
					if(line_buf[i] == ':')		//�ض�
					{
						//������ݳ���
						line_buf[i] = '\0';
						//printf("line buf is %s\r\n",line_buf);
						*piLen = atoi(line_buf);
						//printf("piLen is %d\r\n",*piLen);
						current_status = STATUS_DATA;
						i = 0;
					}
					else
					{
						i++;			//��ȡ��һ���ֽ�
					}
					break;
				}
					
				case STATUS_DATA:
				{
					Data[i] = line_buf[i];
					i++;
					if (i == *piLen)
					{
						return 0;
					}
					break;
				}
			}
		}
		
		//��ȡ���ڵ�ʱ��
		current_time = GetTime();
		
		//���ѵ�ʱ��
		if (current_time > prev_time)
		{
			time_out--;			//���³�ʱʱ��
			prev_time = current_time;
		}
		
	}
	return -2;
}


// ����AT������˻�ȡOK��ERROR����ʱ�ȣ���Ҫ��ȡ�õ���������Ϣ�����浽recv_buf
//��������Ҫ��ȡESP8266��IP��ַ
int ATDataSendandReceive(char* cmd, char* recv_buf, int time_out)
{
	g_pt_ATInterfaceDev->ClearBuffer();							//��ջ��λ�����
	g_pt_ATInterfaceDev->Write(cmd,  strlen(cmd));
	g_pt_ATInterfaceDev->Write("\r\n", 2);
	
	
	int prev_time;
	int current_time;
	prev_time = GetTime();
	
	char *line_buf = recv_buf;
	int i = 0;
	//��ȡESP8266ͨ�����ڻ������з��ص�����
	while(time_out)
	{
		if(g_pt_ATInterfaceDev->ReadByte(&line_buf[i]) == 0)	//һ���ֽ�һ���ֽڵĶ�ȡ�����������ݴ���buf
		{
			//�ж�buf������
			if((line_buf[i-1] == '\r') && (line_buf[i] == '\n'))	//����һ�е�ĩβ
			{
				if(isReturnOK(line_buf,i+1) == 0)							//��������������OK��˵��AT����ͳɹ�
				{
					line_buf[i+1] = '\0';
					return 0;
				}
				else if(isReturnERROR(line_buf,i+1) == 0)			//��������������ERROR��˵��AT�����ʧ��
				{
					line_buf[i+1] = '\0';
					return -1;
				}
			}
			i++;			//������ֽڵ����ݷ���line_buf��������ȡ��һ���ֽ�
		}
		//��ȡ���ڵ�ʱ��
		current_time = GetTime();
		
		//���ѵ�ʱ��
		if (current_time > prev_time)
		{
			time_out--;			//���³�ʱʱ��
			prev_time = current_time;
		}
		
	}
	return -2;
}



/*-----------------------------------------------------------------*/
//����Ĵ������MQTTʹ��
int ATCommandSendMQTT(char* cmd, int time_out)
{
	//printf("Send ATCommand: %s\r\n", cmd);
	
	g_pt_ATInterfaceDev->ClearBuffer();							//��ջ��λ�����
	g_pt_ATInterfaceDev->Write(cmd, strlen(cmd));
	g_pt_ATInterfaceDev->Write("\r\n", 2);
	
	int prev_time;
	int current_time;
	
	prev_time = GetTime();
	
	char line_buf[100];
	int i = 0;
	//��ȡESP8266ͨ�����ڻ������з��ص�����
	while(time_out)
	{
		if(g_pt_ATInterfaceDev->ReadByte(&line_buf[i]) == 0)	//һ���ֽ�һ���ֽڵĶ�ȡ�����������ݴ���buf
		{
			//printf("%c", line_buf[i]);
			//�ж�buf������
			if((line_buf[i-1] == '\r') && (line_buf[i] == '\n'))	//����һ�е�ĩβ
			{
				if(isReturnOK(line_buf,i+1) == 0)							//��������������OK��˵��AT����ͳɹ�
				{
					//printf(" OK\r\n");
					return 0;
				}
				else if(isReturnERROR(line_buf,i+1) == 0)			//��������������ERROR��˵��AT�����ʧ��
				{
					//printf(" ERROR\r\n");
					return -1;
				}
				else			//����������ͨ���ݵĻس����У������ǲ�ͬ���ݣ�����ʼ������ȡ��һ�У���ȡ�������ݷ���buf
				{
					i = 0;
				}
				
			}
			i++;			//������ֽڵ����ݷ���buf��������ȡ��һ���ֽ�
		}

		//��ȡ���ڵ�ʱ��
		current_time = GetTime();
		
		//���ѵ�ʱ��
		if (current_time > prev_time)
		{
			time_out--;			//���³�ʱʱ��
			prev_time = current_time;
		}
			
	}
	printf("timeout\r\n");
	return -2;
}

//-----------------------------------------------------------------------------
//���´���ΪMQTTר��


//�Ƿ񷵻�>
static int isReturnLarge(char *buf, int len)
{
	/* �ж��ַ�������û��: "ERROR\r\n" */
	buf[len] = '\0';
	return strstr(buf, ">")? 1 : 0;
}

int ATCommandSendSend(char *cmd, int time_out)
{
//	char buf[100];
	int i = 0;
//	int pre;
//	int now;

	//printf("Send ATCommand: %s\r\n", cmd);

	g_pt_ATInterfaceDev->ClearBuffer();	//���buffer
	
	g_pt_ATInterfaceDev->Write(cmd, strlen(cmd));
	g_pt_ATInterfaceDev->Write("\r\n", 2);

	int prev_time;
	int current_time;
	
	prev_time = GetTime();
	
	char line_buf[100];
	//��ȡESP8266ͨ�����ڻ������з��ص�����
	while(time_out)
	{
		if(g_pt_ATInterfaceDev->ReadByte(&line_buf[i]) == 0)	//һ���ֽ�һ���ֽڵĶ�ȡ�����������ݴ���buf
		{
			//printf("%c", line_buf[i]);
			//�ж�buf������
			if((line_buf[i-1] == '\r') && (line_buf[i] == '\n'))	//����һ�е�ĩβ
			{
				if(isReturnLarge(line_buf,i+1) == 0)							//��������������OK��˵��AT����ͳɹ�
				{
					printf("Send Data Command OK\r\n");
					return 0;
				}
				else			//����������ͨ���ݵĻس����У������ǲ�ͬ���ݣ�����ʼ������ȡ��һ�У���ȡ�������ݷ���buf
				{
					i = 0;
				}
				
			}
			i++;			//������ֽڵ����ݷ���buf��������ȡ��һ���ֽ�
		}

		//��ȡ���ڵ�ʱ��
		current_time = GetTime();
		
		//���ѵ�ʱ��
		if (current_time > prev_time)
		{
			time_out--;			//���³�ʱʱ��
			prev_time = current_time;
		}
			
	}
	printf("timeout\r\n");
	return -2;
}

int ATDataSend(unsigned char *Data, int piLen, int time_out)
{
	//��������յ�>
	char cmd[100]={0};
	sprintf(cmd, "AT+CIPSEND=%d", piLen);
	int ret  = ATCommandSendSend(cmd, time_out);
	if(ret == 0)
	{
		//˵������ͳɹ�����ʼ��������
		//��������
		g_pt_ATInterfaceDev->ClearBuffer();							//��ջ��λ�����
		g_pt_ATInterfaceDev->Write((char*)Data,  piLen);
		//g_pt_ATInterfaceDev->Write("\r\n", 2);
		
		//�������ݣ����Ƿ��յ�OK
		int prev_time;
		int current_time;
		prev_time = GetTime();
		
		char line_buf[100];
		int i = 0;
		//��ȡESP8266ͨ�����ڻ������з��ص�����
		while(time_out)
		{
			if(g_pt_ATInterfaceDev->ReadByte(&line_buf[i]) == 0)	//һ���ֽ�һ���ֽڵĶ�ȡ�����������ݴ���buf
			{
				//�ж�buf������
				if((line_buf[i-1] == '\r') && (line_buf[i] == '\n'))	//����һ�е�ĩβ
				{
					if(isReturnOK(line_buf,i+1) == 0)							//��������������OK��˵��AT����ͳɹ�
					{
						line_buf[i+1] = '\0';
						return 0;
					}
					else if(isReturnERROR(line_buf,i+1) == 0)			//��������������ERROR��˵��AT�����ʧ��
					{
						line_buf[i+1] = '\0';
						return -1;
					}
				}
				i++;			//������ֽڵ����ݷ���line_buf��������ȡ��һ���ֽ�
			}
			//��ȡ���ڵ�ʱ��
			current_time = GetTime();
			
			//���ѵ�ʱ��
			if (current_time > prev_time)
			{
				time_out--;			//���³�ʱʱ��
				prev_time = current_time;
			}
			
		}
		return -2;
	}
	else if(ret == -1)
	{
		return -1;
	}
	else
	{
		return -2;
	}
}
