#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include "PacketHandler.h"

class NetworkHandler : public PacketHandler
{
private:
	bool HandleSMSG_NEWPLAYER(Packet *pack, Client *cli);
	bool HandleSMSG_PLAYERDISCONNECT(Packet *pack, Client *cli);
	bool HandleSMSG_LOGIN(Packet *pack, Client *cli);
	bool HandleSMSG_PING(Packet *pack, Client *cli);
	bool HandleSMSG_TICK_SYNC(Packet *pack, Client *cli);

public:
	NetworkHandler() { }
	void Register();
	bool Handle(Packet *pack, Client *cli);
	bool Update();
};

#endif