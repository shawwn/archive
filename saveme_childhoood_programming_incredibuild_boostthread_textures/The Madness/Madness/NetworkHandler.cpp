#define NOMINMAX
#include "XMeshInterface.h"
#include "NetworkHandler.h"
#include "NovodexInterface.h"
#include "HandlerInterface.h"
#include "LoggingInterface.h"
#include "ClientSettingsInterface.h"
#include "PlayerInterface.h"
#include "LightingInterface.h"
#include "NetworkInterface.h"
#include "GameInterface.h"
#include "ClientSettingsInterface.h"
using namespace std;

void NetworkHandler::Register()
{
	gHandlers.RegisterHandler(SMSG_SHUTDOWN,(PacketHandler *)this);
	gHandlers.RegisterHandler(SMSG_NEWPLAYER,(PacketHandler *)this);
	gHandlers.RegisterHandler(SMSG_PLAYERDISCONNECT,(PacketHandler *)this);
	gHandlers.RegisterHandler(SMSG_LOGIN,(PacketHandler *)this);
	gHandlers.RegisterHandler(SMSG_PING,(PacketHandler *)this);
	gHandlers.RegisterHandler(SMSG_TICK_SYNC,(PacketHandler *)this);
}

bool NetworkHandler::HandleSMSG_NEWPLAYER(Packet *pack, Client *cli)
{
	string Name;
	NxVec3 Pos;
	QuadWord GUID;

	*pack >> GUID >> Name >> Pos;
	if(GUID == gNetwork.GetGUID())
		return true;

	//Player *ply = new XMesh(GUID);
	XMesh *ply = NULL;
	gMeshes.GetNewInstance(0,&ply);
	ply->SetGUID(GUID);
	ply->CreateDefaultActor(gPhysics.GetScene());
	ply->SetName(Name);
	ply->SetPosition(Pos);
	ply->PlaySet(3,0,1.0f);
	gPlayers.Add((Player *)ply);

	gNetwork.NetworkLights()[ply] = gLights.AddLight(NV3TODV3(Pos),0.5f);

	//if(GUID == gNetwork.GetGUID()) {
		//ply->GetActor()->raiseActorFlag(NX_AF_DISABLE_COLLISION);
	//}

	gLog << "Player [" << ply->GetName() << "]: Added at position[" << Pos.x << "," << Pos.y << "," << Pos.z << "]\n";

	return true;
}

bool NetworkHandler::HandleSMSG_LOGIN(Packet *pack, Client *cli)
{
	Byte Flag;
	Byte Rate;
	QuadWord GUID;
	NxVec3 Position;
	*pack >> Flag;

	Player *ply;
	switch(Flag)
	{
	case 1:
		*pack >> Rate >> GUID >> Position;
		gNetwork.SetGUID(GUID);
		gNetwork.SetTickRate(Rate);
		gEngine.GetPlayer().SetEye(Position);
		ply = new Player(GUID);
		ply->CreateDefaultActor(gPhysics.GetScene());
		gPlayers.Add(ply);
		break;
	case 3:
		MessageBox(NULL,"Invalid Password.","Bad Password",MB_OK);
		gSettings.Done = true;
		break;
	case 4:
		MessageBox(NULL,"Already logged in.","Logged In",MB_OK);
		gSettings.Done = true;
		break;
	}

	return true;
}

bool NetworkHandler::HandleSMSG_PLAYERDISCONNECT(Packet *pack, Client *cli)
{
	QuadWord GUID;
	*pack >> GUID;

	Player *ply = gPlayers.Find(GUID);
	gLights.ReleaseLight(gNetwork.NetworkLights().find(ply)->second);

	if(!ply)
		return false;

	gPlayers.Remove(ply);
	return true;
}

bool NetworkHandler::HandleSMSG_PING(Packet *pack, Client *cli)
{

	return true;
}

bool NetworkHandler::HandleSMSG_TICK_SYNC(Packet *pack, Client *cli)
{
	UINT Tick;
	*pack >> Tick;
	gNetwork.SetTick(Tick);
	return true;
}

bool NetworkHandler::Handle(Packet *pack, Client *cli)
{
	string Message;
	switch(pack->GetOpCode())
	{
	case SMSG_SHUTDOWN:
		*pack >> Message;
		if(Message != "") {
			Message = "Disconnected.  Reason: " + Message;
			MessageBox(NULL,Message.c_str(),"Disconnected",MB_OK);
		}
		else
		{
			MessageBox(NULL,"Disconnected.","Disconnected.",MB_OK);
		}
		gSettings.Done = true;
		return true;
	case SMSG_NEWPLAYER:
		return HandleSMSG_NEWPLAYER(pack,cli);
	case SMSG_PLAYERDISCONNECT:
		return HandleSMSG_PLAYERDISCONNECT(pack,cli);
	case SMSG_LOGIN:
		return HandleSMSG_LOGIN(pack,cli);
	case SMSG_PING:
		return HandleSMSG_PING(pack,cli);
	case SMSG_TICK_SYNC:
		return HandleSMSG_TICK_SYNC(pack,cli);
	}
	return true;
}

bool NetworkHandler::Update()
{
	return true;
}