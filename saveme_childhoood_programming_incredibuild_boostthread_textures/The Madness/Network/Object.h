#ifndef OBJECT_H
#define OBJECT_H

#define NOMINMAX
#include "NxPhysics.h"
#include <string>
#include "Types.h"
#include "Packet.h"
#include "SnapshotManager.h"

void ReckonPosition(NxActor *Base, const NxActor *Move, float Radius);
void ReckonVelocity(NxActor *Base, const NxActor *Move, float Radius);
inline bool Reckon(const NxVec3 &Base, const NxVec3 &Compare, float Radius) { return (Base.distance(Compare) < Radius ? false : true); }
inline bool ReckonAngle(NxVec3 Base, NxVec3 Compare, float MaxDot) { Base.normalize(); Compare.normalize(); return ((1 - abs(Base.dot(Compare))) < MaxDot ? false : true); }

class Kinematic
{
public:
	Kinematic() : Position(NxVec3(0,0,0)), Velocity(NxVec3(0,0,0)), Acceleration(NxVec3(0,0,0)), View(NxVec3(0,0,1)) { }
	NxVec3 Position;
	NxVec3 Velocity;
	NxVec3 Acceleration;
	NxVec3 View;
};

class Simulate
{
public:
	Simulate() : Actor(NULL), Acceleration(NxVec3(0,0,0)), TotalForce(NxVec3(0,0,0)), MovementDirection(NxVec3(0,0,0)), View(NxVec3(0,0,1)) { }
	NxActor *Actor;
	NxVec3 Acceleration;
	NxVec3 TotalForce;
	NxVec3 MovementDirection;
	NxVec3 View;
	Kinematic Previous;

	void SetPrevious() {
		Previous.Position = Actor->getGlobalPosition();
		Previous.Velocity = Actor->getLinearVelocity();
		Previous.Acceleration = Acceleration;
		Previous.View = View;
	}
};

#define OBJECT_POSITION		(1 << 7)
#define OBJECT_VELOCITY		(1 << 6)
#define OBJECT_ACCELERATION (1 << 5)
#define OBJECT_ALL OBJECT_POSITION | OBJECT_VELOCITY | OBJECT_ACCELERATION

class Object
{
protected:
	Simulate mClient;
	float mClock;
	std::string mName;
	QuadWord mGUID;
	Word mType;
	SnapshotManager mSnapshots;
public:
	Object(QuadWord GUID = -1) : mClock(0), mGUID(GUID), mType(-1) { }
	~Object();

	void CreateDefaultActor(NxScene *Scene, UINT Group = 1);
    
	void SetPosition(const NxVec3 &Pos);
	void SetVelocity(const NxVec3 &Vel);
	void SetAcceleration(const NxVec3 &Accel);
	inline void SetMovementDirection(const NxVec3 &Movement) { mClient.MovementDirection = Movement; }
	inline void SetView(const NxVec3 &View) { mClient.View = View; }
	inline void SetGUID(QuadWord GUID) { mGUID = GUID; }

	void SetClock(float Clock) { mClock = Clock; }
	inline void SetName(const std::string &Name) { mName = Name; }
	inline void SetType(Word Type) { mType = Type; }

	NxVec3 GetPosition();
	NxVec3 GetVelocity();
	const NxVec3 &GetAcceleration();
	const NxVec3 &GetView() const { return mClient.View; }
	inline const NxVec3 &GetMovementDirection() const { return mClient.MovementDirection; }

	NxActor *GetActor() const { return mClient.Actor; }
	inline float GetClock() const { return mClock; }
	Kinematic &GetPrevious() { return mClient.Previous; }

	inline const std::string &GetName() const { return mName; }
	inline QuadWord GetGUID() const { return mGUID; }
	inline Word GetType() const { return mType; }

	void AddForce(const NxVec3 &Force);

	bool Update();
	virtual bool CustomUpdate() { return true; }
	virtual bool Render(void *Param) { return true; }

	Packet &GetKinematicPacket(Byte Flags = OBJECT_ALL, Packet &pack = Packet());
	SnapshotManager &Snapshots() { return mSnapshots; }
};

#endif