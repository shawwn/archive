#include "dxstdafx.h"
#include "ChatHandler.h"
#include "Engine.h"
#include "ClientNetworking.h"
#include "ParsingInterface.h"

void ChatHandler::Register()
{
	gNetworking->RegisterHandler(SMSG_CHAT_SAY,(PacketHandler *)this);
}

bool ChatHandler::Handle(Packet *packet, Client *cli)
{
	switch(packet->GetOpCode())
	{
	case SMSG_CHAT_SAY:
		HandleSMSG_CHAT_SAY(packet,cli);
		return true;
	}
	return false;
}

bool ChatHandler::HandleSMSG_CHAT_SAY(Packet *packet, Client *cli)
{
	string msg = (char *)packet->GetBytes(0);

	gEngine->World().Print(StringToWideString(msg));

	return true;
}