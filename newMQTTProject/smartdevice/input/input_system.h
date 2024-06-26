#ifndef __INPUT_SYSTEM_
#define __INPUT_SYSTEM_

#define INPUT_BUF_LEN 50

//һ��enum���ڴ���input����
typedef enum
{
	INPUT_EVENT_KEY,
	INPUT_EVENT_NET
}INPUT_EVENT_TYPE;

typedef struct InputEvent
{
	INPUT_EVENT_TYPE type;		//��������
	int iKey;									//�ĸ�����
	int iPressure;						//��������
	char str[INPUT_BUF_LEN];	//�������볤��
	
}InputEvent, *pInputEvent;


typedef struct InputDevice
{
	char name[10];														//�豸����
	int (*Init)();														//��ʼ���豸
	int (*GetEvent)(pInputEvent event);				//��ȡ�����¼�	
	struct InputDevice* pNext;								//ָ��ָ��������һ���ڵ�
	
}InputDevice, *pInputDevice;

void InputDeviceRegister(pInputDevice ptInputDevice);
void AddInputDevices(void);
int InitInputDevices(void);


#endif
