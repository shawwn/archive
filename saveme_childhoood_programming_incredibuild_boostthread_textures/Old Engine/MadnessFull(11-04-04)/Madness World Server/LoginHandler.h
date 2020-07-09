#ifndef _LOGINHANDLER_H
#define _LOGINHANDLER_H

#include "PacketHandler.h"

class LoginHandler : public PacketHandler
{
private:
	bool HandleCMSG_PREP_CONNECTION(Packet *packet, Client *cli);
	bool HandleCMSG_AUTH_WORLD(Packet *packet, Client *cli);
public:
	void Register();
	bool Handle(Packet *packet, Client *cli);
};

#endif