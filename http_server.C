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

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <errno.h>
#include <dirent.h>
#include "HTTPServer.H"
#include "HTTPrequest.H"
#include "ssbuf.H"
#include "Config.H"

#define PARAM_ARRAY_SIZE 1000

using namespace std;

// create config object
Config con;
int tempnum = con.saveParam();

// use config to lookup parameters
int Port = atoi(con.lookupParam("Port").c_str());
string ROOT_DIR = con.lookupParam("DocRoot");

string usernames [PARAM_ARRAY_SIZE];
string uservalues [PARAM_ARRAY_SIZE];
int index1 = 0;

int saveUserParam() {
	string inputline, name, value;
	int b, e;
	ifstream inFile;
	inFile.open("installation/passwd");
	if(inFile.fail()){ cerr<< "Error: cannot read passwd"<<endl; return 0;} 

	while(getline(inFile,inputline)){
		b = inputline.find_first_not_of(" ",0);
		e = inputline.find_first_of(" ",b);
		name  = inputline.substr(b,e);
		inputline.erase(0,e+1);
		b = inputline.find_first_not_of(" ",0);
		e = inputline.find_first_of("\r",b);
		value = inputline.substr(b,e);
		usernames[index1]=name; 
		uservalues[index1]=value;
		index1++;
	}
	inFile.close();
	return 1;
}

string lookupUserParam(string name) {
	for(int i = 0; i<index1; i++)
		if(name == usernames[i]) return uservalues[i];
	return "";
}

string strconvert (int i) { 
	// effects: converts integer argument to C++ string 
	// note: needed because ugsparc doesn't have stringstream 
	// installed :-(
	char s[50] ; // bad programming practice 
	sprintf (s, "%d", i);
	return (s); // implicit string constructor
}

int FileSize (const char * filename) { 
	struct stat fileStat; 
	int err = stat (filename, &fileStat); 
	if (0 != err) return 0;
	else return fileStat.st_size;
}

void httperror (iostream &sstream, int status, string s_str, string relURL) { 

	string errorFile;

	con.errorLog (s_str, relURL);

	// display error page for corresponding error
	if (status == 403)
		errorFile = "public_www/403error.html";
	else if (status == 404)
		errorFile = "public_www/404error.html";
	else if (status == 400)
		errorFile = "public_www/400error.html";
	else if (status == 501)
		errorFile = "public_www/501error.html";
	else
		errorFile = "public_www/000error.html";

	ifstream inputfile2(errorFile.c_str(), ios::in);
	
	string header = "HTTP/1.1 200 OK\r\n";
	sstream.write (header.c_str(), header.size());
	
	header = "Content-Type:text/html\r\n";
	sstream.write (header.c_str(), header.size());

	int filesize = FileSize (errorFile.c_str());
	header = "Content-Length:" + strconvert(filesize) + "\r\n";
	sstream.write (header.c_str(), header.size());

	// end with empty line
	sstream.write ("\r\n", 2);

	char c2;
				
	for (; filesize >0; filesize--) {
		c2 = inputfile2.get();
		sstream.put (c2);
		//cout.put(c2);
		if (sstream.fail()) {
			// socket stream failed. Closed by client???
			// nothing we can do, but stop
			break;
		}
	}
	sstream.flush ();

	inputfile2.close ();
}

