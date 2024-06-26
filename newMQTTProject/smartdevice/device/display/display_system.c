#include "display_system.h"
#include "oled_device.h"

void AddDisplayDevices()
{
	AddDisplayDeviceOLED();
}

PDisplayDevice GetDisplayDevice(char *name)
{
	return __GetDisplayDevice(name);
}

