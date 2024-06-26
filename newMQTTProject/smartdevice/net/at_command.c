#include "at_command.h"
#include "uart_device.h"
#include <string.h>
#include "time.h"
#include <stdlib.h>
#include <stdio.h>

static pUART_Device g_pt_ATInterfaceDev;		//串口设备

void ATInterfaceInit(void)
{
	//获得串口/I2C设备
	g_pt_ATInterfaceDev = GetATInterfaceDevice();
	
	//串口设备初始化
	g_pt_ATInterfaceDev->Init();
	
}

//在buf从开始到len的范围内，有没有'OK\r\n'
int isReturnOK(char* buf, int len)	
{
	buf[len] = '\0';	//截取buf，变为string
	if( strstr(buf, "OK\r\n") )
	{
		return 0;	//如果有，返回0
	}
	return -1;	//没有，返回-1
}


int isReturnERROR(char* buf, int len)		//在buf从开始到len的范围内，有没有'ERROR\r\n'
{
	buf[len] = '\0';	//截取buf，变为string
	if( strstr(buf, "ERROR\r\n") )
	{
		return 0;	//如果有，返回0
	}
	return -1;	//没有，返回-1
	
}


//通过串口向ESP8266发送指令后，还要判断在超时时间内是否收到OK还是ERROR
int ATCommandSend(char* cmd, int time_out)
{
	//printf("Send ATCommand: %s\r\n", cmd);
	
	g_pt_ATInterfaceDev->ClearBuffer();							//清空环形缓冲区
	g_pt_ATInterfaceDev->Write(cmd, strlen(cmd));
	g_pt_ATInterfaceDev->Write("\r\n", 2);
	
	int prev_time;
	int current_time;
	
	prev_time = GetTime();
	
	char line_buf[100];
	int i = 0;
	//读取ESP8266通过串口缓冲区中返回的数据
	while(time_out)
	{
		if(g_pt_ATInterfaceDev->ReadByte(&line_buf[i]) == 0)	//一个字节一个字节的读取，读到的数据存入buf
		{
			//printf("%c", line_buf[i]);
			//判断buf的内容
			if((line_buf[i-1] == '\r') && (line_buf[i] == '\n'))	//读到一行的末尾
			{
				if(isReturnOK(line_buf,i+1) == 0)							//如果该行里面读到OK，说明AT命令发送成功
				{
					//printf(" OK\r\n");
					return 0;
				}
				else if(isReturnERROR(line_buf,i+1) == 0)			//如果该行里面读到ERROR，说明AT命令发送失败
				{
					//printf(" ERROR\r\n");
					return -1;
				}
				else			//读到的是普通数据的回车换行（该行是不同数据），则开始继续读取下一行，读取到的数据放入buf
				{
					i = 0;
				}
				
			}
			i++;			//把这个字节的数据放入buf，继续读取下一个字节
		}

		//获取现在的时间
		current_time = GetTime();
		
		//花费的时间
		if (current_time > prev_time)
		{
			time_out--;			//更新超时时间
			prev_time = current_time;
		}
			
	}
	printf("timeout\r\n");
	return -2;
}

