#ifndef __DRIVER_LED_H
#define __DRIVER_LED_H

#include <led_device.h>

/*
 *  函数名：int HAL_LEDDeviceControl(struct LEDDevice *ptLEDDevice, int iStatus)
 *  输入参数：ptLEDDevice-哪个LED
 *  输入参数：iStatus-LED状态, 1-亮, 0-灭
 *  输出参数：无
 *  返回值：0-成功, -1: 失败
 */
int HAL_LEDDeviceControl(struct LEDDevice *ptLEDDevice, int iStatus);

#endif /* __DRIVER_LED_H */
