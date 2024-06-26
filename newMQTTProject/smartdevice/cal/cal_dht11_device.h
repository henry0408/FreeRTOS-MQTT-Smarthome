#ifndef __CAL_DHT11_DEV_H
#define __CAL_DHT11_DEV_H

#include "dht11_device.h"

int CAL_DHT11DeviceInit(void);

int CAL_DHT11Device_GetValue(struct DHT11Dev *ptDHT11Dev);

#endif
