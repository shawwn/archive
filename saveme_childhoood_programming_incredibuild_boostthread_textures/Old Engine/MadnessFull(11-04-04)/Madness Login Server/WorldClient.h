#ifndef _WORLDCLIENT_H
#define _WORLDCLIENT_H

#include "Client.h"

class WorldClient : public Client
{
private:
	string mName;
	Word mPort;
	DoubleWord mIP;
public:
	WorldClient();
	~WorldClient();

	void SetName(const string &Name) { mName = Name; }
	void SetPort(const Word &Port) { mPort = Port; }
	void SetIP(const DoubleWord &IP) { mIP = IP; }

	const string &GetName() { return mName; }
	const Word &GetPort() { return mPort; }
	DoubleWord GetNetworkIP() { return mIP; }
};

#endif
