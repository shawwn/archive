#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#include <string>
#include <vector>
#include "ClientInterface.h"
#include "ServerDependencies.h"
#include "Packet.h"
#include "Object.h"
#include "Thread.h"

/* Handlers */
#include "LoginHandler.h"
#include "MovementHandler.h"
#include "ObjectHandler.h"

#pragma comment(lib,"ws2_32.lib")

#define MADNESS_SERVER
//#define MADNESS_SERVER_USE_DIRECT3D

class ServerSet
{
private:
	std::vector<fd_set> mfds;
	UINT mCurrentSet, mSocketsInCurrentSet;
public:
	ServerSet() : mCurrentSet(0), mSocketsInCurrentSet(0) { }
	//~ServerSet() { for(UINT i = 0; i < NumSets(); i++) { SAFE_DELETE(mfds[i]); } }

	void Zero();
	void Set(SOCKET sock);
	bool IsSet(SOCKET sock);
	inline fd_set &GetSet(UINT Which) { return (Which < NumSets() ? mfds[Which] : mfds[0]); }

	inline UINT NumSets() const { return (UINT)mfds.size(); }
};

class ServerInterface : public Thread
{
private:
	LoginHandler mLoginHandler;
	MovementHandler mMovementHandler;
	ObjectHandler mObjectHandler;

	float mNetworkClock;
	UINT mTick;
	Byte mTickRate;

	SOCKET mListenSocket;
	Word mListenPort;
	bool mDone;
	int AddrSize;

	void RegisterHandlers();

	void NoDelay(SOCKET sock);

	void Fill(sockaddr_in &Structure, Word Port);
	void Cleanup();

	bool HandleSockets();
	bool Read(Client *cli);
	bool Write(Client *cli);
	ClientInterface::Iterator RemoveClient(Client *cli);

	void HandleClientData(Client *cli);

	void ProcessTick();
	void ThreadProc();
public:
	UINT mCounter;
	ServerInterface() : mTick(0), mTickRate(33), mCounter(0), mListenSocket(INVALID_SOCKET), mNetworkClock(0), mListenPort(0), mDone(false), AddrSize(sizeof(sockaddr_in)) { }
	~ServerInterface() { Stop(); }

	bool Run(Word Port);
	bool SendPacket(Packet *pack, Client *cli);
	bool SendToPlayersInRange(Packet *pack, Object *obj);
	bool SendToAllPlayers(Packet *pack);
	void Stop();
	void Stop(const std::string &Stack);

	inline float GetNetworkTime() const { return mNetworkClock; }
	inline UINT GetTick() const { return mTick; }
	inline Byte GetTickRate() const { return mTickRate; }
};

extern ServerInterface gServer;

#endif