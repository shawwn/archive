#ifndef SNAPSHOTMANAGER_H
#define SNAPSHOTMANAGER_H

#include "NxPhysics.h"
#include "Types.h"
#include <list>

class Snapshot
{
public:
	UINT Tick;
	NxVec3 Position;
	NxVec3 Velocity;
	NxVec3 Acceleration;
	NxVec3 View;

	Snapshot(UINT SetTick, const NxVec3 &SetPosition = NxVec3(), const NxVec3 &SetVelocity = NxVec3(), const NxVec3 &SetAcceleration = NxVec3(), const NxVec3 &SetView = NxVec3())
		: Tick(SetTick), Position(SetPosition), Velocity(SetVelocity), Acceleration(SetAcceleration), View(SetView) 
	{ }
	~Snapshot() {
	}
};

class SnapshotManager
{
private:
	std::list<Snapshot *> mSnapshots;
	UINT mNumRemoved;
	float mFirstTime;
	float mTimeSubtract;
	float mTime;
public:
	SnapshotManager() : mNumRemoved(0), mFirstTime(-1.0f), mTimeSubtract(0.0f), mTime(0.0f) { }
	~SnapshotManager();

	typedef std::list<Snapshot *>::iterator Iterator;
	typedef std::list<Snapshot *>::reverse_iterator ReverseIterator;
	inline Iterator Begin() { return mSnapshots.begin(); }
	inline Iterator End() { return mSnapshots.end(); }
	inline UINT Size() const { return (UINT)mSnapshots.size(); }

	void AddSnapshot(Snapshot *snap);
	Iterator RemoveSnapshot(Iterator Which);
	Iterator GetSnapshot(UINT Index);
	NxVec3 GetPosition(UINT TickRate);
	NxVec3 GetView(UINT TickRate);
	bool Update();
};

#endif