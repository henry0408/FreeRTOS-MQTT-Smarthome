#ifndef __SMART_HOME_H
#define __SMART_HOME_H

#ifndef NULL
#define NULL (void *)0
#endif

/* 你做实验时需要修改这2项: WIFI热点的名称和密码 */
#define SSID        "HUAWEI_2C71"
#define PASSWORD    "Chy08042000"

//默认用1234号端口与PC相连
#define LOCAL_PORT  1234


//智能家居任务函数
void SmartHomeTask(void);

//DHT11更新数值函数
void DHT11_UpdateValue(void);

//ESP8266获取SNTP服务器时间函数
void DisplayNetTimeInfo(void);


#endif /* __SMART_HOME_H */

