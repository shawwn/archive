#include "Object.h"
#include "NovodexInterface.h"
#include "TimeInterface.h"
#include "PlayerInterface.h"

void ReckonPosition(NxActor *Base, const NxActor *Move, float Radius)
{
	NxVec3 BasePos = Base->getGlobalPosition();
	NxVec3 MovePos = Move->getGlobalPosition();
	if(BasePos.distance(MovePos) > Radius) {
		NxVec3 DifferenceVec = BasePos - MovePos;
		DifferenceVec.setMagnitude(Radius);
		Base->setGlobalPosition(MovePos + DifferenceVec);
	}
}

void ReckonVelocity(NxActor *Base, const NxActor *Move, float Radius)
{
	NxVec3 BaseVel = Base->getLinearVelocity();
	NxVec3 MoveVel = Move->getLinearVelocity();
	if(BaseVel.distance(MoveVel) > Radius) {
		NxVec3 DifferenceVec = BaseVel - MoveVel;
		DifferenceVec.setMagnitude(Radius);
		Base->setLinearVelocity(MoveVel + DifferenceVec);
	}
}

Object::~Object()
{
	if(mClient.Actor)
		gPhysics.GetScene()->releaseActor(*mClient.Actor);
}

void Object::CreateDefaultActor(NxScene *Scene, UINT Group)
{
	if(!Scene)
		return;
	NxActorDesc ActorDesc;
	NxBodyDesc BodyDesc;
	NxSphereShapeDesc SphereDesc;
	
	BodyDesc.linearDamping = 0.5f;
	BodyDesc.angularDamping = 0.5f;

	ActorDesc.body = &BodyDesc;
	ActorDesc.density = 10.0f;
	ActorDesc.group = Group;
	ActorDesc.userData = this;
	
	SphereDesc.radius = 5.0f;
	ActorDesc.shapes.push_back(&SphereDesc);
	mClient.Actor = Scene->createActor(ActorDesc);
	NxActor **Actors = Scene->getActors();
}

void Object::SetPosition(const NxVec3 &Pos)
{
	if(!mClient.Actor)
		return;
	mClient.Actor->setGlobalPosition(Pos);
}

void Object::SetVelocity(const NxVec3 &Vel)
{
	if(!mClient.Actor)
		return;
	mClient.Actor->setLinearVelocity(Vel);
}

void Object::SetAcceleration(const NxVec3 &Acc)
{
	if(!mClient.Actor)
		return;
	mClient.Acceleration = Acc;
}

NxVec3 Object::GetPosition() { return mClient.Actor->getGlobalPosition(); }
NxVec3 Object::GetVelocity() { return mClient.Actor->getLinearVelocity(); }
const NxVec3 &Object::GetAcceleration() { return mClient.Acceleration; }

bool Object::Update()
{
	CustomUpdate();

	if(mClient.Actor) {
		SetAcceleration(mClient.MovementDirection);
		mClient.Actor->addForce(mClient.Acceleration * gTime.GetDeltaTimeInSeconds(),NX_VELOCITY_CHANGE);
		mClient.TotalForce = NxVec3(0,0,0);
		//SetAcceleration(mClient.Actor->getLinearVelocity() - mClient.Previous.Velocity);
	}

	mClock += gTime.GetDeltaTimeInSeconds();
	mSnapshots.Update();
	return true;
}

Packet &Object::GetKinematicPacket(Byte Flags, Packet &pack)
{
	//pack.Build(SMSG_OBJECT_UPDATE) << mClock << mGUID << GetPosition() << GetVelocity() << GetAcceleration();

	pack.Build(SMSG_OBJECT_UPDATE) << mGUID;
	if(Flags & OBJECT_POSITION) { 
		pack << GetPosition(); }
	if(gPlayers.Find(mGUID))
		pack << GetView();
	/*
	if(Flags & OBJECT_VELOCITY) { 
		pack << GetVelocity(); }
	if(Flags & OBJECT_ACCELERATION) { 
		pack << GetAcceleration(); }*/
	return pack;
}

void Object::AddForce(const NxVec3 &Force)
{
	mClient.TotalForce += Force;
}