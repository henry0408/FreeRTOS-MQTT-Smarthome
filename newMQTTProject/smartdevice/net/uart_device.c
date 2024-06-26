#include "uart_device.h"
#include "driver_usart.h"


//typedef struct UART_Device
//{
//	char* name;
//	int (*Init)();
//	int (*Write)(char* buffer, int len);
//	int (*ReadByte)(char* buffer);
//	void (*ClearBuffer)();
//}UART_Device, *pUART_Device;

//���廷�λ�����
ring_buffer uart3_buffer;

//��ȡUART3�Ļ��λ�����
ring_buffer* GetUART3RingBuffer()
{
	return &uart3_buffer;
}

int UART3Init()
{
	
	ring_buffer_init(&uart3_buffer);
	return 0;
}


int UART3Write(char* buffer, int len)
{
	USART3_SendBytes(buffer, len);
	return 0;
}
	

int UART3ReadByte(char *Data)
{
	return ring_buffer_read((unsigned char *)Data,  &uart3_buffer);
}
	
//���UART3�Ļ��λ�����
void UART3ClearBuffer()
{
	uart3_buffer.pR = uart3_buffer.pW;
}


struct UART_Device uart ={
	"uart3",
	UART3Init,
	UART3Write,
	UART3ReadByte,
	UART3ClearBuffer
};

pUART_Device GetATInterfaceDevice()
{
	return &uart;
}
