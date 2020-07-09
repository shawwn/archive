#ifndef _PACKETHANDLER_H
#define _PACKETHANDLER_H

#include "Packet.h"
#include "Bsd.h"

class PacketHandler
{
public:
	PacketHandler();
	virtual ~PacketHandler();

	virtual bool Handle(Packet *packet, Client *client) = 0;
};

#endif
