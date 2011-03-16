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

#include <string>
#include <cstdlib>

#include <sys/socket.h>
#include "HTTPServer.H"
#include "Thread.H"
#include "Config.H"
#include "Socket.H"
#include "ssbuf.H"
#include "Types.H"
#include "Mutex.H"
#include "MutexInstance.H"
//#include "Runnable.H"
#include "Thread.H"

#include <signal.h>

using namespace std;

class ServWatcher : public Runnable {
public:
	ServWatcher(Thread *thread) : Runnable(thread) {}
	virtual ~ServWatcher() {}
	virtual int Run() {
		bool already = false;
		while (1) {
			cout << "running watch"<<endl;
			sleep(1);
			if (HTTPServer::server_count != 0)
				already = true;
			if (already && HTTPServer::doTerminate && HTTPServer::server_count == 0) {
					cout << "exiting"<<endl;
					exit (-1);
					cout << "should not print"<<endl;
			}
		}
	}
};

int main(){
	signal(SIGHUP, HTTPServer::HUP_received);

	Config* con = Config::Instance();
	con->saveParam();
	int Port = atoi(con->lookupParam("Port").c_str());
	cerr << "opening a socket" << endl;
	Socket* masterSocket = new Socket();
	if (masterSocket->Listen(Port) < 0) { 
		// handle error
		cerr << "Error: the listen function has failed, OS cannot accept new connection" << endl; 
		return -1;
	}

	int LENGTH = 2;
	int count = 0;
	Thread* threads[LENGTH];
	for (int i = 0; i < LENGTH; i++)
		threads[i] = NULL;

	ssbuf* ssbuffer;
	Thread* thread;

	Thread* threadWatch = new Thread();
	threadWatch->Run(new ServWatcher(thread));

	MutexInstance::Instance();

	while ((ssbuffer = new ssbuf())->accept(masterSocket) != 0) {
		cout << (threads[count] == NULL) << endl;
		if (threads[count] == NULL) {
			cout << "created new" << endl;
			threads[count] = new Thread();
		}
		thread = threads[count];
		cout << "Opening new connection:"<<count<<endl;

		thread->Join();
		cout << "Joining"<<endl;
		cout << "Run:"<<thread->Run(new HTTPServer(thread, ssbuffer, masterSocket))<<endl;
		cout << "Running:"<<count<<endl;
		if (count++ > LENGTH-1)
			count = 0;
		if (HTTPServer::server_count <= 0) break;
	}
	return 0;
}