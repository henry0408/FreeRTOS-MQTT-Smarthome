#include "input_buffer.h"
#include "config.h"
#include <stdio.h>

#if CONFIG_USE_FREERTOS

//�ö��д��滺����
static QueueHandle_t g_xQueueInput;

int InitInputQueue(void)
{
	g_xQueueInput = xQueueCreate(10, sizeof(InputEvent));
	if (g_xQueueInput == NULL)
	{
		printf("can not create queue\r\n");
		return -1;
	}
	return 0;
}

//д����
int PutInputEvent(pInputEvent ptInputEvent)
{
	xQueueSendFromISR(g_xQueueInput,ptInputEvent,NULL);
	return 0;
}

//������
int GetInputEvent(pInputEvent ptInputEvent)
{
	xQueueReceive(g_xQueueInput, ptInputEvent, portMAX_DELAY);
	return 0;
}


#else
static InputEventBuffer g_tInputBuffer;

/* ��ȡ�����¼���buffer */
int GetInputEvent(pInputEvent ptInputEvent)
{
	/* ����ʽ��� */
	if (!ptInputEvent)
		return -1;

	/* ���λ�������, �򷵻�-1 */
    if(g_tInputBuffer.pR == g_tInputBuffer.pW)
    {
        return -1;
    }
    else
    {
        *ptInputEvent = g_tInputBuffer.buffer[g_tInputBuffer.pR];
        g_tInputBuffer.pR = (g_tInputBuffer.pR + 1) % EVENT_BUFFER_SIZE;
        return 0;
    }
}


/* ��������¼���buffer */
int PutInputEvent(pInputEvent ptInputEvent)
{
	int i = (g_tInputBuffer.pW + 1) % EVENT_BUFFER_SIZE;

	/* ����ʽ��� */
	if (!ptInputEvent)
		return -1;
	
	if(i != g_tInputBuffer.pR)    // ���λ�����û��д��
	{
			g_tInputBuffer.buffer[g_tInputBuffer.pW] = *ptInputEvent;
			g_tInputBuffer.pW = i;
			return 0;
	}
	return -1;
}

#endif

