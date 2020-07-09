#ifndef MOVEMENTHANDLER_H
#define MOVEMENTHANDLER_H

#include "HandlerInterface.h"

class MovementHandler : public PacketHandler
{
private:
	bool HandleCMSG_PLAYER_INPUT(Packet *pack, Client *cli);
	bool HandleCMSG_ADJUSTVIEW(Packet *pack, Client *cli);
public:
	void Register();
	bool Handle(Packet *pack, Client *cli);
	bool Update();

	void SendUpdates();
};

#endif