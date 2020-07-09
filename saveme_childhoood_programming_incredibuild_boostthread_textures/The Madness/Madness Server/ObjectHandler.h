#ifndef OBJECTHANDLER_H
#define OBJECTHANDLER_H

#include "PacketHandler.h"

class ObjectHandler : public PacketHandler
{
private:
	bool HandleCMSG_OBJECT_CREATE(Packet *pack, Client *cli);
	bool HandleCMSG_OBJECT_CLEARALL(Packet *pack, Client *cli);
public:
	void Register();
	bool Handle(Packet *pack, Client *cli);
};

#endif