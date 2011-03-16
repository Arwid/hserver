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

#include "HTTPrequest.H"

int HTTPrequest::readAndParse (istream &s)
{
	string inputline, inputline_orig;
	//istream s(sin.rdbuf());

	getline (s, inputline);

	// if the input fails, give error
	if (s.fail()&&!s.eof()){
		//cerr << "Error: Cannot read input!" << endl;
		return 0;
	}
	// if theres no input, give error
	if (inputline.size() == 0){
		//cerr << "Error: No input" << endl;
		return 0;
	}
	inputline_orig = inputline;

	// change input line to all lower case
	for (unsigned int i=0; i<inputline.size(); i++){
		inputline[i] = tolower (inputline[i]);
	}
	
	int b, e ; // beginning and ending characters
	string component, component_orig;
	
	// find first non-blank character
	b = inputline.find_first_not_of (" ", 0);
	// find first subsequent blank character
	e = inputline.find_first_of (" ", b);
	// component is everything between b and e
	component = inputline.substr (b, e-b);
	
	// save this component as the method variable
	method (component);

	// then remove all text up to e from inputline
	inputline = inputline.erase (0, e);
	inputline_orig = inputline_orig.erase (0, e);

	// find first non-blank character
	b = inputline.find_first_not_of (" ", 0);
	// find first subsequent blank character
	e = inputline.find_first_of (" ", b);
	
	// check for complete request line
	if (e < 0){
		//cerr << "Error: Request line must have METHOD, URL, and VERSION" << endl;
		return 0;
	}
	
	// component is everything between b and e
	component = inputline.substr (b, e-b);
	component_orig = inputline_orig.substr (b, e-b);
	
	// save this component as the url variable
	relURL (component);
	relURL_orig (component_orig);

	// then remove all text up to e from inputline
	inputline = inputline.erase (0, e);

	// find first non-blank character
	b = inputline.find_first_not_of (" ", 0);
	// find first '/' character
	e = inputline.find_first_of ("/", b);
	
	// check for complete request line
	if (b < 0){
		//cerr << "Error: Request line must have METHOD, URL, and VERSION" << endl;
		return 0;
	}
	
	// component should be http keyword
	component = inputline.substr (b, e-b+1);
	
	// check to make sure that the keyword http is there
	if (component == "http/"){
		// then remove all text up to e from inputline which is http
		inputline = inputline.erase (0, e+1);
		
		// find the end of the line
		e = inputline.find_first_of ("\r", 0);
		// component is everything remaining in inputline
		component = inputline.substr (0, e);
		
		// save this component as the version variable
		version (component);

		// then remove all text up to e from inputline
		inputline = inputline.erase (0, e);
	}
	else{
		//give error if cant find http keyword
		//cerr << "Error: Malformed HTTP version, must type HTTP/ before version" << endl;
		return 0;
	}
	
	// get the next line
	getline (s, inputline);
	
	// check for empty line, if so, then program complete
	if (inputline == "\r"){
		return 1;
	}
	
	// change input line to all lower case
	for (unsigned int i=0; i<inputline.size(); i++){
		inputline[i] = tolower (inputline[i]);
	}
	
	string name_component, value_component;
	
	// make sure there is something in the line
	while (inputline.size() != 0) {
	
		// find first non-blank character
		b = inputline.find_first_not_of (" ", 0);
		// find first ':' character
		e = inputline.find_first_of (":", b);
		// name component is everything between b and e
		name_component = inputline.substr (b, e-b);
		
		// this is a check to make sure that a ':' was found
		// if a ':' is NOT found, give an error because the header is malformed
		if (name_component == inputline && name_component != "\r"){
			//cerr << "Error: Malformed header" << endl;
			return 0;
		}
		
		// then remove all text up to and including the ':'
		inputline = inputline.erase (0, e+1);
		
		// find first non-blank character
		b = inputline.find_first_not_of (" ", 0);
		// find the end of the line
		e = inputline.find_first_of ("\r", b);
		// value component is everything between b to the end of the line
		value_component = inputline.substr (b, e-b);
		
		// save the name and value as long as they are there
		if (name_component != "\r")
			saveParam (name_component, value_component);
		
		//cout << name_component << ": " << value_component << endl;
		
		// get the next line
		getline (s, inputline);
		
		// check for empty line, if so, then program complete
		if (inputline == "\r"){
			return 1;
		}
	
		// change input line to all lower case
		for (unsigned int i=0; i<inputline.size(); i++){
			inputline[i] = tolower (inputline[i]);
		}
	}
	
	//cerr << "Error: Requires an empty line at the end" << endl;
		
	return 0;
}

void HTTPrequest::method (string &method){
	method_ = method;
}

string HTTPrequest::method (){
	return method_;
}

void HTTPrequest::relURL (string &url){
	url_ = url;
}

string HTTPrequest::relURL (){
	return url_;
}

void HTTPrequest::version (string &vers){
	vers_ = vers;
}

string HTTPrequest::version (){
	return vers_;
}

void HTTPrequest::relURL_orig (string &url_orig){
	url_orig_ = url_orig;
}

string HTTPrequest::relURL_orig (){
	return url_orig_;
}


