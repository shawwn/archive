#ifndef TIMEINTERFACE_H
#define TIMEINTERFACE_H

#include <windows.h>
#include "Types.h"

class TimeInterface
{
private:
	LARGE_INTEGER mFrequency;
	LARGE_INTEGER mNow;
	LARGE_INTEGER mLast;
	double mDeltaTimeInSeconds;
	double mSyncRate;
public:
	TimeInterface() : mSyncRate(0) { mLast.QuadPart = 0; mNow.QuadPart = 0; }

	bool Initialize();
	bool Update();

	//inline long GetDeltaTime() const { return (long)(mDeltaTimeInSeconds*1000.0f); }
	inline float GetDeltaTime() const { return (float)mDeltaTimeInSeconds * 1000.0f; }
	inline float GetDeltaTimeInSeconds() const { return (float)mDeltaTimeInSeconds; }
	inline void SetSyncRate(float Sync) { mSyncRate = (double)Sync; }
	inline QuadWord GetNow() const { return mNow.QuadPart; }
	inline QuadWord GetFrequency() const { return mFrequency.QuadPart; }
};
extern TimeInterface gTime;

#endif