void makeDirectory (iostream &sstream, string relURL) {
	string makedir = "make_directory.html";

	DIR *pdir;
 	struct dirent *pent;

	string requestedFile = ROOT_DIR + relURL;

 	pdir=opendir(requestedFile.c_str());

	ofstream myfile;
	myfile.open (makedir.c_str());
	myfile << "<html><head>" << endl;
	myfile << "<title>Listing Directory Contents of " << relURL << "</title>" << endl;
	myfile << "</head><body>" << endl;
	myfile << "<h1>Listing Directory Contents of " << relURL << "</h1>" << endl;


	while ((pent=readdir(pdir))){
		if (!strcmp(pent->d_name, ".") || !strcmp(pent->d_name, ".."))
			continue;
		myfile << "<a href=\"" << relURL << pent->d_name << "\">" << pent->d_name << "</a>";
		if (con.lookupAccessLog(relURL + pent->d_name) == "")
			myfile << "  <-> <-><br>" << endl;
		else
			myfile << "  " << con.lookupAccessLog(relURL + pent->d_name) << "<br>" << endl;	
	}

	myfile << "</body></html>" << endl;
	
	closedir(pdir);

	ifstream inputfile2(makedir.c_str(), ios::in);
	
	string header = "HTTP/1.1 200 OK\r\n";
	sstream.write (header.c_str(), header.size());
	
	header = "Content-Type:text/html\r\n";
	sstream.write (header.c_str(), header.size());
	
	int filesize = FileSize (makedir.c_str());
	header = "Content-Length:" + strconvert(filesize) + "\r\n";
	sstream.write (header.c_str(), header.size());
	
	// end with empty line
	sstream.write ("\r\n", 2);

	char c2;
				
	for (; filesize >0; filesize--) {
		c2 = inputfile2.get();
		sstream.put (c2);
		//cout.put(c2);
		if (sstream.fail()) {
			// socket stream failed. Closed by client???
			// nothing we can do, but stop
			break;
		}
	}
	sstream.flush ();

	inputfile2.close ();
}

