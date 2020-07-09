#ifndef _CHATHANDLER_H
#define _CHATHANDLER_H

#include "PacketHandler.h"

class ChatHandler : public PacketHandler
{
private:
	bool HandleCMSG_CHAT_SAY(Packet *packet, Client *cli);
public:
	void Register();
	bool Handle(Packet *packet, Client *cli);
};

#endif