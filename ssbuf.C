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

#include "ssbuf.H"

using namespace std;

ssbuf::ssbuf() {
	inbuffer_ = 0;
	outbuffer_ = 0;
	size_ = 0;
	socket_ = NULL;
}

ssbuf::ssbuf(Socket* socket) {
	socket_ = socket;
	inbuffer_ = new char [STREAMSIZE];
	outbuffer_ = new char [STREAMSIZE];
	size_ = STREAMSIZE;
	setg (inbuffer_, inbuffer_, inbuffer_);
	setp (outbuffer_, outbuffer_ + STREAMSIZE - 1);
}

ssbuf::~ssbuf() {
	delete inbuffer_;
	delete outbuffer_ ;
	size_ = 0;
	delete socket_;
}

// accept a new connection
int ssbuf::accept (Socket* socket) {
	socket_ = socket->Accept();
	
	// allocate input buffer and output buffer
	inbuffer_ = new char [STREAMSIZE];
	outbuffer_ = new char [STREAMSIZE];
	size_ = STREAMSIZE;
	
	// set pointers accordingly
	setg (inbuffer_, inbuffer_, inbuffer_);
	setp (outbuffer_, outbuffer_ + size_ - 1);
	
	return 1;
}

int ssbuf::underflow() {
	int n;
	if (gptr() < egptr()) // do we even need to read anything?
	return *(unsigned char*)gptr();
	if ((n = socket_->Read(inbuffer_, STREAMSIZE)) <= 0)
		return EOF;
	setg (inbuffer_, inbuffer_, inbuffer_ + n);
	return *(unsigned char*)gptr();
}

int ssbuf::overflow(int c) {
	int n;
	if (c != EOF) {
		*(pptr()) = c;
		pbump(1);
	}
	char* p = pbase();
	while (p < pptr()) {
		n = socket_->Write(p, pptr()-p);
		if (n<0) return EOF;
		p += n;
	}
	setp (outbuffer_, outbuffer_ + size_ - 1);
	return 0 ;
}

int ssbuf::sync() {
	return overflow (EOF);
}
