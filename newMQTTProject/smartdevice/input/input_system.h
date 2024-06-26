#ifndef __INPUT_SYSTEM_
#define __INPUT_SYSTEM_

#define INPUT_BUF_LEN 50

//一个enum用于储存input类型
typedef enum
{
	INPUT_EVENT_KEY,
	INPUT_EVENT_NET
}INPUT_EVENT_TYPE;

typedef struct InputEvent
{
	INPUT_EVENT_TYPE type;		//输入类型
	int iKey;									//哪个按键
	int iPressure;						//按键按下
	char str[INPUT_BUF_LEN];	//网络输入长度
	
}InputEvent, *pInputEvent;


typedef struct InputDevice
{
	char name[10];														//设备名称
	int (*Init)();														//初始化设备
	int (*GetEvent)(pInputEvent event);				//获取输入事件	
	struct InputDevice* pNext;								//指针指向链表下一个节点
	
}InputDevice, *pInputDevice;

void InputDeviceRegister(pInputDevice ptInputDevice);
void AddInputDevices(void);
int InitInputDevices(void);


#endif
