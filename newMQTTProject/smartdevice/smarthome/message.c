#include <stdio.h>
#include <string.h>
#include <input_buffer.h>
#include <input_system.h>
#include <led_device.h>
#include <fan_device.h>
#include <display_system.h>
#include <font_system.h>
#include <show_text.h>
#include <net_system.h>
#include <gpio_input.h>


/**********************************************************************
 * 函数名称： ConvertInputEventToJson
 * 功能描述： 把输入事件InputEvent转换为Json格式的参数
 * 输入参数： ptEvent-输入事件的指针
 * 输出参数： pcJson-Json格式的参数
 * 返 回 值： 0-成功, (-1)-失败
 
 Json格式：{"dev":"lamp1","status":"1"}	//灯1开
 {"dev":"lamp1","status":"0"}	//灯1关
  {"dev":"lamp1","status":"2"}	//灯1翻转状态
 
{"dev":"fan","status":"1"}	//风扇顺时针
{"dev":"fan","status":"-1"}	//风扇逆时针
{"dev":"fan","status":"1"}	//风扇停

{"dev":"home","status":"0"}	//回家模式：3个LED和风扇都打开
{"dev":"home","status":"1"}	//离家模式：3个LED和风扇都打开
 ***********************************************************************/

int ConvertInputEventToJson(pInputEvent ptEvent, char *pcJson)
{
	if(ptEvent->type == INPUT_EVENT_KEY)
	{
		if(ptEvent->iPressure)
		{
			return -1;
		}
		else
		{
			if(ptEvent->iKey == K1_CODE)
			{
				strcpy(pcJson,"{\"dev\":\"lamp1\",\"status\":\"2\"}");
				return 0;
			}
			else if(ptEvent->iKey == K2_CODE)
			{
				strcpy(pcJson,"{\"dev\":\"lamp2\",\"status\":\"2\"}");
				return 0;
			}
			else
			{
				return -1;
			}
		}
	}
	else if(ptEvent->type == INPUT_EVENT_NET)
	{
		strcpy(pcJson, ptEvent->str);
		return 0;
	}
	return -1;
}
