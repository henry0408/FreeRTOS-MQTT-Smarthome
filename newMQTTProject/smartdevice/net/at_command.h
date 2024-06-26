
#ifndef __AT_COMMAND_H
#define __AT_COMMAND_H

#include "net_system.h"

void ATInterfaceInit(void);

int ATCommandSend(char* cmd, int time_out);

int ATDataRecv(unsigned char *Data, int *piLen, int time_out);

int ATDataSendandReceive(char* command, char* recv_buf, int time_out);

int ATDataSend(unsigned char *Data, int piLen, int time_out);

#endif /* __NET_SYSTEM_H */

