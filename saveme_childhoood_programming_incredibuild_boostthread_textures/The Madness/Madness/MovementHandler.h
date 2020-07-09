#ifndef MOVEMENTHANDLER_H
#define MOVEMENTHANDLER_H

#include "Object.h"
#include "Macros.h"
#include "HandlerInterface.h"
#include "Types.h"
#include <list>

class MovementHandler : public PacketHandler
{
private:
	bool HandleSMSG_OBJECTUPDATE(Packet *pack, Client *cli);
	bool HandleSMSG_PLAYERANIMATION(Packet *pack, Client *cli);
public:
	~MovementHandler() { }
	void Register();
	bool Handle(Packet *pack, Client *cli);
	bool Update();
};

#endif