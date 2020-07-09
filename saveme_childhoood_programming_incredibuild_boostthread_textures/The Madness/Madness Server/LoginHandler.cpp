#include "PlayerInterface.h"
#include "LoginHandler.h"
#include "HandlerInterface.h"
#include "LoggingInterface.h"
#include "NovodexInterface.h"
#include "ServerInterface.h"
#include "DatabaseInterface.h"
#include "ObjectInterface.h"
#include "GUIDInterface.h"
using namespace std;

void LoginHandler::Register()
{
	gHandlers.RegisterHandler(CMSG_LOGIN,(PacketHandler *)this);
	gHandlers.RegisterHandler(CMSG_DISCONNECT,(PacketHandler *)this);
}

bool LoginHandler::HandleCMSG_DISCONNECT(Packet *pack, Client *cli)
{
	Player *ply = gPlayers.Find(cli);
	if(ply)
		gLog << "Player [" << ply->GetName() << "]: Graceful disconnect.\n";
	else
		gLog << "Client [" << cli->AddressString() << "]: Graceful disconnect.\n";
	gPlayers.Remove(ply);
	cli->Close();
	return true;
}

bool LoginHandler::HandleCMSG_LOGIN(Packet *pack, Client *cli)
{
	
	Packet retpack;	
	Player *ply;

	string Username;
	string Password;
	*pack >> Username >> Password;

	/*
	ply = gPlayers.Find(Username);
	if(ply) {
		retpack.Build(SMSG_LOGIN) << (Byte)4;
		gServer.SendPacket(&retpack,cli);
		return true;
	}

	DatabaseEntry *Ent = gDatabase.FindUsername(Username);
	if(Ent) {
		if(Ent->Password != Password || Password == "") {
			retpack.Build(SMSG_LOGIN) << (Byte)3;
			gServer.SendPacket(&retpack,cli);
			return true;
		}
	}
	else
	{
		gDatabase.AddEntry(Username,Username,Password);
	}*/
	

	ply = new Player(gGUID.GetGUID());
	ply->SetClient(cli);
	NxVec3 Pos = NxVec3(0,60.0f,0);
	ply->CreateDefaultActor(gPhysics.GetScene());
	ply->SetName(Username);
	ply->SetPosition(Pos);
	gPlayers.Add(ply);

	gLog << "Client[" << cli->AddressString() << "]: Logged in.  Player [" << Username << "] created with GUID [" << ply->GetGUID() << "].\n";

	retpack.Build(SMSG_LOGIN);
	retpack << (Byte)1 << gServer.GetTickRate() << ply->GetGUID() << ply->GetPosition();
	gServer.SendPacket(&retpack,cli);

	retpack.Build(SMSG_NEWPLAYER);
	retpack << ply->GetGUID() << Username << Pos;
	for(PlayerInterface::Iterator it = gPlayers.Begin(); it != gPlayers.End(); it++) {
		Player *ply = (*it).second;
		gServer.SendPacket(&retpack,ply->GetClient());
	}

	for(PlayerInterface::Iterator it = gPlayers.Begin(); it != gPlayers.End(); it++) {
		Player *ply = (*it).second;
		retpack.Build(SMSG_NEWPLAYER);
		retpack << ply->GetGUID() << ply->GetName() << ply->GetPosition();
		gServer.SendPacket(&retpack,cli);		
	}

	for(ObjectInterface::Iterator it = gObjects.Begin(); it != gObjects.End(); it++) {
		Object *obj = (*it).second;
		retpack.Build(SMSG_OBJECT_CREATE);
		retpack << (Word)1 << obj->GetGUID() << obj->GetName() << obj->GetPosition() << obj->GetVelocity();
		gServer.SendPacket(&retpack,cli);		
	}

	return true;
}

bool LoginHandler::Handle(Packet *pack, Client *cli)
{
	switch(pack->GetOpCode()) 
	{
	case CMSG_LOGIN:
		return HandleCMSG_LOGIN(pack,cli);
	case CMSG_DISCONNECT:
		return HandleCMSG_DISCONNECT(pack,cli);
	}
	return false;
}

bool LoginHandler::Update()
{
	return true;
}