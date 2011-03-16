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

#include "Mutex.H"

Mutex::Mutex()
{
    // Initialize the mutual exclusion variable.
    // Making use of POSIX thread libray mutex.
    pthread_mutex_init(&mutexVar, NULL);
}

Mutex::~Mutex()
{
    // Destroying the POSIX mutual exclusion variable.
    pthread_mutex_destroy(&mutexVar);
}


// You should call this method when entering a mutual exclusion area.
StatusCode 
Mutex::Lock()
{
    // Acquire POSIX mutual exclusion lock.
    int stat = pthread_mutex_lock(&mutexVar);
  
    if (stat == 0) {
        return scOK;
    } else {
        return scERR;
    }
}


// You should call this method when exiting a mutual exclusion area.
StatusCode 
Mutex::Unlock()
{
    // Release POSIX mutual exclusion lock.
    int stat = pthread_mutex_unlock(&mutexVar);
  
    if (stat == 0) {
        return scOK;
    } else {
        // This error could happen when the calling thread does not hold this
        // mutex, i.e. it hasn't done Lock() yet.
        return scERR;
    }
}
