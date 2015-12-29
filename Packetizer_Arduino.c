#include "Packetizer_Arduino.h"

void getSync_Arduino (int portHandle, char controlByte) {
	char buf[100];
	char tempByte, sizeByte;
	int bytesRead;
	int inSync = 0;
	
	printf("Acquiring Stream Sync...");
	fflush(stdout);
	while (!inSync) {
		readSerial(portHandle, &tempByte, 1);
		printf("%c", tempByte);
		fflush(stdout);
		if (tempByte == controlByte) {
			//next should be the size byte
			readSerial(portHandle, &sizeByte, 1);
			sizeByte -= '0';
			//now, we need to read this many bytes
			readSerial(portHandle, buf, sizeByte+2); //read plus two to throw out newline char and CR
			
			//read one more byte. If this byte is the control byte, we are probably in sync.
			readSerial(portHandle, &tempByte, 1);
			
			if (tempByte == controlByte) {
				inSync = 1;
				
				//read the rest of the packet
				readSerial(portHandle, &sizeByte, 1);
				sizeByte -= '0';
				readSerial(portHandle, buf, sizeByte+2);
			}
		}
	}

	printf("Sync locked.\n");
	fflush(stdout);
}


int readPacket_Arduino(int port, char controlByte, char *buffer) {
	
	char header[2];
	
	readSerial(port, header, 2);
	
	//if we encounter the control byte, we are still in sync
	if (header[0] == controlByte) {
		readSerial(port, buffer, header[1]-'0'+2); //get rid of CR and NL chracters
		
	} else { //else we need to reacquire stream sync
		printf("Sync lost.\n");
		getSync_Arduino(port, controlByte);
	}
	
	return header[1]-'0';
}

