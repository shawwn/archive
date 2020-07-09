#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

#include "HandlerInterface.h"

class LoginHandler : public PacketHandler
{
private:
	bool HandleCMSG_DISCONNECT(Packet *pack, Client *cli);
	bool HandleCMSG_LOGIN(Packet *pack, Client *cli);
public:
	LoginHandler() { }
	void Register();
	bool Handle(Packet *pack, Client *cli);
	bool Update();

};

#endif