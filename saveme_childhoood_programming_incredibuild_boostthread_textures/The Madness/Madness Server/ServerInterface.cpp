#define NOMINMAX
#include "ServerInterface.h"
#include "NovodexInterface.h"
#include "LoggingInterface.h"
#include "HandlerInterface.h"
#include "TimeInterface.h"
#include "PlayerInterface.h"
#include "DatabaseInterface.h"
#include "ObjectInterface.h"
#include "PlayerInterface.h"
#include "Parsing.h"
#include "GameInterface.h"
#include "Packet.h"
#include "ContactReport.h"
#include <sstream>
#include <algorithm>
using namespace std;

ServerInterface gServer;

void ServerSet::Zero()
{
	for(UINT i = 0; i < NumSets(); i++) {
		FD_ZERO(&mfds[i]);
	}
	mCurrentSet = 0;
	mSocketsInCurrentSet = 0;
}


void ServerSet::Set(SOCKET sock)
{
	while(mCurrentSet >= NumSets()) {
		mfds.push_back(fd_set());
		FD_ZERO(&mfds[NumSets()-1]);
	}

	FD_SET(sock,&mfds[mCurrentSet]);
	mSocketsInCurrentSet++;
	if(mSocketsInCurrentSet == FD_SETSIZE) {
		mCurrentSet++;
		mSocketsInCurrentSet = 0;
	}
}

bool ServerSet::IsSet(SOCKET sock)
{
	for(UINT i = 0; i < NumSets(); i++) {
		if(FD_ISSET(sock,&mfds[i]))
			return true;
	}
	return false;
}

void ServerInterface::RegisterHandlers()
{
	mLoginHandler.Register();
	mMovementHandler.Register();
	mObjectHandler.Register();
}

void ServerInterface::NoDelay(SOCKET sock)
{
	int flag = 1;
	int result = setsockopt(sock,IPPROTO_TCP,TCP_NODELAY,(char *)&flag,sizeof(int));
	if(result < 0) {
		gLog << "Networking: Failed to set TCP_NODELAY.\n";
	}
}

void ServerInterface::Fill(sockaddr_in &Structure, Word Port)
{
	ZeroMemory(&Structure,sizeof(sockaddr_in));
    Structure.sin_family = AF_INET;
	Structure.sin_addr.s_addr = INADDR_ANY;
	Structure.sin_port = htons(Port);
}

void ServerInterface::Cleanup()
{
	netclose(mListenSocket);

	WSACleanup();
}

void ServerInterface::Stop()
{
	mDone = true;
}

void ServerInterface::Stop(const std::string &Stack)
{
	gLog << "Server: Stop called by [" << Stack << "]\n";
	Stop();
}

void ServerInterface::ThreadProc()
{
	while(!mDone)
	{
		HandleSockets();
	}
}

bool ServerInterface::HandleSockets()
{
	fd_set rfds, wfds;
	ClientInterface::Iterator it;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);

	FD_SET(mListenSocket,&rfds);

	for(it = gClients.Begin(); it != gClients.End(); it++) {
		Client *cli = (*it);
		FD_SET(cli->GetSocket(),&rfds);
		if(!cli->GetOutBuffer().empty()) {
	
			FD_SET(cli->GetSocket(),&wfds);
		}
	}

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;	

	//gLog << "Time is now [" << gTime.GetDeltaTime() << "]\n";

	int ret = select(FD_SETSIZE,&rfds,&wfds,NULL,&tv);
	if(ret < 0) {
		gLog << "Server ERROR: Select error.\n";
		return false;
	}
	if(ret == 0)
		return true;

	if(FD_ISSET(mListenSocket,&rfds)) {
		sockaddr_in Addr;
		SOCKET sock = accept(mListenSocket,(sockaddr *)&Addr,&AddrSize);
		NoDelay(sock);
		Client *cli = new Client(sock,&Addr);
		gLog << "Client [" << cli->AddressString() << "]: Connected.\n";
		gClients.Add(cli);
	}

	for(it = gClients.Begin(); it != gClients.End(); it++) {
		Client *cli = *it;
		if(FD_ISSET(cli->GetSocket(),&rfds)) {
			if(!Read(cli)) {
				it = RemoveClient(cli);
				continue;
			}
		}

		if(FD_ISSET(cli->GetSocket(),&wfds)) {
			gServer.mCounter++;
			if(!Write(cli)) {
				it = RemoveClient(cli);
				continue;
			}
		}

		if(cli->IsClosing() && cli->GetOutBuffer().empty()) {
			it = RemoveClient(cli);
			continue;
		}
	}

	return true;
}

