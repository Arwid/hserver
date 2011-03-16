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
#include <iostream>
#include <tiuser.h>

#include "Socket.H"
using namespace std;

#define BACK_LOG 5

Socket::Socket() {
	sockFD = -1;
}

Socket::Socket(int _sockFD) {
	sockFD = _sockFD;
}

Socket::~Socket() {
	close(sockFD);
}

void Socket::Close() {
	close(sockFD);
}


// Listen(port) instructs the OS to start accepting new connections on
// the specified port.
// Returns : -1 if any error occurs, 0 otherwise           
// This method will also bind the socket to local address and port.
//
// Note: this code is Unix specific
//

int Socket::Listen(int port) {
	struct sockaddr_in servAddr ; // structure used to pass params to OS
	int listenPort = port ;

	// first open a Unix Socket
	sockFD = socket( AF_INET, SOCK_STREAM, 0 ) ;
	if( sockFD < 0 ) {
		cerr << "Socket::Listen: cannot get a socket" << endl ;
		return -1 ;
	} 
	else {
		int v = 1 ;
		setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(v));
	}

	servAddr.sin_family = AF_INET;           // use Internet Protocol
	servAddr.sin_addr.s_addr = INADDR_ANY;   // system provides IP address
	servAddr.sin_port = htons(listenPort);   // use Port passed in as param

	if(bind( sockFD, (struct sockaddr *) &servAddr, sizeof(servAddr) ) < 0) {
    		cerr << "ServerSocket::Listen: can't bind the socket to port " << port << endl;
    		return -1 ;
  	}

  	if( listen( sockFD, BACK_LOG ) ) {
    		cerr << "Socket::Listen: cannot listen on sockFD " << sockFD << endl;
    		return -1;
  	}

	return 0 ;
}


// Accept() waits on a socket for a new incoming connection.
// On a new connection, a Socket object is instantiated to allow
// communication over the connection.
// Returns NULL if any error occurs.

Socket* Socket::Accept() {
	struct sockaddr_in addr;

	unsigned int len=sizeof(struct sockaddr_in);

	int newSockFD = accept(sockFD, (struct sockaddr *) &addr, (socklen_t *) &len);

	if( newSockFD < 0 ) {
		cerr << "ServerSocket::Accept cannot accept connection" << endl ;
		return 0 ;
	}

	address = strtok(inet_ntoa(addr.sin_addr), "\n");

	Socket *socket = new Socket(newSockFD);
	return socket;
}


// Acquire data from socket.
// Read up to <size> bytes of data and place it into <buf>.
// Returns number of actual bytes read.
// Returns -1 if error.

int Socket::Read(void *buf, int size) {
	return read (sockFD, buf, size);
}


// Send data to socket.
// Write up to <size> bytes of data from <buf>.
// Returns number of actual bytes written.
// Returns -1 if error.

int Socket::Write(void *buf, int size) {
	return write (sockFD, buf, size);
}

