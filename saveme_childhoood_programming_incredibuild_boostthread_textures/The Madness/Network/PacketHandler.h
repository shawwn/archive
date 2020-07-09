#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include "Client.h"
#include "Packet.h"

class PacketHandler
{
public:
	virtual bool Handle(Packet *pack, Client *cli) { return true; }
	virtual bool Update() { return true; }
};

#endif