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

#include "HTTPmessage.H"

class HTTPrequest : public HTTPmessage {
public:
	int readAndParse (istream &s);//ios &sin);
		// effect: 	reads one full request from s, parses the request and stores
		//  		the components in appropriate data members.
		// 		returns 1 when request well formed and 0 otherwise
		// requires: 	s must be an input stream open for reading

	void method (string &method);
		// effect: 	stores method in appropriate data member

	string method ();
		// effect: 	returns stored method

	void relURL (string &url);
		// effect: 	stores url in appropriate data member

	string relURL ();
		// effect: 	returns stored url

	void version (string &vers);
		// effect: 	stores version number in appropriate data member

	string version ();
		// effect: 	returns stored version number
	void relURL_orig (string &url_orig);
	string relURL_orig();
		
	string method_;
	string url_;
	string vers_;
	string url_orig_;
};
