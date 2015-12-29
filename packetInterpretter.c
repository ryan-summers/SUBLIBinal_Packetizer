// This file will take in a serial port and do the following:
//i. get_sync(): This function will get sync with the packetizer on the microcontroller and then return once sync has been established
//ii. get_packet(): This function will receive a packet from the microcontroller and interpret it. It will then interpret the data and hand back a pointer to a malloc'd size of data.
//	The function will return the size of the packet. It will also modify the input pointer to point at dynamically allocated memory. This memory should be free'd once the user is
// 	done using it.
#include "packetInterpretter.h"

void getSync(int fd, char control) { //The only inputs to this function is a valid file descriptor object and the control byte. This object should be a read object for it to properly do a blocking port read
	char controlByte;
	char sizeByte;
	char data[200];
	int inSync = 0;
	//We will continue to get a single byte at a time from the serial port until we encounter our control byte.
	//Once we have the control byte, we will then read the next byte and assume that it is the size byte. We will then read size amount of bytes and then get one more byte.
		//If the next byte is the control byte, we will read the next 
	
	printf("Acquiring synchronization...\n");
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
	fflush(NULL);
}

//This function wil attempt to read a packet from the serial port
//This function will return 0 if it failed to read a packet and had to resynchronize
int readPacket(int fd, char control, char *data) {
	char sizeByte, tempByte;

	//Read the first byte and test to ensure it is the control byte. If it is not, we need to reacquire sync
	read(fd, &tempByte, 1);

	if (tempByte != control) { //if we did not just read the control byte, we have lost sync
		getSync(fd, control);
		sizeByte = 0; //return a 0 to indicate we failed a read and reacquired synchronization
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
