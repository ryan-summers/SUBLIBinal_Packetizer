#ifndef PACKET_INT_H
#define PACKET_INT_H

#include "serial.h"
#include <string.h>

//function prototypes
void getSync(int fd, char control);
int readPacket(int fd, char control, char *data);
int writePacket(int fd, char control, char *data, int byteCount);

#endif
