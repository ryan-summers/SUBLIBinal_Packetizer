
/*
	SUBLIBinal_Packetizer - GNU GPLv2 Notice
    Copyright (C) 2015  Ryan Summers and James Irwin

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "serial.h"
#include <errno.h>
#include <termios.h>
#include <unistd.h>

//This function accepts a file descriptor supplied in portHandle and a baudRate
//The port handle should be open in the proper mode before configuration is executed. 
//This function will fongiure the serial port for use

int configureSerial(int portHandle, int baudRate)
{
	struct termios portConfig = {0}; //initialize our termios structure

	if (tcgetattr(portHandle, &portConfig) != 0) {
		printf("Get Attributes failed.\n Error: %d", errno);
		return -1;
	}
	
	//manually configure the port flags
	portConfig.c_oflag = 0; //do not filter the output of the serial port
	portConfig.c_lflag = 0; //Do not use signalling characters

	portConfig.c_cflag |= (portConfig.c_cflag & ~CSIZE) | CS8; //Set 8 bit
	portConfig.c_cflag |= CREAD | CLOCAL; //8 bit, enable receiver, ignore modem controls	
	portConfig.c_cflag &= ~(PARENB | PARODD); //Disable parity
	portConfig.c_cflag &= ~(CSTOPB); //Set 1 stop bit
	portConfig.c_cflag &= ~(CRTSCTS); //Disable RTS CTS flow control

	portConfig.c_iflag |= IGNBRK | IGNPAR | IGNCR; //ignore break, parity errors, and carriage returns
	portConfig.c_iflag &= ~(IXON | IXOFF | IXANY);

	cfsetospeed(&portConfig, baudRate);
	cfsetispeed(&portConfig, baudRate);

	portConfig.c_cc[VMIN] = 255; //the port will not return until timeout occurs or bytes requested are red OR 255 (whichever is less)
	portConfig.c_cc[VTIME] = 5; //the port has .5 second timeout after first byte

	if (tcsetattr(portHandle, TCSANOW,  &portConfig) != 0)
	{	
		printf("Failed to set port attributes. Error: %d\n", errno);
		return -1;		
	}
	
	return 0; //successful completion
}

//This function will open and properly configure a serial port
//It wil then return a handle to that serial port
int openSerial(char *portName, int baudRate) 
{
	int portHandle = 0;

	portHandle = open(portName, O_RDWR | O_NOCTTY | O_SYNC); //open the file for Read-write permissions and do not hand it control as a serial device (it is a slave)	

	if (portHandle == -1) 
	{
		printf("Failed to open serial port. Error: %d", errno);
		return -1;
	}

	if (configureSerial(portHandle, baudRate) != 0) 
	{
		printf("Failed to configure the serial port. \n");
		return -1;
	}

	//flush data from the port FD
//	tcflush(portHandle, TCIOFLUSH);

	return portHandle;
}

//This function will attempt to read from the serial port. Currently, the call will not fail unless byteCount >255
int readSerial(int portHandle, char *data, int byteCount) {
	int bytesRead = read(portHandle, data, byteCount);
	return bytesRead;	
}

//Write to a serial port
//This function will return the number of bytes successfully written
int writeSerial(int portHandle, char *data, int byteCount) {
	
	int bytesWritten = write(portHandle, data, byteCount);
	return bytesWritten;
}

void closeSerial(int portHandle) {
	if (close(portHandle) != 0) 
	{
		printf("Error closing serial port. Error: %d\n", errno);
	}
}
