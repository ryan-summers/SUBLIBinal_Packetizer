#ifndef SERIAL_H
#define SERIAL_H

#include <fcntl.h>
#include <stdio.h>

//function prototypes
int configureSerial(int portHandle, int baudRate);
int readSerial(int portHandle, char *data, int byteCount);
int writeSerial(int portHandle, char *data, int byteCount);
int openSerial(char *portName, int baudRate);
void closeSerial(int portHandle);

#endif
