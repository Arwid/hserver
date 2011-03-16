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

#ifndef __RUNNABLE_H__
#define __RUNNABLE_H__

// Forward declaration in order to avoid cyclic dependency.
class Thread;

// The purpose of this abstract class is enable other classes to inherit it
// and enable a thread to execute the class' Run() method.
class Runnable {
protected:
    // Keeps a handle to the thread that this object is executing on.
    Thread *myThread;

public:
    // Passing the thread that calls run so that the 
    // runnable object knows which thread is running it.
    Runnable(Thread *thread)  {  myThread = thread; };

    // Derived classes must define this method, which will be executed within
    // a thread.
    virtual int Run()=0;
};

#endif 
