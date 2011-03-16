/****************************************************************************
 *   hserver
 *   Copyright (C) 2007  
 *   Arwid Bancewicz <arwid@arwid.ca>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef __SOCKET_H__
#define __SOCKET_H__
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// This class is a wrapper so that a traditional BSD socket appears as a
// bonified C++ class/object.  Helps maintain a uniform object-oriented
// abstraction.
class Socket {
public:
	Socket();
	Socket(int _sockFD);
	virtual ~Socket();

	char *address;

	virtual int Listen(int port);
		// Listen(port) instructs the OS to start accepting new connections on
		// the specified port.
		// Returns : -1 if any error occurs, 0 otherwise
		// This method will also bind the socket to local address and port.

	virtual Socket* Accept();
		// Accept() waits on a socket for a new incoming connection.
		// On a new connection, a Socket object is instantiated to allow
		// communication over the connection.
		// Returns NULL if any error occurs.

	virtual int Read(void *buf, int size);
		// Acquire data from socket.
		// Read up to <size> bytes of data and place it into <buf>.
		// Returns number of actual bytes read.
		// Returns -1 if error.

	virtual int Write(void *buf, int size);
		// Send data to socket.
		// Write up to <size> bytes of data from <buf>.
		// Returns number of actual bytes written.
		// Returns -1 if error.

	void Close() ;

protected:
	int sockFD;
		// Holds file descriptor of socket.

};
#endif // __SOCKET_H__
