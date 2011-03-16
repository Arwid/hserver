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

#ifndef __CONFIG_H__
#define __CONFIG_H__
#define PARAM_ARRAY_SIZE 1000
#include <string>
#include <fstream>
using std::string;


class Config {
	private:
		int count;
		
		string names [PARAM_ARRAY_SIZE];
		string values [PARAM_ARRAY_SIZE];
		static Config * pinstance; //initialize pointer

	protected:
		Config();
		//initializes paramCount and index
		~Config(){};
	
	public:
		static Config* Instance(){
			if (pinstance ==0){
				pinstance = new Config;
			}
			return pinstance;
		}

		int index;

		int saveParam() ;
			// effect: saves the pair (name, value) for future lookup
			// returns 0 on error
			// requires: name and value not be null-strings
		string lookupParam( string name ) ;
			// effect: looks up name and returns corresponding value
			// or null-string on error
		int getFirstParam( string &name, string &value ) ;
			// effect: treating all saved (n,v)-pairs as a sequence,
			// sets name and value to the (name,value) of the
			// first. Returns 0 when there are no saved
			// parameters or on error and 1 otherwise.
			// note: name and value are reference parameters
		int getNextParam( string &name, string &value ) ;
			// effect: sets name and value to the next (name,value)
			// pair in the sequence
			// Returns 0 when there is no next pair or on error
			// and 1 otherwise
			// note: name and value are reference parameters
		void printH();
		void accessLog(char* addr, string method, string relUrl, string vers, int status, int fsize);
			// write request information to access log file for future reference
		void errorLog(string error, string relUrl);
			// write error information to error log file for future reference
		void debugLog(string params[], int filesize);
			// write debug information to debug log file for future reference
		string lookupAccessLog(string relURL);
};

#endif 

