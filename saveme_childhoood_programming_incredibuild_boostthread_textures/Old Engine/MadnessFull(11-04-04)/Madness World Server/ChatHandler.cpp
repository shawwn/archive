#include "ChatHandler.h"
#include "WorldServer.h"

void ChatHandler::Register()
{
	gServer->RegisterHandler(CMSG_CHAT_SAY,(PacketHandler *)this);
}

bool ChatHandler::Handle(Packet *packet, Client *cli)
{
	switch(packet->GetOpCode())
	{
	case CMSG_CHAT_SAY:
		HandleCMSG_CHAT_SAY(packet,cli);
		return true;
	}

	return false;
}

bool ChatHandler::HandleCMSG_CHAT_SAY(Packet *packet, Client *cli)
{
	Account *acc = gServer->Accounts().Find(cli);
	if(!acc)
		return false;

	string WorldMsg = acc->GetName() + ": " + (char *)packet->GetBytes(0);
	Packet pack;
	pack.Build(SMSG_CHAT_SAY);
	pack.AddBytes((Byte *)WorldMsg.c_str(),(int)WorldMsg.length()+1);
	for(WorldServer::Iterator it = gServer->Begin(); it != gServer->End(); it++)
	{
		gServer->WriteData(*it,&pack);
	}
	return true;
}