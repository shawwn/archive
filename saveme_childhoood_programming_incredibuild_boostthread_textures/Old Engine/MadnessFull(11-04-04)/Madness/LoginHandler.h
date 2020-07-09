#ifndef _LOGINHANDLER_H
#define _LOGINHANDLER_H

#include "PacketHandler.h"

class LoginHandler : public PacketHandler
{
private:
	bool HandleSMSG_AUTH_RESPONSE(Packet *packet, Client *cli);
	bool HandleSMSG_PICK_WORLDSERVER(Packet *packet, Client *cl);
	bool HandleSMSG_AUTH_WORLD(Packet *packet, Client *cli);
public:
	void Register();
	bool Handle(Packet *packet, Client *cli);
};

#endif