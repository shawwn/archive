#ifndef _NETWORKING_H
#define _NETWORKING_H

#include <list>
#include <map>
#include "Client.h"
#include "Player.h"
#include "PacketHandler.h"
#include "Bsd.h"
#include "BasicNetworking.h"
using namespace std;

#ifdef WIN32
#pragma comment(lib,"ws2_32.lib")
#endif

class Networking : public BasicNetworking
{
protected:
	typedef std::list<Client *>::iterator Iterator;

	sockaddr_in mInfo;
	Client *mServer;
	int mClientCount;
	fd_set mRead;
	bool mRunning;

	map<DoubleWord, PacketHandler *> mPacketHandlers;

	virtual bool Initialize();
	virtual void Deinitialize();
	bool Read();
	bool Write();
	bool Disconnect();
	bool Select();

	void ServerDataRecvd();
public:
	Networking();
	~Networking();

	bool Start(const string &ConfigFile);
	bool Connect(const string &IP, const int &Port);
	bool Update();
	bool Stop();
	void RegisterHandler(DoubleWord op, PacketHandler *handler);
	void WriteData(const unsigned char byte);
	void WriteData(const unsigned char *bytes, unsigned int len);
	void WriteData(Packet *pack);
	void Destroy();

	int Count() { return mClientCount; }
	bool Running() { return mRunning; }
};

#endif
