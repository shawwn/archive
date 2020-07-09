#include "NetworkInterface.h"
#include "NovodexInterface.h"
#include "LoggingInterface.h"
#include "HandlerInterface.h"
#include "ClientSettingsInterface.h"
#include "TimeInterface.h"
#include "ObjectInterface.h"

NetworkInterface gNetwork;

void NetworkInterface::RegisterHandlers()
{
	mNetworkHandler.Register();
	mMovementHandler.Register();
	mObjectHandler.Register();
}

void NetworkInterface::Fill(sockaddr_in &Structure, const std::string &Address, Word Port)
{
	ZeroMemory(&Structure,sizeof(sockaddr_in));
    Structure.sin_family = AF_INET;
	Structure.sin_addr.s_addr = inet_addr(Address.c_str());
	Structure.sin_port = htons(Port);
}

bool NetworkInterface::Connect(Client *cli)
{
	if(!cli)
		return false;
	int ret = connect(cli->GetSocket(),(sockaddr *)&cli->GetSockaddr(),mAddrSize);
	if(ret == SOCKET_ERROR) 
		return false;

	RegisterHandlers();

	return true;
}

bool NetworkInterface::Read(Client *cli)
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
	//gLog << "Recv'd [" << ret << "] bytes of data.\n";
	HandleServerData();
	return true;
}

bool NetworkInterface::Write(Client *cli)
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

void NetworkInterface::HandleServerData()
{
	Packet pack;
	while(pack.FromInput(mServer->GetInBuffer())) {
		//gLog << "Networking: Recieved packet [" << strOpCodes[pack.GetOpCode()] << "] length [" << pack.GetLength() << "]\n";
		UINT packlen = pack.GetLength();
		PacketHandler *handler = gHandlers.Find(pack.GetOpCode());
		if(handler)
			handler->Handle(&pack,mServer);
		else
			gLog << "Networking ERROR: No handler for packet [" << strOpCodes[pack.GetOpCode()] << "] length [" << pack.GetLength() << "]\n";
		mServer->GetInBuffer() = mServer->GetInBuffer().substr(6+packlen);
	}
}

bool NetworkInterface::SendPacket(Packet *pack)
{
	if(!pack || !mServer)
		return false;

	Word len = pack->GetLength() + sizeof(Word) + sizeof(DoubleWord);
	mServer->GetOutBuffer() = mServer->GetOutBuffer() + 
		(char)((len & 0x00FF)     ) +
		(char)((len & 0xFF00) >> 8) +
		(char)((pack->GetOpCode() & 0x000000FF)     ) +
		(char)((pack->GetOpCode() & 0x0000FF00) >> 8) +
		(char)((pack->GetOpCode() & 0x00FF0000) >> 16) +
		(char)((pack->GetOpCode() & 0xFF000000) >> 24);

	copy(pack->Access(),pack->Access()+pack->GetLength(),back_inserter(mServer->GetOutBuffer()));

	//gLog << "Sent packet[" << strOpCodes[pack->GetOpCode()] << "] length [" << pack->GetLength() << "]\n";

	return true;
}


bool NetworkInterface::Start(const std::string &Address, UINT Port)
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);

	mClose = false;

	sockaddr_in Addr;
	Fill(Addr,Address,Port);

	mServer = new Client(socket(AF_INET,SOCK_STREAM,IPPROTO_TCP),&Addr);

	if(mServer->GetSocket() == INVALID_SOCKET) {
		gLog << "Networking ERROR: Server socket invalid.\n";
		Stop();
		return false;
	}

	int flag = 1;
	int result = setsockopt(mServer->GetSocket(),IPPROTO_TCP,TCP_NODELAY,(char *)&flag,sizeof(int));
	if(result < 0) {
		gLog << "Networking: Failed to set TCP_NODELAY.\n";
		return false;
	}

	if(!Connect(mServer)) {
		gLog << "Networking: Failed to connect to [" << mServer->AddressString() << "]\n";
		return false;
	}

	StartupData();

	return true;
}

void NetworkInterface::StartupData()
{
	Packet pack(CMSG_LOGIN);
	pack << gSettings.Username << gSettings.Password;
	SendPacket(&pack);
}

bool NetworkInterface::Update()
{
	if(!mServer)
		return false;

	gLog << "Tick[" << gNetwork.GetTick() << "] TotalRecv[" << mServer->GetTotalRecvBytes() << "] TotalSent[" << mServer->GetTotalSentBytes() << "] RecvBPS[" << mServer->GetRecvBytesPerSecond() << "] SentBPS[" << mServer->GetSentBytesPerSecond() << "] NumObjects[" << gObjects.Size() << "]"; 
	SetWindowText(gSettings.hWnd,gLog.GetStream().str().c_str());
	gLog.GetStream().str(std::string());

	for(UINT i = 0; i < gHandlers.NumHandlers(); i++) {
		gHandlers.FindHandler(i)->Update();
	}

	mNetworkClock += gTime.GetDeltaTimeInSeconds();
	
	FD_SET rfds, wfds;
	
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);

	FD_SET(mServer->GetSocket(),&rfds);
	if(!mServer->GetOutBuffer().empty())
		FD_SET(mServer->GetSocket(),&wfds);

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	int ret = select(FD_SETSIZE,&rfds,&wfds,NULL,&tv);
	if(ret < 0) {
		gLog << "Networking ERROR: Select returned <0.\n";
		Stop();
		return false;
	}

	if(ret == 0) {
		return true;
	}

	if(FD_ISSET(mServer->GetSocket(),&rfds)) {
		if(!Read(mServer)) {
			Stop("Read()");
			Cleanup();
			return false;
		}
	}

	if(FD_ISSET(mServer->GetSocket(),&wfds)) {
		if(!Write(mServer)) {
			Stop("Write()");
			Cleanup();
			return false;
		}
	}

	if(mClose && mServer->GetOutBuffer().empty()) {
		Cleanup();
		return false;
	}

	for(PlayerInterface::Iterator it = gPlayers.Begin(); it != gPlayers.End(); it++) {
		Player *ply = (*it).second;
		Light *lit = (mLights.find(ply) != mLights.end() ? mLights.find(ply)->second : NULL);
		if(!ply || !lit)
			continue;
		lit->Position = NV3TODV3(ply->GetPosition());
	}

	return true;
}

void NetworkInterface::Cleanup()
{
	closesocket(mServer->GetSocket());
	SAFE_DELETE(mServer);

	WSACleanup();
}

void NetworkInterface::Stop() 
{
	gLog << "Networking: Stop called.\n";
	mClose = true;
	Packet pack(CMSG_DISCONNECT);
	SendPacket(&pack);
}


void NetworkInterface::Stop(const std::string &Stack) 
{
	gLog << "Networking: Stop called by [" << Stack << "]\n";
	mClose = true;
	Packet pack(CMSG_DISCONNECT);
	SendPacket(&pack);
}