//PC向ESP8266发送网络数据，ESP8266接收网络数据，例如应该会在串口显示+IPD,n:xxxxxxxxxx // received n bytes, data=xxxxxxxxxxx	
int ATDataRecv(unsigned char *Data, int *piLen, int time_out)
{
	*piLen = 0;
	//+IPD,n:xxxxxxxxxx，我们分成3个部分，利用状态机
	enum STATUS
	{
		STATUS_INIT,
		STATUS_LEN,
		STATUS_DATA
	};
	
	enum STATUS current_status = STATUS_INIT;		//定义初始状态
	
	int prev_time;
	int current_time;
	prev_time = GetTime();
	
	char line_buf[100];
	int i = 0;
	//读取ESP8266通过串口缓冲区中返回的数据
	while(time_out)
	{
		if(g_pt_ATInterfaceDev->ReadByte(&line_buf[i]) == 0)		//一个字节一个字节的读取，读到的数据存入buf
		{
			switch(current_status)
			{
				case STATUS_INIT:
				{
					line_buf[i+1] = '\0';		//截断
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
					if(line_buf[i] == ':')		//截断
					{
						//算出数据长度
						line_buf[i] = '\0';
						//printf("line buf is %s\r\n",line_buf);
						*piLen = atoi(line_buf);
						//printf("piLen is %d\r\n",*piLen);
						current_status = STATUS_DATA;
						i = 0;
					}
					else
					{
						i++;			//读取下一个字节
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
		
		//获取现在的时间
		current_time = GetTime();
		
		//花费的时间
		if (current_time > prev_time)
		{
			time_out--;			//更新超时时间
			prev_time = current_time;
		}
		
	}
	return -2;
}


// 发送AT命令，除了获取OK，ERROR、超时等，还要获取得到的数据信息，保存到recv_buf
//比如我们要获取ESP8266的IP地址
int ATDataSendandReceive(char* cmd, char* recv_buf, int time_out)
{
	g_pt_ATInterfaceDev->ClearBuffer();							//清空环形缓冲区
	g_pt_ATInterfaceDev->Write(cmd,  strlen(cmd));
	g_pt_ATInterfaceDev->Write("\r\n", 2);
	
	
	int prev_time;
	int current_time;
	prev_time = GetTime();
	
	char *line_buf = recv_buf;
	int i = 0;
	//读取ESP8266通过串口缓冲区中返回的数据
	while(time_out)
	{
		if(g_pt_ATInterfaceDev->ReadByte(&line_buf[i]) == 0)	//一个字节一个字节的读取，读到的数据存入buf
		{
			//判断buf的内容
			if((line_buf[i-1] == '\r') && (line_buf[i] == '\n'))	//读到一行的末尾
			{
				if(isReturnOK(line_buf,i+1) == 0)							//如果该行里面读到OK，说明AT命令发送成功
				{
					line_buf[i+1] = '\0';
					return 0;
				}
				else if(isReturnERROR(line_buf,i+1) == 0)			//如果该行里面读到ERROR，说明AT命令发送失败
				{
					line_buf[i+1] = '\0';
					return -1;
				}
			}
			i++;			//把这个字节的数据放入line_buf，继续读取下一个字节
		}
		//获取现在的时间
		current_time = GetTime();
		
		//花费的时间
		if (current_time > prev_time)
		{
			time_out--;			//更新超时时间
			prev_time = current_time;
		}
		
	}
	return -2;
}



/*-----------------------------------------------------------------*/
//下面的代码仅供MQTT使用
int ATCommandSendMQTT(char* cmd, int time_out)
{
	//printf("Send ATCommand: %s\r\n", cmd);
	
	g_pt_ATInterfaceDev->ClearBuffer();							//清空环形缓冲区
	g_pt_ATInterfaceDev->Write(cmd, strlen(cmd));
	g_pt_ATInterfaceDev->Write("\r\n", 2);
	
	int prev_time;
	int current_time;
	
	prev_time = GetTime();
	
	char line_buf[100];
	int i = 0;
	//读取ESP8266通过串口缓冲区中返回的数据
	while(time_out)
	{
		if(g_pt_ATInterfaceDev->ReadByte(&line_buf[i]) == 0)	//一个字节一个字节的读取，读到的数据存入buf
		{
			//printf("%c", line_buf[i]);
			//判断buf的内容
			if((line_buf[i-1] == '\r') && (line_buf[i] == '\n'))	//读到一行的末尾
			{
				if(isReturnOK(line_buf,i+1) == 0)							//如果该行里面读到OK，说明AT命令发送成功
				{
					//printf(" OK\r\n");
					return 0;
				}
				else if(isReturnERROR(line_buf,i+1) == 0)			//如果该行里面读到ERROR，说明AT命令发送失败
				{
					//printf(" ERROR\r\n");
					return -1;
				}
				else			//读到的是普通数据的回车换行（该行是不同数据），则开始继续读取下一行，读取到的数据放入buf
				{
					i = 0;
				}
				
			}
			i++;			//把这个字节的数据放入buf，继续读取下一个字节
		}

		//获取现在的时间
		current_time = GetTime();
		
		//花费的时间
		if (current_time > prev_time)
		{
			time_out--;			//更新超时时间
			prev_time = current_time;
		}
			
	}
	printf("timeout\r\n");
	return -2;
}

//-----------------------------------------------------------------------------
//以下代码为MQTT专用


//是否返回>
static int isReturnLarge(char *buf, int len)
{
	/* 判断字符串里有没有: "ERROR\r\n" */
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

	g_pt_ATInterfaceDev->ClearBuffer();	//清空buffer
	
	g_pt_ATInterfaceDev->Write(cmd, strlen(cmd));
	g_pt_ATInterfaceDev->Write("\r\n", 2);

	int prev_time;
	int current_time;
	
	prev_time = GetTime();
	
	char line_buf[100];
	//读取ESP8266通过串口缓冲区中返回的数据
	while(time_out)
	{
		if(g_pt_ATInterfaceDev->ReadByte(&line_buf[i]) == 0)	//一个字节一个字节的读取，读到的数据存入buf
		{
			//printf("%c", line_buf[i]);
			//判断buf的内容
			if((line_buf[i-1] == '\r') && (line_buf[i] == '\n'))	//读到一行的末尾
			{
				if(isReturnLarge(line_buf,i+1) == 0)							//如果该行里面读到OK，说明AT命令发送成功
				{
					printf("Send Data Command OK\r\n");
					return 0;
				}
				else			//读到的是普通数据的回车换行（该行是不同数据），则开始继续读取下一行，读取到的数据放入buf
				{
					i = 0;
				}
				
			}
			i++;			//把这个字节的数据放入buf，继续读取下一个字节
		}

		//获取现在的时间
		current_time = GetTime();
		
		//花费的时间
		if (current_time > prev_time)
		{
			time_out--;			//更新超时时间
			prev_time = current_time;
		}
			
	}
	printf("timeout\r\n");
	return -2;
}

int ATDataSend(unsigned char *Data, int piLen, int time_out)
{
	//发送命令，收到>
	char cmd[100]={0};
	sprintf(cmd, "AT+CIPSEND=%d", piLen);
	int ret  = ATCommandSendSend(cmd, time_out);
	if(ret == 0)
	{
		//说明命令发送成功，开始发送数据
		//发送数据
		g_pt_ATInterfaceDev->ClearBuffer();							//清空环形缓冲区
		g_pt_ATInterfaceDev->Write((char*)Data,  piLen);
		//g_pt_ATInterfaceDev->Write("\r\n", 2);
		
		//解析数据，看是否收到OK
		int prev_time;
		int current_time;
		prev_time = GetTime();
		
		char line_buf[100];
		int i = 0;
		//读取ESP8266通过串口缓冲区中返回的数据
		while(time_out)
		{
			if(g_pt_ATInterfaceDev->ReadByte(&line_buf[i]) == 0)	//一个字节一个字节的读取，读到的数据存入buf
			{
				//判断buf的内容
				if((line_buf[i-1] == '\r') && (line_buf[i] == '\n'))	//读到一行的末尾
				{
					if(isReturnOK(line_buf,i+1) == 0)							//如果该行里面读到OK，说明AT命令发送成功
					{
						line_buf[i+1] = '\0';
						return 0;
					}
					else if(isReturnERROR(line_buf,i+1) == 0)			//如果该行里面读到ERROR，说明AT命令发送失败
					{
						line_buf[i+1] = '\0';
						return -1;
					}
				}
				i++;			//把这个字节的数据放入line_buf，继续读取下一个字节
			}
			//获取现在的时间
			current_time = GetTime();
			
			//花费的时间
			if (current_time > prev_time)
			{
				time_out--;			//更新超时时间
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
