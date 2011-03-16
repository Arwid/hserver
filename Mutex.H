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

#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "Types.H"
#include <pthread.h>

// The purpose of this class is to wrap the underlying system mutual exclusion
// facility within an object-oriented interface, making it easier to use.
class Mutex {
protected:
    // Mutual exclusion variable.
    // Making use of POSIX thread libray mutex.
    pthread_mutex_t mutexVar;

public:
    Mutex();
    virtual ~Mutex();

    // You should call this method when entering a mutual exclusion area.
    StatusCode Lock();

    // You should call this method when exiting a mutual exclusion area.
    StatusCode Unlock();
};

#endif // __MUTEX_H__

