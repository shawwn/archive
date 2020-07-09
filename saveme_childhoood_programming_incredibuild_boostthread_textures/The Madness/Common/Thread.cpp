#include "Thread.h"
#include "LoggingInterface.h"

static DWORD WINAPI GlobalThreadProc(Thread *th)
{
	th->ThreadProc();
	return 0;
}

void Thread::BeginThread()
{
	StopThread();

	DWORD ThreadID;
	mHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)GlobalThreadProc,this,0,&ThreadID);
	if(!mHandle) {
		gLog << "Thread ERROR: Could not create thread.\n";
		StopThread();
	}
}

void Thread::StopThread()
{
	if(!mHandle)
		return;

	WaitForSingleObject(mHandle,INFINITE);
	CloseHandle(mHandle);
	mHandle = 0;
}

