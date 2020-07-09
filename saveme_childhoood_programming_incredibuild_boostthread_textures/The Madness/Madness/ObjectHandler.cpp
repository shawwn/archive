#include "ObjectHandler.h"
#include "NetworkInterface.h"
#include "LoggingInterface.h"
#include "ClientSettingsInterface.h"
#include "PlayerInterface.h"
#include "HandlerInterface.h"
#include "NovodexInterface.h"
#include "ObjectInterface.h"
#include "Packet.h"
using namespace std;

void ObjectHandler::Register()
{
	gHandlers.RegisterHandler(SMSG_OBJECT_CREATE,(PacketHandler *)this);
	gHandlers.RegisterHandler(SMSG_OBJECT_DESTROY,(PacketHandler *)this);
}

bool ObjectHandler::HandleSMSG_OBJECT_CREATE(Packet *pack, Client *cli)
{
	Word Type;
	QuadWord GUID;
	NxVec3 Pos;
	NxVec3 Vel;
	string Name;

	*pack >> Type;

	Object *obj;
	switch(Type)
	{
	case 1:
		*pack >> GUID >> Name >> Pos >> Vel;
		obj = new Object(GUID);
		obj->SetName(Name);
		obj->CreateDefaultActor(gPhysics.GetScene());
		obj->SetPosition(Pos);
		obj->SetVelocity(Vel);
		gObjects.Add(obj);
		break;
	default:
		return false;
	}

	return true;
}

bool ObjectHandler::HandleSMSG_OBJECT_DESTROY(Packet *pack, Client *cli)
{
	DoubleWord Size;
	*pack >> Size;

	for(UINT i = 0; i < Size; i++) {
		QuadWord GUID;
		*pack >> GUID;
		Object *obj = gObjects.Find(GUID);
		if(obj)
			gObjects.Remove(obj);
	}

	return true;
}

bool ObjectHandler::Handle(Packet *pack, Client *cli)
{
	switch(pack->GetOpCode())
	{
	case SMSG_OBJECT_CREATE:
		return HandleSMSG_OBJECT_CREATE(pack,cli);
	case SMSG_OBJECT_DESTROY:
		return HandleSMSG_OBJECT_DESTROY(pack,cli);
	}
	return false;
}