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

#ifndef __THREAD_H__
#define __THREAD_H__

#include "Types.H"
#include "Runnable.H"

// Define a logical type for thread ID.
typedef unsigned int ThreadId;


// The purpose of this class is to wrap the underlying system thread library
// within an object-oriented interface and make it easier to use threads.
class Thread {
protected:
    // Contains ID of this thread.
    ThreadId threadId;

public:
    Thread();
    virtual ~Thread();

    // Returns ID of the thread.
    ThreadId GetId()  { return threadId; };

    // Takes a runnable object and executes its Run() method within a system thread.
    StatusCode Run(Runnable *runnableObj);

    // Makes the caller of this method block until this thread exits.
    // Note that the caller of this method is executing on another thread.
    StatusCode Join();

    // Causes the caller of this method to terminate.
    // Note that this call only makes sense when the caller is running on this
    // same thread. It's like a suicide!!
    StatusCode Exit();
};

#endif  // __THREAD_H__
