#include "cal_dht11_device.h"
#include "driver_dht11.h"

int CAL_DHT11DeviceInit()
{
	DHT11_Init();
	return 0;
}

int CAL_DHT11Device_GetValue(struct DHT11Dev *ptDHT11Dev)
{
    return DHT11_Read(&ptDHT11Dev->value.temperature, &ptDHT11Dev->value.humidity); //��ΪDHT11_Read�Ĳ�����int*���ͣ�Ҫ��intת��Ϊ&int
}
