//PAcketizer Arduino Header

#ifndef PACKETIZER_ARDUINO
#define PACKETIZER_ARDUINO

#include <stdio.h>

void getSync_Arduino (int portHandle, char controlByte);
int readPacket_Arduino(int port, char controlByte, char *buffer);

#endif
