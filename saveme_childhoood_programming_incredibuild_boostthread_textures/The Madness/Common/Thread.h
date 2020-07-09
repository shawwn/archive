#ifndef THREAD_H
#define THREAD_H

#include <windows.h>

class Thread
{
private:
	HANDLE mHandle;
public:
	void BeginThread();
	virtual void ThreadProc() = 0;
	void StopThread();
};

#endif