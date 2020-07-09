#ifndef _LOGINHANDLER_H
#define _LOGINHANDLER_H

#include "PacketHandler.h"

class LoginHandler : public PacketHandler
{
private:
	QuadWord mNextGUID;

	bool HandleCMSG_AUTH_SESSION(Packet *packet, Client *cli);
	bool HandleCMSG_PICK_WORLDSERVER(Packet *packet, Client *cli);
	bool HandleSMSG_PREP_CONNECTION(Packet *packet, Client *cli);
public:
	LoginHandler();
	~LoginHandler();

	void Register();
	bool Handle(Packet *packet, Client *cli);
};

#endif
