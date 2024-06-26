#ifndef __INPUT_BUFFER_
#define __INPUT_BUFFER_

#include "input_system.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define EVENT_BUFFER_SIZE 10        /* ���λ������Ĵ�С */
typedef struct
{
    InputEvent buffer[EVENT_BUFFER_SIZE];  /* �������ռ� */
    volatile unsigned int pW;           /* д��ַ */
    volatile unsigned int pR;           /* ����ַ */
} InputEventBuffer;

int InitInputQueue(void);
 
int GetInputEvent(pInputEvent ptInputEvent);		//��ȡ�����¼���buffer

int PutInputEvent(pInputEvent ptInputEvent);		//��������¼���buffer

#endif
