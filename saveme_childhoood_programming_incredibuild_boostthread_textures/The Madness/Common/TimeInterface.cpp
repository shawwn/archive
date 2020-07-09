#include "TimeInterface.h"
#include "LoggingInterface.h"

TimeInterface gTime;

bool TimeInterface::Initialize()
{
	if(!QueryPerformanceFrequency(&mFrequency))
		return false;

	if(!QueryPerformanceCounter(&mNow))
		return false;
	mLast = mNow;

	return true;
}

bool TimeInterface::Update() 
{
	mLast = mNow;
	
	if(mSyncRate) {
		double t;
		while((t = ((mNow.QuadPart - mLast.QuadPart) / (double)mFrequency.QuadPart)) < mSyncRate) {
			Sleep((DWORD)(( mSyncRate - t) * 1000.0));
			if(!QueryPerformanceCounter(&mNow))
				return false;
		}
	}
	else
	{
		if(!QueryPerformanceCounter(&mNow))
			return false;
	}

	mDeltaTimeInSeconds = ((mNow.QuadPart - mLast.QuadPart) / (double)mFrequency.QuadPart);
	/*
	static double OneSecond = 0;
	static QuadWord Ticks = 0;
	OneSecond += mDeltaTimeInSeconds;
	Ticks += mNow.QuadPart - mLast.QuadPart;

	if(Ticks > (QuadWord)mFrequency.QuadPart) {
		double Test = Ticks / (double)mFrequency.QuadPart;
		gLog << "Sec[" << OneSecond << "] Ticks[" << Test << "]\n";
		OneSecond -= 1.0f;
		Ticks -= mFrequency.QuadPart;
	}*/
	return true;
}