int main() {
	cerr << "opening a socket" << endl;
	Socket* masterSocket = new Socket();
	if (masterSocket->Listen(Port) < 0) { 
		// handle error
		cerr << "Error: the listen function has failed, OS cannot accept new connection" << endl; 
		return -1;
	}
	
	iostream sstream;
	ssbuf* ssbuffer = new ssbuf();
	
	string content_type;
	string relURL, relURL_orig;
	string connection;
	string requestedFile;
	char c;
	int response;
	string header;
	int filesize = 0;
	string debugParams[3];

	while (ssbuffer->accept(masterSocket) != 0) {
		sstream.rdbuf(ssbuffer);
		HTTPrequest httpreq;

		while (1) {

			response = httpreq.readAndParse (sstream);

			relURL = httpreq.relURL_orig();

			tempnum = saveUserParam();
			int b = relURL.find_first_of("~",0);
			if (relURL.substr(0,b) == "/" && relURL != "/") {
				int e = relURL.find_first_of("/",b);
				if (relURL == relURL.substr(0,e)) {
					relURL = relURL + "/";
					e = relURL.find_first_of("/",b);
				}
				int e2 = relURL.find_first_of("\r",0);
	
				string temprelURL = relURL.substr(2,e-2);
				if (lookupUserParam(temprelURL) != "") {
					relURL = lookupUserParam(temprelURL) + relURL.substr(e,e2-e);
				}
				else {
					httperror (sstream, 404, "Requested file not found", relURL);
					continue;
				}	
			}

			requestedFile = ROOT_DIR + relURL;
			filesize = FileSize (requestedFile.c_str());

			debugParams[0] = httpreq.method();
			debugParams[1] = relURL;
			debugParams[2] = httpreq.version();

			// send information to be written to debug log
			con.debugLog(debugParams, filesize);		

			// send information to be written to access log
			con.accessLog(masterSocket->address, httpreq.method(), relURL, httpreq.version(), 200, filesize);

			if (response == 0) {
				// see if connection has been terminated
				if (sstream.fail()) {
					cerr << "Seems connection no more" << endl;
					break;
				}
				else {
					httperror (sstream, 400, "Malformed request", relURL);
					continue;
				}
			}
			else {
				if (httpreq.method() != "get") {
					httperror (sstream, 501, "Request method not implemented", relURL);
					continue;
				}
				
				struct stat FileStats;
				lstat(requestedFile.c_str(), &FileStats);
				if (FileStats.st_mode&S_IFDIR) {

					if (relURL.substr (relURL.length() - 1) != "/")
						relURL = relURL + "/";
 
					string indexURL1 = ROOT_DIR + relURL + "index.html";
					string indexURL2 = ROOT_DIR + relURL + "index.htm";
					string indexURL3 = ROOT_DIR + relURL + "index.shtml";

					ifstream indexfile1(indexURL1.c_str(), ios::in);
					ifstream indexfile2(indexURL2.c_str(), ios::in);
					ifstream indexfile3(indexURL3.c_str(), ios::in);
					if (indexfile1.fail()) {
						if (indexfile2.fail()) {
							if (indexfile3.fail()) {
								requestedFile = ROOT_DIR + relURL;
								ifstream inputfile(requestedFile.c_str(), ios::in);
								if (inputfile.fail()) {
									httperror (sstream, 404, "Requested file not found", relURL);
									continue;
								}
								makeDirectory (sstream, relURL);
								continue;
							}
							else {
								relURL = relURL + "index.shtml";
							}
						}
						else {
							relURL = relURL + "index.htm";
						}
					}
					else {
						relURL = relURL + "index.html";
					}

				}

				// appropriately set content type corresponding to file extension
				content_type = "no content";
				if (relURL == relURL.substr (0, relURL.find_first_of (".", 0))) {
					content_type = "text/plain";
				}
				if (relURL.length() >= 4) {
					if (relURL.substr (relURL.length() - 2) == ".h" || relURL.substr (relURL.length() - 2) == ".c") {
						content_type = "text/plain";
					}
					if (relURL.substr (relURL.length() - 2) == ".H" || relURL.substr (relURL.length() - 2) == ".C") {
						content_type = "text/plain";
					}
				}
				if (relURL.length() >= 6) {
					if (relURL.substr (relURL.length() - 4) == ".htm") {
						content_type = "text/html";
					}
					else if (relURL.substr (relURL.length() - 4) == ".jpg") {
						content_type = "image/jpeg";
					}
					else if (relURL.substr (relURL.length() - 4) == ".txt") {
						content_type = "text/plain";
					}
					else if (relURL.substr (relURL.length() - 4) == ".gif") {
						content_type = "image/gif";
					}
					else if (relURL.substr (relURL.length() - 4) == ".mpg") {
						content_type = "video/mpeg";
					}
					else if (relURL.substr (relURL.length() - 4) == ".pdf") {
						content_type = "application/pdf";
					}
				}
				if (relURL.length() >= 7) {
					if (relURL.substr (relURL.length() - 5) == ".html") {
						content_type = "text/html";
					}
					else if (relURL.substr (relURL.length() - 5) == ".jpeg") {
						content_type = "image/jpeg";
					}
				}
				if (relURL.length() >= 8) {
					if (relURL.substr (relURL.length() - 6) == ".class") {
						content_type = "application/java";
					}
					else if (relURL.substr (relURL.length() - 6) == ".shtml") {
						content_type = "text/html";
					}
				}
				if (content_type == "no content") {
					httperror (sstream, 403, "Requested file forbidden", relURL);
					continue;
				}

				requestedFile = ROOT_DIR + relURL;
				filesize = FileSize (requestedFile.c_str());
				ifstream inputfile(requestedFile.c_str(), ios::in);
			
				if (inputfile.fail()) {
					httperror (sstream, 404, "Requested file not found", relURL);
					continue;
				}

				header = "HTTP/1.1 200 OK\r\n";
				sstream.write (header.c_str(), header.size());
	
				header = "Content-Type:" + content_type + "\r\n";
				sstream.write (header.c_str(), header.size());

				header = "Content-Length:" + strconvert(filesize) + "\r\n";
				sstream.write (header.c_str(), header.size());
				
				header = "Connection:" + connection + "\r\n";
				sstream.write (header.c_str(), header.size());
				
				// end with empty line
				sstream.write ("\r\n", 2);

				for (; filesize >0; filesize--) {
					c = inputfile.get();
					sstream.put (c);
					//cout.put(c);
					if (sstream.fail()) {
						// socket stream failed. Closed by client???
						// nothing we can do, but stop
						break;
					}
				}
		
				sstream.flush ();

				inputfile.close ();
			}
			if (httpreq.lookupParam("connection") == "close")
				break;
		}
		break;
	}
	
	return 0;
}
