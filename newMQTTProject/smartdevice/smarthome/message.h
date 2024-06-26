#ifndef __MESSAGE_H
#define __MESSAGE_H

#include <stdio.h>
#include <input_system.h>

/**********************************************************************
 * 函数名称： ConvertInputEventToJson
 * 功能描述： 把输入事件InputEvent转换为Json格式的参数
 * 输入参数： ptEvent-输入事件的指针
 * 输出参数： pcJson-Json格式的参数
 * 返 回 值： 0-成功, (-1)-失败
 ***********************************************************************/
int ConvertInputEventToJson(pInputEvent ptEvent, char *pcJson);


#endif /* __MESSAGE_H */

