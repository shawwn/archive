#ifndef _SERVER_H
#define _SERVER_H

#include "Bsd.h"
#include <list>
#include <map>
#include "Client.h"
#include "Player.h"
#include "PlayerManager.h"
#include "AccountManager.h"
#include "PacketHandler.h"
#include "BasicNetworking.h"
using namespace std;

#ifdef WIN32
#pragma comment(lib,"ws2_32.lib")
#endif

class Server : public BasicNetworking
{
protected:
	sockaddr_in mInfo;
	SOCKET mListen;
	std::list<Client *> mClients;
	int mClientCount;
	fd_set mRead;
	bool mRunning;

	map<DoubleWord, PacketHandler *> mPacketHandlers;
	AccountManager mAccounts;
	PlayerManager mPlayers;

	virtual bool Initialize();
	virtual void Deinitialize();
	bool Start();
	bool Update();
	virtual bool CustomUpdate();
	void Destroy();
	bool Listen(const int &Port);
	bool Accept();
	bool Read(Client *cli);
	bool Write(Client *cli);
	bool Disconnect(Client *cli);
	bool Select();
	Client *Connect(const char *IP, const int &Port);

	void ClientConnected(Client *cli);
	void ClientDisconnected(Client *cli);
	void ClientDataRecvd(Client *cli);
public:
	typedef std::list<Client *>::iterator Iterator;

	Server();
	~Server();

	bool Run();
	bool Stop();
	void RegisterHandler(DoubleWord op, PacketHandler *handler);
	void WriteData(Client *cli, const unsigned char byte);
	void WriteData(Client *cli, const unsigned char *bytes, unsigned int len);
	void WriteData(Client *cli, Packet *pack);
	void SendPacketTo(Player *ply, Packet *pack);
	
	Iterator Begin() { return mClients.begin(); }
	Iterator End() { return mClients.end(); }
	int Count() { return mClientCount; }
	const bool &Running() { return mRunning; }
	ConfigurationParsingInterface &Configuration() { return mConfiguration; }

	AccountManager &Accounts() { return mAccounts; }
	PlayerManager &Players() { return mPlayers; }
};

//extern Server *gServer;

#endif
