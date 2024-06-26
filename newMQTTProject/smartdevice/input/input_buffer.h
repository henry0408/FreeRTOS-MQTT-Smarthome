#ifndef __INPUT_BUFFER_
#define __INPUT_BUFFER_

#include "input_system.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define EVENT_BUFFER_SIZE 10        /* 环形缓冲区的大小 */
typedef struct
{
    InputEvent buffer[EVENT_BUFFER_SIZE];  /* 缓冲区空间 */
    volatile unsigned int pW;           /* 写地址 */
    volatile unsigned int pR;           /* 读地址 */
} InputEventBuffer;

int InitInputQueue(void);
 
int GetInputEvent(pInputEvent ptInputEvent);		//获取输入事件从buffer

int PutInputEvent(pInputEvent ptInputEvent);		//添加输入事件到buffer

#endif
