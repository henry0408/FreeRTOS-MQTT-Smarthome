#include "kal_dht11_device.h"
#include "cal_dht11_device.h"

int KAL_DHT11DeviceInit()
{
	return CAL_DHT11DeviceInit();
}

int KAL_DHT11Device_GetValue(struct DHT11Dev *ptDHT11Dev)
{
	return CAL_DHT11Device_GetValue(ptDHT11Dev);
}
