#ifndef PACKET_INT_H
#define PACKET_INT_H

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
#include <string.h>

//function prototypes
void acquireSynchronization(int fd, char control);

void getSync(int fd, char control);
int readPacket(int fd, char control, char *data);
int writePacket(int fd, char control, char *data, int byteCount);

#endif
