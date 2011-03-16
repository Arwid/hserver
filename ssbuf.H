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

#ifndef __SSBUF_H
#define __SSBUF_H

#include <iostream>
#include <fstream>
#include <string>

#include <fcntl.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#include "Socket.H"

using namespace std;

class ssbuf : public streambuf {
public:
	ssbuf (); // constructor
	ssbuf (Socket* socket); // constructor with arguments
	virtual ~ssbuf (); // destructor
	int accept (Socket* socket); // accept a new connection
protected:
	virtual int underflow ();
	virtual int overflow (int c = EOF);
	virtual int sync ();
private:
	const static int STREAMSIZE = 512;
	
	Socket* socket_;
	
	char* inbuffer_;
	char* outbuffer_;
	streamsize size_;
};

#endif
