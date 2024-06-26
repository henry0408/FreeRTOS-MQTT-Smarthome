
#include <input_system.h>
#include <input_buffer.h>
#include <stdio.h>


/**********************************************************************
 * 函数名称： input_test
 * 功能描述： 输入系统单元测试函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void input_test(void)
{
	InputEvent event;
	
	AddInputDevices();
	InitInputDevices();
	
	while (1)
	{
		if ( GetInputEvent(&event) == 0)
		{
			printf("get input event:\r\n");
			printf("type: %d\r\n", event.type);
			//printf("time: %d\r\n", event.time);
			printf("key : %d\r\n", event.iKey);
			printf("pressure : %d\r\n", event.iPressure);
		}
		
	}
}

