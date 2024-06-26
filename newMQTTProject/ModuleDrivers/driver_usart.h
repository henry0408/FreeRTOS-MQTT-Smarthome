#ifndef __DRIVER_USART_H
#define __DRIVER_USART_H

#include "stm32f1xx_hal.h"

/*
 *  函数名：EnableDebugIRQ
 *  功能描述：使能USART1的中断
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
*/
extern void EnableDebugIRQ(void);

/*
 *  函数名：DisableDebugIRQ
 *  功能描述：失能USART1的中断
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
*/
extern void DisableDebugIRQ(void);


//使能USART3中断
void EnableUART3IRQ(void);

//USART3发多个字节的数据
void USART3_SendBytes(char *buf, int len);


#endif /* __DRIVER_USART_H */
