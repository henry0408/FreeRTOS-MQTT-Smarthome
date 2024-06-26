#ifndef __KAL_DHT11_DEV_H
#define __KAL_DHT11_DEV_H

#include "dht11_device.h"

int KAL_DHT11DeviceInit(void);

int KAL_DHT11Device_GetValue(struct DHT11Dev *ptDHT11Dev);

#endif

