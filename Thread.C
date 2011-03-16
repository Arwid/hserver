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

#include <pthread.h>
#include <string>
#include "Thread.H"

Thread::Thread()
{
    threadId = 0;
}


Thread::~Thread()
{
}


// A helper function since there are difficulties in directly passing a method
// as a function pointer to pthread_create(). 
// WorkerFunc is just a wrapper around runnableObj->Run().
void *WorkerFunc(void * runnableObj)
{
     ((Runnable *)runnableObj)->Run();
     return NULL;
}


// Takes a runnable object and executes its Run() method within a system thread.
StatusCode 
Thread::Run(Runnable *runnableObj)
{
    // We make use of the POSIX thread library.
    int res = pthread_create((pthread_t *)&threadId, NULL, WorkerFunc, (void *)runnableObj);

    if (res == 0) { 
       return scOK;
    } else {
        return scERR;
    }
}


// Makes the caller of this method block until this thread exits.
// Note that the caller of this method is executing on another thread.
StatusCode 
Thread::Join()
{
    // At the moment, we are ignoring the return value of the thread
    // so we pass NULL as the second argument.
    int res = pthread_join(threadId, NULL);

    if (res == 0) { 
       return scOK;
    } else {
        return scERR;
    }
}


// Causes the caller of this method to terminate.
// Note that this call only makes sense when the caller is running on this
// same thread. It's like a suicide!!
StatusCode 
Thread::Exit()
{
		cout << "EXITING THREAD 0"<<endl;
    // Assuming the caller of this method is the Runnable object that is
    // executing on this thread.  There is no point in checking for errors.
    pthread_exit(NULL);
	cout << "EXITING THREAD 1"<<endl;
    return scOK;
}
