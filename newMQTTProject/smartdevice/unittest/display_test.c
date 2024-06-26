
//#include <stdio.h>
//#include <string.h>
//#include "display_system.h"

//void display_test(void)
//{
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

//	/* 3. 画线 */
//	int x, y;
//	y = ptDevice->iYres / 2;
//	for (x = 0; x < ptDevice->iXres; x++)
//		ptDevice->SetPixel(ptDevice, x, y, 1);

//	x = ptDevice->iXres / 2;
//	for (y = 0; y < ptDevice->iYres; y++)
//		ptDevice->SetPixel(ptDevice, x, y, 1);


//	/* 4. Flush */
//	ptDevice->Flush(ptDevice);
//}

