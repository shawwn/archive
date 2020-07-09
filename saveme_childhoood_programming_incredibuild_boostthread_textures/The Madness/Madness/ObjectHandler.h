#ifndef OBJECTHANDLER_H
#define OBJECTHANDLER_H

#include "PacketHandler.h"

class ObjectHandler : public PacketHandler
{
private:
	bool HandleSMSG_OBJECT_CREATE(Packet *pack, Client *cli);
	bool HandleSMSG_OBJECT_DESTROY(Packet *pack, Client *cli);
public:
	void Register();
	bool Handle(Packet *pack, Client *cli);
};

#endif