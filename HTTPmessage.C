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

using namespace std;

const int PARAM_ARRAY_SIZE = 10;

string paramNames [PARAM_ARRAY_SIZE];
string paramValues [PARAM_ARRAY_SIZE];
int paramCount_ = 0;
int index_ = 0;
	
int HTTPmessage::saveParam (string name, string value){

	// don't save parameters if it exceeds the parameter array size
	if (paramCount_ >= PARAM_ARRAY_SIZE){
		return 0;
	}
	
	// save name and value in the array of parameters
	paramNames [paramCount_] = name;
	paramValues [paramCount_] = value;
	
	// increase the parameter count by 1
	paramCount_++;

	return 1;
}

string HTTPmessage::lookupParam (string name){
	
	// match the name with a parameter name, or else return nothing
	for (int i=0; i<PARAM_ARRAY_SIZE; i++){
		if (paramNames [i] == name)
			return paramValues [i];
	}
	return "";
}

int HTTPmessage::getFirstParam (string &name, string &value){
	
	// make sure there is a first parameter
	if (paramCount_ <= 0) return 0;
	index_ = 0;
	
	// send back the parameter name and value
	name = paramNames [index_];
	value = paramValues [index_];
	
	return 1;
}

int HTTPmessage::getNextParam (string &name, string &value){

	// make sure there are parameters
	if (index_ < 0) return 0;
	index_++;
	
	// stop when there are no more parameters
	if (index_ >= paramCount_) return 0;
	
	// semd back the next parameter name and value
	name = paramNames [index_];
	value = paramValues [index_];
	
	return 1;
}

int HTTPmessage::getParamCount (){
	return paramCount_;
}
	