bool ServerInterface::Read(Client *cli)
{
	char buffer[1024];
	int ret = recv(cli->GetSocket(),buffer,1024,0);
	if(ret < 0) {
		gLog << "Server ERROR: Client[" << cli->AddressString() << "] read() returned <0.\n";
		return false;
	}
	if(ret == 0) 
		return false;

	cli->GetInBuffer().append(buffer,ret);
	cli->AddTotalRecvBytes(ret);

	//gLog << "Recvd [" << ret << "] bytes of data.\n";

	return true;
}

bool ServerInterface::Write(Client *cli)
{

	int ret = send(cli->GetSocket(),cli->GetOutBuffer().c_str(),(int)cli->GetOutBuffer().length(),0);
	if(ret < 0) {
		gLog << "Server ERROR: Client[" << cli->AddressString() << "] send() returned <0.\n";
		return false;
	}
	cli->GetOutBuffer() = cli->GetOutBuffer().substr(ret);
	cli->AddTotalSentBytes(ret);
	//gLog << "Sent [" << ret << "] bytes of data.\n";
	return true;
}

ClientInterface::Iterator ServerInterface::RemoveClient(Client *cli)
{
	gLog << "Client [" << cli->AddressString() << "]: Removed.\n";

	Player *ply = gPlayers.Find(cli);
	if(ply)
	{
		Packet retpack(SMSG_PLAYERDISCONNECT);
		retpack << ply->GetGUID();
		SendToAllPlayers(&retpack);
		gPlayers.Remove(ply);
	}
	return gClients.Remove(cli);
}

void ServerInterface::HandleClientData(Client *cli)
{
	Packet pack;
	while(pack.FromInput(cli->GetInBuffer())) {
		//gLog << "Packet [" << strOpCodes[pack.GetOpCode()] << "] length[" << pack.GetLength() << "] recieved.\n";
		UINT packlen = pack.GetLength();
		PacketHandler *handler = gHandlers.Find(pack.GetOpCode());
		if(!handler) {
			gLog << "Server WARNING: No handler for Packet[" << strOpCodes[pack.GetOpCode()] << "] Length[" << pack.GetLength() << "]\n";
			continue;
		}
		if(!handler->Handle(&pack,cli)) {
			gLog << "Server WARNING: Packet[" << strOpCodes[pack.GetOpCode()] << "(" << pack.GetOpCode() << ")" << "] Length[" << packlen << "] unhandled.";
			cli->Close(gLog.GetStream().str());
			gLog << "\n";
			break;
		}
		cli->GetInBuffer() = cli->GetInBuffer().substr(6+packlen);
	}
}

void UpdateObject(Object *obj)
{
	float PositionUpdate = 0.1f;
	float VelocityUpdate = 1.0f;
	float AccelerationUpdate = 1.0f;
	Kinematic &Previous = obj->GetPrevious();
	Byte flags = 0;
	//if(Reckon(Previous.Position,obj->GetPosition(),PositionUpdate)) { flags |= OBJECT_POSITION; Previous.Position = obj->GetPosition(); }
	//if(Reckon(Previous.Velocity,obj->GetVelocity(),VelocityUpdate)) { flags |= OBJECT_VELOCITY; Previous.Velocity = obj->GetVelocity(); }
	//if(Reckon(Previous.Acceleration,obj->GetAcceleration(),AccelerationUpdate)) { flags |= OBJECT_ACCELERATION; Previous.Acceleration = obj->GetAcceleration(); }

	if(Reckon(Previous.Position,obj->GetPosition(),PositionUpdate) || ReckonAngle(Previous.View,obj->GetView(),0.01f)) {
		flags |= OBJECT_POSITION; Previous.Position = obj->GetPosition(); 
		flags |= OBJECT_VELOCITY; Previous.Velocity = obj->GetVelocity(); 
		flags |= OBJECT_ACCELERATION; Previous.Acceleration = obj->GetAcceleration(); 
		Previous.View = obj->GetView();
	}
	if(flags != 0) {
		Packet pack;
		obj->GetKinematicPacket(flags,pack);
		gServer.SendToPlayersInRange(&pack,obj);
	}
}

