#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#define NOMINMAX
#include "Winsock2.h"
#include "Packet.h"
#include "Client.h"
#include "Macros.h"
#include "Types.h"
#include "NetworkHandler.h"
#include "MovementHandler.h"
#include "PlayerInterface.h"
#include "LightingInterface.h"
#include "ObjectHandler.h"
#include <map>

#pragma comment(lib,"ws2_32.lib")

class NetworkInterface
{
private:
	MovementHandler mMovementHandler;
	NetworkHandler mNetworkHandler;
	ObjectHandler mObjectHandler;

	Byte mTickRate;
	UINT mNetworkTick;
	QuadWord mGUID;
	float mNetworkClock;
	bool mClose;

	Client *mServer;
	int mAddrSize;

	void RegisterHandlers();
	void Fill(sockaddr_in &Structure, const std::string &Address, Word Port);
	bool Connect(Client *cli);
	void StartupData();

	bool Read(Client *cli);
	bool Write(Client *cli);

	void HandleServerData();
	void Cleanup();

	std::map<Player *, Light *> mLights;
public:
	NetworkInterface() : mTickRate(33), mNetworkTick(0), mServer(NULL), mNetworkClock(0.0f), mClose(false), mAddrSize(sizeof(sockaddr_in)), mGUID(-1) { }
	~NetworkInterface() { SAFE_DELETE(mServer); }

	bool Start(const std::string &Address, UINT Port);
	bool Update();
	void Stop();
	void Stop(const std::string &Stack);
	bool SendPacket(Packet *pack);

	inline void SetGUID(QuadWord GUID) { mGUID = GUID; }
	inline void SetTick(UINT Tick) { mNetworkTick = Tick; }
	inline void SetTickRate(Byte Rate) { mTickRate = Rate; }

	inline QuadWord GetGUID() const { return mGUID; }
	inline float GetNetworkTime() const { return mNetworkClock; }
	inline bool IsConnected() const { return (mServer != NULL); }
	inline UINT GetTick() const { return mNetworkTick; }
	inline Byte GetTickRate() const { return mTickRate; }

	inline std::map<Player *, Light *> &NetworkLights() { return mLights; }
};

extern NetworkInterface gNetwork;

#endif