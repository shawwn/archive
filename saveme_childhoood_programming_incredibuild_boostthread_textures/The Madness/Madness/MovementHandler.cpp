#include "XMesh.h"
#include "PlayerInterface.h"
#include "MovementHandler.h"
#include "GameInterface.h"
#include "NetworkInterface.h"
#include "TimeInterface.h"
#include "ClientSettingsInterface.h"
#include "ObjectInterface.h"


void MovementHandler::Register()
{
	gHandlers.RegisterHandler(SMSG_OBJECT_UPDATE,(PacketHandler *)this);
	gHandlers.RegisterHandler(SMSG_PLAYERANIMATION,(PacketHandler *)this);
}

bool MovementHandler::HandleSMSG_OBJECTUPDATE(Packet *pack, Client *cli)
{
	QuadWord GUID;
//	Byte Flags;
	NxVec3 Pos = NxVec3(0,0,0);
	NxVec3 Vel = NxVec3(0,0,0);
	NxVec3 Accel = NxVec3(0,0,0);
	NxVec3 View = NxVec3(0,0,0);

	//*pack >> GUID >> Flags;
	*pack >> GUID;

	Object *obj = (Object *)gPlayers.Find(GUID);
	bool IsPlayer = true;
	if(!obj) {
		IsPlayer = false;
		obj = gObjects.Find(GUID);
		if(!obj)
			return false;
	}

	//*pack >> Pos >> Vel >> Accel;
	*pack >> Pos;
	if(IsPlayer)
		*pack >> View;

	if(gNetwork.GetTick()) {
		Snapshot *snap = new Snapshot(gNetwork.GetTick(),Pos,Vel,Accel,View);
		obj->Snapshots().AddSnapshot(snap);
	}
	return true;
}

bool MovementHandler::HandleSMSG_PLAYERANIMATION(Packet *pack, Client *cli)
{
	QuadWord GUID = 0;
	Byte Set;
	Byte Reverse;

	*pack >> GUID >> Set >> Reverse;

	if(GUID == gNetwork.GetGUID())
		return true;

	XMesh *mesh = (XMesh *)gPlayers.Find(GUID);

	if(Reverse == 1)
		mesh->PlaySet(Set,0,1.0f,-1.0f);
	else
		mesh->PlaySet(Set,0,1.0f,1.0f);
	mesh->ResetTime();
	return true;
}

bool MovementHandler::Update()
{
	for(PlayerInterface::Iterator it = gPlayers.Begin(); it != gPlayers.End(); it++) {
		Object *obj = (Object *)(*it).second;
		NxVec3 Pos = obj->Snapshots().GetPosition(gNetwork.GetTickRate());
		NxVec3 View = obj->Snapshots().GetView(gNetwork.GetTickRate());
		if(Pos.magnitudeSquared() != 0)
			obj->SetPosition(Pos);
		if(View.magnitudeSquared() != 0)
			obj->SetView(View);

		/*
		gLog << " Control Points: " 
			<< "One[" << (*Points[0])->Position.x << "," << (*Points[0])->Position.y << "," << (*Points[0])->Position.z 
			<< "] Two[" << (*Points[1])->Position.x << "," << (*Points[1])->Position.y << "," << (*Points[1])->Position.z 
			<< "] Three[" << (*Points[2])->Position.x << "," << (*Points[2])->Position.y << "," << (*Points[2])->Position.z 
			<< "] Four[" << (*Points[3])->Position.x << "," << (*Points[3])->Position.y << "," << (*Points[3])->Position.z << "]";*/
//		gLog << " Set[" << obj->GetPosition().x << "," << obj->GetPosition().y << "," << obj->GetPosition().z << "] ";
		obj->Update();
	}

	for(ObjectInterface::Iterator it = gObjects.Begin(); it != gObjects.End(); it++) {
		Object *obj = (*it).second;
		NxVec3 Pos = obj->Snapshots().GetPosition(gNetwork.GetTickRate());
		if(Pos.magnitudeSquared() != 0)
			obj->SetPosition(Pos);
		obj->Update();
	}

	//obj->Update();
	
	return true;
}

bool MovementHandler::Handle(Packet *pack, Client *cli)
{
	switch(pack->GetOpCode())
	{
	case SMSG_OBJECT_UPDATE:
		return HandleSMSG_OBJECTUPDATE(pack, cli);
	case SMSG_PLAYERANIMATION:
		return HandleSMSG_PLAYERANIMATION(pack,cli);
	}

	return false;
}