void ServerInterface::ProcessTick()
{
	mTick++;

	Packet syncpack(SMSG_TICK_SYNC);
	syncpack << mTick;
	SendToAllPlayers(&syncpack);

	if(!gTime.Update()) {
		gLog << "Server WARNING: Time update failed.\n";
	}

	for(ClientInterface::Iterator it = gClients.Begin(); it != gClients.End(); it++) {
		HandleClientData((*it));
	}

	for(UINT i = 0; i < gHandlers.NumHandlers(); i++) {
		gHandlers.FindHandler(i)->Update();
	}

	mNetworkClock += gTime.GetDeltaTimeInSeconds();

	LPNXSCENE Scene = gPhysics.GetScene();
	if(!Scene)
		return;
	Scene->simulate((NxReal)gTime.GetDeltaTime());
	Scene->flushStream();
	Scene->fetchResults(NX_RIGID_BODY_FINISHED,true);

	Object *obj;
	for(PlayerInterface::Iterator it = gPlayers.Begin(); it != gPlayers.End(); it++) {
		obj = (Object *)(*it).second;
		UpdateObject(obj);
	}

	for(ObjectInterface::Iterator it = gObjects.Begin(); it != gObjects.End(); it++) {
		obj = (*it).second;
		UpdateObject(obj);
	}
}

bool ServerInterface::SendPacket(Packet *pack, Client *cli)
{
	if(!cli || !pack)
		return false;

	cli->Send(pack);

	return true;
}

bool ServerInterface::SendToPlayersInRange(Packet *pack, Object *obj)
{
	float Range = 0;
	for(PlayerInterface::Iterator it = gPlayers.Begin(); it != gPlayers.End(); it++) {
		Player *other = (*it).second;
		if(Range == 0)
			other->GetClient()->Send(pack);
		else
		{
			if(obj->GetActor()->getGlobalPosition().distance(other->GetActor()->getGlobalPosition()) < Range)
				other->GetClient()->Send(pack);
		}
	}

	return true;
}

bool ServerInterface::SendToAllPlayers(Packet *pack)
{
	for(PlayerInterface::Iterator it = gPlayers.Begin(); it != gPlayers.End(); it++) {
		Player *ply = (*it).second;
		ply->GetClient()->Send(pack);	
	}
	return true;
}

bool ServerInterface::Run(Word Port)
{
	gLog << "Server: Session start.\n";
	mListenPort = Port;

	gTime.Initialize();
	gDatabase.Load(FindPath("database.dat"));

	Cleanup();

	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);

	if((mListenSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == INVALID_SOCKET) {
		Stop("socket()");
		Cleanup();
		return false;
	}

    NoDelay(mListenSocket);

	sockaddr_in Addr;
	Fill(Addr,mListenPort);
	
	if(bind(mListenSocket,(sockaddr *)&Addr,AddrSize) == SOCKET_ERROR) {
		Stop("bind()");
		Cleanup();
		return false;
	}

	if(netlisten(mListenSocket) == SOCKET_ERROR) {
		Stop("listen()");
		Cleanup();
		return false;
	}

	RegisterHandlers();

	if(!gPhysics.Initialize(&gContact)) {
		gLog << "Server ERROR: Could not initialize physics.\n";
		return false;
	}

	gLog << "Server listening on port [" << mListenPort << "]\n";

	//BeginThread();

	gEngine.Start();
	float Time = 0.0f;
	float Rate = 1.0f / mTickRate;

	TimeInterface Sync;
	Sync.Initialize();
	while(!mDone)
	{
		//Sleep((DWORD)(1/200.0f * 1000));
		Sync.Update();

		if(Time < Rate)
			Sleep(0);
		while(Time > Rate) {
			gEngine.Loop();
			ProcessTick();
			Time -= Rate;
			if(!HandleSockets())
				Stop();
		}
		if(!HandleSockets())
			Stop();
		Time += Sync.GetDeltaTimeInSeconds();
		
	}

	gEngine.Deinitialize();

	Cleanup();

	gLog << "Server: Session end.\n";

	return true;
}