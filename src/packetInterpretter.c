#include "packetInterpretter.h"

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


//This function conforms to the SUBLIBinal getSynchronization() function to establish communication in sync between the computer and the microcontroller on startup
void acquireSynchronization(int fd, char control)
{
	int sync = 0;
	char buf;
	int size;

	//Attempt to read packets from the serial port 
	//We have read the NULL packet. Keep reading packets and echoing
	while (!sync)
	{
		//Read a packet
		size = readPacket(fd, control, &buf);
		if (size == 0)
		{
			writePacket(fd, control, &buf, size);
		} 
		else if (size == 1 && buf == 1)
		{
			printf("Sending confirmation packet.\n");
			sync = 1;
			writePacket(fd, control, &buf, size);
		}
	}
}

//i. get_sync(): This function will get sync with the packetizer on the microcontroller and then return once sync has been established

void getSync(int fd, char control) {
	char controlByte;
	char sizeByte;
	char data[200];
	int inSync = 0;
	//We will continue to get a single byte at a time from the serial port until we encounter our control byte.
	//Once we have the control byte, we will then read the next byte and assume that it is the size byte. We will then read size amount of bytes and then get one more byte.
	//If the next byte is the control byte, we will read the next 
	
	printf("Acquiring synchronization...\n");
	fflush(stdout);
	while (inSync == 0) { //We have not acquired sync
		readSerial(fd, &controlByte, 1); //read a single byte at a time looking for the control byte

		if (controlByte == control) { //if we have found the control byte, assume that the next byte is the size byte
			//now, read the size byte
			readSerial(fd, &sizeByte, 1);
			

			//read the remainder of the packet into the data array
			readSerial(fd, data, sizeByte);

			//check to see if the next byte is the control byte
			readSerial(fd, &controlByte, 1);

			if (control == controlByte) {
				readSerial(fd, &sizeByte, 1); //read the next size byte
				
				//read the rest of the packet so that we remain in sync
				readSerial(fd, data, sizeByte);

				//signify that we have acquired synchronization
				inSync = 1;
			}
		}
	}

	printf("Sync acquired.");
	fflush(stdout);
}

//This function wil attempt to read a packet from the serial port
//This function will return -1 if it failed to read a packet and had to resynchronize
int readPacket(int fd, char control, char *data) {
	char sizeByte, tempByte;

	//Read the first byte and test to ensure it is the control byte. If it is not, we need to reacquire sync
	read(fd, &tempByte, 1);

	if (tempByte != control) { //if we did not just read the control byte, we have lost sync
		getSync(fd, control);
		sizeByte = -1; //return a -1 to indicate we failed a read and reacquired synchronization
	} else {
		//else we will receive a packet as normal
		//read the size byte
		readSerial(fd, &sizeByte, 1);
		readSerial(fd, data, sizeByte);	
	}

	return sizeByte;
}

int writePacket(int fd, char control, char *data, int size) {
	//we will create a packet to send on the serial channel
	char packet[255];
	int status;
	packet[0] = control;
	packet[1] = size;

	memcpy(&(packet[2]), data, size);
	
	status = writeSerial(fd, packet, size+2);
	
	return status;

}
