//#include <stdio.h>
//#include <string.h>
//#include <display_system.h>
//#include <font_system.h>
//#include <show_text.h>


///**********************************************************************
// * 函数名称： text_test
// * 功能描述： 文本系统单元测试函数
// * 输入参数： 无
// * 输出参数： 无
// * 返 回 值： 无
// * 修改日期       版本号     修改人	      修改内容
// * -----------------------------------------------
// * 2021/10/13	     V1.0	  韦东山	      创建
// ***********************************************************************/
//void text_test(void)
//{
//	AddFontLibs();
//	char *str = "www.100ask.net";
//	AddDisplayDevices();
//	
//	pDisplayDevice ptDevice;
//	char *name = "OLED";
//	ptDevice = GetDisplayDevice(name);

//	if (!ptDevice)
//	{
//		printf("Can not get DisplayDevice %s\r\n", name);
//		return;
//	}

//	/* 1. 初始化设备 */
//	ptDevice->Init(ptDevice);

//	/* 2. 清除屏幕 */
//	memset(ptDevice->FBBase, 0, ptDevice->iSize);

//	ShowTextInDisplayDevice(ptDevice,16,16,str);
//	
//}



