#include "input_buffer.h"
#include "config.h"
#include <stdio.h>

#if CONFIG_USE_FREERTOS

//用队列代替缓冲区
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

//写队列
int PutInputEvent(pInputEvent ptInputEvent)
{
	xQueueSendFromISR(g_xQueueInput,ptInputEvent,NULL);
	return 0;
}

//读队列
int GetInputEvent(pInputEvent ptInputEvent)
{
	xQueueReceive(g_xQueueInput, ptInputEvent, portMAX_DELAY);
	return 0;
}


#else
static InputEventBuffer g_tInputBuffer;

/* 获取输入事件从buffer */
int GetInputEvent(pInputEvent ptInputEvent)
{
	/* 防御式编程 */
	if (!ptInputEvent)
		return -1;

	/* 环形缓冲区空, 则返回-1 */
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


/* 添加输入事件到buffer */
int PutInputEvent(pInputEvent ptInputEvent)
{
	int i = (g_tInputBuffer.pW + 1) % EVENT_BUFFER_SIZE;

	/* 防御式编程 */
	if (!ptInputEvent)
		return -1;
	
	if(i != g_tInputBuffer.pR)    // 环形缓冲区没有写满
	{
			g_tInputBuffer.buffer[g_tInputBuffer.pW] = *ptInputEvent;
			g_tInputBuffer.pW = i;
			return 0;
	}
	return -1;
}

#endif

