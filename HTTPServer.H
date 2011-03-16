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

#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__
#include <iostream>
#include <fstream>
#include <string>
#include "Config.H"
#include "Runnable.H"
#include "Socket.H"
#include "ssbuf.H"
#define PARAM_ARRAY_SIZE 1000
class HTTPServer : public Runnable {
private:
	Socket* masterSocket;
	ssbuf* ssbuffer;
public:
	static int server_count;
	static bool doTerminate;
	Config *con;
	int index1;
	int Port;
	int tempnum;
	string ROOT_DIR;
	string usernames [PARAM_ARRAY_SIZE];
	string uservalues [PARAM_ARRAY_SIZE];
	HTTPServer(Thread *thread, ssbuf * ssbuffer_, Socket* socket);//Socket* socket);
	virtual ~HTTPServer();
	int saveUserParam();
	string lookupUserParam(string name);
	string strconvert (int i);
	int FileSize (const char * filename);
	void httperror (iostream &sstream, int status, string s_str, string relURL);
	void makeDirectory (iostream &sstream, string relURL); 
	virtual int Run();
	//Graceful server sutdown
	static void HUP_received(int val);


};
#endif // __HTTPSERVER_H__
