#include "SnapshotManager.h"
#include "Macros.h"
#include "TimeInterface.h"


inline float Lerp(float F1, float F2, float Percent) { Percent = min(1.0f,Percent); return (F2-F1)*Percent+F1; }
inline NxVec3 Lerp(const NxVec3 &V1, const NxVec3 &V2, float Percent) { Percent = min(1.0f,Percent); return NxVec3(Lerp(V1.x,V2.x,Percent),Lerp(V1.y,V2.y,Percent),Lerp(V1.z,V2.z,Percent)); }


inline NxVec3 CubicSpline(const NxVec3 &V0, NxVec3 V1, NxVec3 V2, const NxVec3 &V3, float Percent) {
	float A, B, C, D;
	NxVec3 Out;

	A = V3.x - 3 * V2.x + 3 * V1.x - V0.x;
	B = 3 * V2.x - 6 * V1.x + 3 * V0.x;
	C = 3 * V1.x - 3 * V0.x;
	D = V0.x;
	Out.x = A * Percent * Percent * Percent + B * Percent * Percent + C * Percent + D;

	A = V3.y - 3 * V2.y + 3 * V1.y - V0.y;
	B = 3 * V2.y - 6 * V1.y + 3 * V0.y;
	C = 3 * V1.y - 3 * V0.y;
	D = V0.y;
	Out.y = A * Percent * Percent * Percent + B * Percent * Percent + C * Percent + D;

	A = V3.z - 3 * V2.z + 3 * V1.z - V0.z;
	B = 3 * V2.z - 6 * V1.z + 3 * V0.z;
	C = 3 * V1.z - 3 * V0.z;
	D = V0.z;
	Out.z = A * Percent * Percent * Percent + B * Percent * Percent + C * Percent + D;

	return Out;
}

SnapshotManager::~SnapshotManager()
{
	for(Iterator it = Begin(); it != End(); it++) {
		SAFE_DELETE(*it);
	}
}

void SnapshotManager::AddSnapshot(Snapshot *snap)
{
	mSnapshots.push_back(snap);
}

SnapshotManager::Iterator SnapshotManager::RemoveSnapshot(SnapshotManager::Iterator Which)
{
	if(Which == End())
		return End();
	SAFE_DELETE(*Which);
	return mSnapshots.erase(Which);
}

SnapshotManager::Iterator SnapshotManager::GetSnapshot(UINT Index)
{
	if(Index >= Size())
		return End();

	Iterator ret = Begin();
	for(UINT i = 0; i < Index; i++) {
		ret++;
	}
	return ret;
}
#include "TimeInterface.h"

NxVec3 SnapshotManager::GetPosition(UINT TickRate)
{
	float Time = mTime;
	if(mSnapshots.size() == 0)
		return NxVec3(0,0,0);
	if(mFirstTime == -1) {
		//mFirstTime = Time;
		mFirstTime = 0.0f;
	}
	//Time = Time - mFirstTime - mTimeSubtract;
	float TickTime = Time * TickRate;
	UINT StartingIndex = (UINT)TickTime;
	if(StartingIndex+2 > mSnapshots.size()) {
		if(mSnapshots.size()) {
			//mTimeSubtract += gTime.GetDeltaTimeInSeconds();
			while(Size() > 1) {
				RemoveSnapshot(GetSnapshot(0));
			}
			//mFirstTime = mTime;
			mTime = 0.0f;
			return (*GetSnapshot(Size()-1))->Position;
		}
		else
			return NxVec3(0,0,0);
		mFirstTime = Time;
	}
	SnapshotManager::Iterator current = GetSnapshot(StartingIndex);
	SnapshotManager::Iterator next = GetSnapshot(StartingIndex+1);
	return(Lerp((*current)->Position,(*next)->Position,fmod(TickTime,1.0f)));
}

NxVec3 SnapshotManager::GetView(UINT TickRate)
{
	if(Size() == 0)
		return NxVec3(0,0,0);
	SnapshotManager::Iterator Last = GetSnapshot(Size()-1);
	return((*Last)->View);
	/*
	float Time = mTime;
	if(mSnapshots.size() == 0)
		return NxVec3(0,0,0);
	if(mFirstTime == -1) {
		//mFirstTime = Time;
		mFirstTime = 0.0f;
	}
	//Time = Time - mFirstTime - mTimeSubtract;
	float TickTime = Time * TickRate;
	UINT StartingIndex = (UINT)TickTime;
	if(StartingIndex+2 > mSnapshots.size()) {
		if(mSnapshots.size()) {
			//mTimeSubtract += gTime.GetDeltaTimeInSeconds();
			while(Size() > 1) {
				RemoveSnapshot(GetSnapshot(0));
			}
			//mFirstTime = mTime;
			mTime = 0.0f;
			return (*GetSnapshot(Size()-1))->Position;
		}
		else
			return NxVec3(0,0,0);
		mFirstTime = Time;
	}
	SnapshotManager::Iterator current = GetSnapshot(StartingIndex);
	SnapshotManager::Iterator next = GetSnapshot(StartingIndex+1);
	return(Lerp((*current)->Position,(*next)->Position,fmod(TickTime,1.0f)));*/
}

bool SnapshotManager::Update()
{
	mTime += gTime.GetDeltaTimeInSeconds();
	return true;
}