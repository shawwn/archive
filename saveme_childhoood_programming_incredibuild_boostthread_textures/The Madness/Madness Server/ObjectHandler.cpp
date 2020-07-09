#include "ObjectHandler.h"
#include "ServerInterface.h"
#include "LoggingInterface.h"
#include "PlayerInterface.h"
#include "HandlerInterface.h"
#include "GUIDInterface.h"
#include "NovodexInterface.h"
#include "ObjectInterface.h"
#include "Packet.h"

void ObjectHandler::Register()
{
	gHandlers.RegisterHandler(CMSG_OBJECT_CREATE,(PacketHandler *)this);
	gHandlers.RegisterHandler(CMSG_OBJECT_CLEARALL,(PacketHandler *)this);
}

bool ObjectHandler::HandleCMSG_OBJECT_CREATE(Packet *pack, Client *cli)
{
	Word Type;
	NxVec3 Pos, Vel;
	*pack >> Type >> Vel;

	Object *obj;
	Player *ply = gPlayers.Find(cli);
	if(!ply)
		return false;

	NxVec3 CreatePos;

	Packet retpack(SMSG_OBJECT_CREATE);
	switch(Type)
	{
	case 1:
		CreatePos = Vel;
		CreatePos.setMagnitude(6.0f);
		CreatePos = CreatePos + ply->GetPosition();

		obj = new Object(gGUID.GetGUID());
		obj->CreateDefaultActor(gPhysics.GetScene());
		obj->SetPosition(CreatePos);
		obj->SetVelocity(Vel + ply->GetVelocity());		
		gObjects.Add(obj);

		retpack << (Word)1 << obj->GetGUID() << "Rock" << CreatePos << Vel;
		break;
	default:
		return false;
	}

	gServer.SendToAllPlayers(&retpack);

	return true;
}

bool ObjectHandler::HandleCMSG_OBJECT_CLEARALL(Packet *pack, Client *cli)
{
	Packet retpack(SMSG_OBJECT_DESTROY);
	retpack << (DoubleWord)gObjects.Size();

	for(ObjectInterface::Iterator it = gObjects.Begin(); it != gObjects.End(); ) {
		Object *obj = (*it).second;
		retpack << (QuadWord)obj->GetGUID();
		it = gObjects.Remove(obj);	
	}

	gServer.SendToAllPlayers(&retpack);

	return true;
}

bool ObjectHandler::Handle(Packet *pack, Client *cli)
{
	switch(pack->GetOpCode())
	{
	case CMSG_OBJECT_CREATE:
		return HandleCMSG_OBJECT_CREATE(pack,cli);
	case CMSG_OBJECT_CLEARALL:
		return HandleCMSG_OBJECT_CLEARALL(pack,cli);
	}
	return false;
}