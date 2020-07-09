#include "PlayerInterface.h"
#include "MovementHandler.h"
#include "ServerInterface.h"
#include "TimeInterface.h"
#include "LoggingInterface.h"
#include "GameInterface.h"
#include "ObjectInterface.h"

void MovementHandler::Register()
{
	gHandlers.RegisterHandler(CMSG_PLAYER_INPUT,(PacketHandler *)this);
	gHandlers.RegisterHandler(CMSG_ADJUSTVIEW,(PacketHandler *)this);
}

bool MovementHandler::HandleCMSG_PLAYER_INPUT(Packet *pack, Client *cli)
{
	Player *ply = gPlayers.Find(cli);
	if(!ply)
		return false;
	NxVec3 Direction;
	*pack >> Direction;

	ply->SetMovementDirection(Direction * 50.0f);

	return true;
}

bool MovementHandler::HandleCMSG_ADJUSTVIEW(Packet *pack, Client *cli)
{
	Player *ply = gPlayers.Find(cli);
	if(!ply)
		return false;
	NxVec3 View;
	*pack >> View;
	ply->SetView(View);
	return true;
}

bool MovementHandler::Handle(Packet *pack, Client *cli)
{
	switch(pack->GetOpCode())
	{
	case CMSG_PLAYER_INPUT:
		return HandleCMSG_PLAYER_INPUT(pack,cli);
	case CMSG_ADJUSTVIEW:
		return HandleCMSG_ADJUSTVIEW(pack,cli);
	}
	return true;
}

bool MovementHandler::Update()
{
	gEngine.GetPlayer().GetObject().Update();
	for(PlayerInterface::Iterator it = gPlayers.Begin(); it != gPlayers.End(); it++) {
		Player *ply = (*it).second;
		ply->Update();

		NxVec3 Anim = ply->GetVelocity();
		Anim.y = 0.0f;	
		Byte Reverse;

		if(ply->GetVelocity().dot(ply->GetView()) < 0)
			Reverse = 1;
		else
			Reverse = 0;
		

		if(Anim.magnitude() < 1.0f && ply->mAnimation != 3) {	
			Packet animpack(SMSG_PLAYERANIMATION);
			animpack << ply->GetGUID() << (Byte)3 << (Byte)0;
			gServer.SendToPlayersInRange(&animpack,ply);
			ply->mAnimation = 3;
		}

		if(Anim.magnitude() >= 1.0f && Anim.magnitude() < 10.0f && ply->mAnimation != 2) {
			Packet animpack(SMSG_PLAYERANIMATION);
			animpack << ply->GetGUID() << (Byte)2 << Reverse;
			gServer.SendToPlayersInRange(&animpack,ply);
			ply->mAnimation = 2;

		}

		if(Anim.magnitude() >= 10.0f && Anim.magnitude() < 50.0f && ply->mAnimation != 1) {
			Packet animpack(SMSG_PLAYERANIMATION);
			animpack << ply->GetGUID() << (Byte)1 << Reverse;
			gServer.SendToPlayersInRange(&animpack,ply);
			ply->mAnimation = 1;

		}
	}

	for(ObjectInterface::Iterator it = gObjects.Begin(); it != gObjects.End(); it++) {
		Object *obj = (*it).second;
		obj->Update();
	}

	return true;
}

void MovementHandler::SendUpdates()
{

}