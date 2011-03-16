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
 ****************************************************************************/

#include "Config.H" 
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include "MutexInstance.H"
using namespace std;
using std::string;

// Implementation
Config* Config::pinstance = 0;

Config :: Config(){
	count = 0;
	index = 0;
}

int Config :: saveParam() {
	MutexInstance::Instance()->Lock();
	string inputline, name, value;
	int v,e,r,b;
	ifstream inFile;
	string whitespaces (" \t\f\v\n\r");
	inFile.open("config_file.txt");
	if(inFile.fail()){ 
		cerr<< "Error: cannot read config_file.txt"<<endl; 
		MutexInstance::Instance()->Unlock();
		return 0;
	} 
	
	while(getline(inFile,inputline)){
		
		b = inputline.find_first_not_of(" ",0);
		e = inputline.find_first_of(":",b);
		name  = inputline.substr(b,e);
		inputline.erase(0,e+1);
		r = inputline.find_last_not_of(whitespaces);
		v = inputline.find_first_not_of(" ",0);
		value = inputline.substr(v,r);
		names[index]=name; 
		values[index]=value;
		index++;
	}
	inFile.close();
	MutexInstance::Instance()->Unlock();
	return 1 ;
}

string Config :: lookupParam( string name ) {
	for(int i = 0; i<index; i++)
		if(name == names[i]) return values[i];
	return "";
}

int Config :: getFirstParam( string &name, string &value ) {
	if( names[0] == "" ) return 0 ;
	name = names[ 0] ;
	value = values[ 0 ] ;
	count++;
	return 1 ;
}

int Config :: getNextParam( string &name, string &value ) {
	if( index <0 ) return 0 ;
	if(count == 0){cerr<<"Error: Cannot call next parameter without calling first parameter"<<endl; return 0;}
	if( count>index ) return 0 ;
	name = names[count] ;
	value = values[count] ;
	count++ ;
	return 1 ;
}

void Config :: printH() {
	for(int i=0; i < index; i++){
		string s = names[i];
		string s2 = values[i];
		cout<<"parameter "<<i+1<<": "<<s<<": "<<s2<<endl;
	}
}

string Config :: lookupAccessLog (string relURL) {
	MutexInstance::Instance()->Lock();
	string accessnames [PARAM_ARRAY_SIZE];
	string accessvalues [PARAM_ARRAY_SIZE];
	string inputline, returnString, tempvalues;
	returnString = "";
	int b,e;
	int index_access = 0;
	ifstream inFile;
	inFile.open("access.log");
	if(inFile.fail()){ cerr<< "Error: cannot read access.log"<<endl; return 0;} 
	
	while(getline(inFile,inputline)){
		e = inputline.find_first_of(" ",0);
		inputline.erase(0,e);
		b = inputline.find_first_not_of(" ",0);
		inputline.erase(0,b);
		e = inputline.find_first_of("\"",0);
		tempvalues = " &lt;" + inputline.substr(0,e-1) + "&gt;";
		inputline.erase(0,e);
		b = inputline.find_first_of("/",0);
		inputline.erase(0,b);
		b = inputline.find_first_of(" ",0);
		accessnames[index_access] = inputline.substr(0,b);
		inputline.erase(0,b);
		e = inputline.find_first_of("\"",0);
		inputline.erase(0,e+1);
		b = inputline.find_first_not_of(" ",0);
		inputline.erase(0,b);
		e = inputline.find_first_of(" ",0);
		inputline.erase(0,e);
		b = inputline.find_first_not_of(" ",0);
		e = inputline.find_first_of("\r",b);		
		accessvalues[index_access] = "<" + inputline.substr(b,e-b) + ">" + tempvalues;
		index_access++;
	}
	inFile.close();

	for(int i = 0; i<index_access; i++)
		if(relURL == accessnames[i]) returnString = accessvalues[i];
	MutexInstance::Instance()->Unlock();
	return returnString;
}

void Config :: accessLog(char* addr, string method, string relUrl, string vers, int status, int fsize) {
	MutexInstance::Instance()->Lock();
	time_t rawtime;
	ofstream myfile;
	int r;
	string loctime, whitespaces (" \t\f\v\n\r");
	myfile.open (lookupParam("AccessLog").c_str() ,ios::app);
	time ( &rawtime );
	loctime =  ctime (&rawtime);
	r = loctime.find_last_not_of(whitespaces);
	loctime = loctime.substr(0,r+1);
	myfile <<addr<<" "<<loctime<<" "<<"\""<<method<<" "<<relUrl<<" "<<"HTTP/"<<vers<<"\" "<<status<<" "<<fsize<<endl;
	myfile.close();
	MutexInstance::Instance()->Unlock();
}

void Config :: errorLog(string error, string relUrl) {
	MutexInstance::Instance()->Lock();
	time_t rawtime;
	ofstream myfile;
	int r;
	string loctime, whitespaces (" \t\f\v\n\r");
	myfile.open (lookupParam("ErrorLog").c_str() ,ios::app);
	time ( &rawtime );
	loctime =  ctime (&rawtime);
	r = loctime.find_last_not_of(whitespaces);
	loctime = loctime.substr(0,r+1);
	myfile <<"["<<loctime<<"]"<<" "<<"httpserver::main"<<" "<<"["<<error<<"]"<<" "<<relUrl<<" "<<endl;
	myfile.close();
	MutexInstance::Instance()->Unlock();
}

void Config :: debugLog(string params[], int filesize) {
	MutexInstance::Instance()->Lock();
	time_t rawtime;
	ofstream myfile;
	int r;
	string loctime, whitespaces (" \t\f\v\n\r");
	myfile.open (lookupParam("DebugLog").c_str() ,ios::app);
	time ( &rawtime );
	loctime =  ctime (&rawtime);
	r = loctime.find_last_not_of(whitespaces);
	loctime = loctime.substr(0,r+1);
	myfile <<"["<<loctime<<"]"<<" "<<params[0]<<" "<<params[1]<<" "<<params[2]<<" "<<filesize<<endl;
	myfile.close();
	MutexInstance::Instance()->Unlock();
}

