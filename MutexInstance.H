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

#ifndef __MUTEX_INSTANCE_H
#define __MUTEX_INSTANCE_H

#include "Mutex.H"

class MutexInstance {
private:
	static Mutex * pinstance;
protected:
	MutexInstance();
public:
	static Mutex * Instance() {
		if (pinstance == 0) {
			pinstance = new Mutex();
		}
		return pinstance;
	}
};
#endif


