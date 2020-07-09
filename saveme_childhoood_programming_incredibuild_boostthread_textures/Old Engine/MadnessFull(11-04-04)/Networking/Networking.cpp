#include "stdafx.h"
#include "NetworkError.h"
#include "Networking.h"
#include "LoggingInterface.h"


Networking::Networking()
{
	mServer = NULL;
	Destroy();
}

Networking::~Networking()
{
	Destroy();
}

void Networking::Destroy()
{
	mClientCount = 0;
	FD_ZERO(&mRead);
	
	if(mServer)  {
#ifdef WIN32
		closesocket(mServer->GetSocket());
#else
		close(mServer->GetSocket());
#endif
	}
	delete mServer;

	mRunning = false;

	mInfo.sin_addr.s_addr = INADDR_ANY;
	mInfo.sin_family = AF_INET;
	mInfo.sin_port = 0;

#ifdef WIN32
	WSACleanup();
#endif
}

bool Networking::Read()
{	
	int read;
	Byte buf[1024];

	while(true)
	{
		read = recv(mServer->GetSocket(),(char *)buf,sizeof(buf),0);
		if(read < 0)
		{
#ifdef WIN32
			if(WSAGetLastError() == WSAEWOULDBLOCK)
				return true;
#endif
			return false;
		}
		else if(read == 0)
			return false;

		mServer->RecvData(buf,read);

		if(read < sizeof(buf))
			break;
	}

	ServerDataRecvd();

	return true;
}

bool Networking::Initialize()
{
	return true;
}

void Networking::Deinitialize()
{
	
}

bool Networking::Write()
{
	int len = send(mServer->GetSocket(),mServer->GetOutBuffer().c_str(),(int)mServer->GetOutBuffer().length(),0);
	if(len == SOCKET_ERROR)
		return false;

	mServer->GetOutBuffer() = mServer->GetOutBuffer().substr(len);

	if (mServer->GetOutBuffer().empty() && mServer->GetClose())
		return false;
	if (mServer->GetOutBuffer().empty() && mServer->GetRemove())
		return false;

	return true;
}

bool Networking::Disconnect()
{
	mClientCount--;
	if(mServer) {
#ifdef WIN32
		closesocket(mServer->GetSocket());
#else
		close(mServer->GetSocket());
#endif
		delete mServer;
	}
	mServer = NULL;
	return true;
}

bool Networking::Select()
{	
	fd_set rfds, wfds;
	struct timeval tv;
	int cnt;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);

	if(mServer)
	{
		FD_SET(mServer->GetSocket(),&rfds);
		if(!mServer->GetOutBuffer().empty())
			FD_SET(mServer->GetSocket(),&wfds);

		cnt = select(FD_SETSIZE,&rfds,&wfds,NULL,&tv);
		if(cnt < 0)
			return false;

		if(FD_ISSET(mServer->GetSocket(),&rfds))
		{
			mServer->Touch();
			if(!Read())
			{
				Disconnect();
				return false;
			}
		}

		if(FD_ISSET(mServer->GetSocket(),&wfds))
		{
			mServer->Touch();
			if(!Write())
			{
				Disconnect();
				return false;
			}
		}
	}

/*	for(it = mClients.begin(); it != mClients.end(); it++)
	{
		cli = *it;
		if(cli->IsTimedOut())
		{
			gLog->Stream << cli->GetAccountname() << ": Timed out.";
			gLog->Print();
			Disconnect(cli);
			RemoveList.push_back(cli);
		}
	}
	for (it = RemoveList.begin(); it != RemoveList.end(); it++)
		mClients.remove(*it);*/

	return true;
}

bool Networking::Connect(const string &IP, const int &Port)
{
	sockaddr_in addr;
	SOCKET sock;
		
	addr.sin_addr.s_addr = inet_addr(IP.c_str());
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);

	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == INVALID_SOCKET)
	{
		return false;
	}

#ifdef WIN32
	unsigned long arg = 1;
	ioctlsocket(sock,FIONBIO,&arg);
#endif

	fd_set wfds;
	TIMEVAL tv;
	tv.tv_sec = 0;
	tv.tv_usec = 500000;
	FD_ZERO(&wfds);
	FD_SET(sock,&wfds);
	connect(sock,(sockaddr *)&addr,sizeof(addr));

	int ret = select(FD_SETSIZE,NULL,&wfds,NULL,&tv);
	if(ret <= 0) {
		return false;
	}

	gLog->Stream << "Networking: Connected to " << IP << " on port " << Port;
	gLog->Print();

	if(mServer)
		delete mServer;
	mServer = new Client;
	mServer->Make(sock);
	mServer->Touch();

	return true;
}


void Networking::ServerDataRecvd()
{
	Packet packet;
	
	map<DoubleWord, PacketHandler *>::iterator it;
	
	while(packet.FromInput(mServer))
	{
		it = mPacketHandlers.find(packet.GetOpCode());
		if( it != mPacketHandlers.end())
		{
			if(!(*it).second->Handle(&packet,mServer))
			{
				// Something bad happened, disconnect player here
				mServer->CloseWhenClear();
			}
		}
		else
		{
			// No handler
			gLog->Stream << "OpCode " << packet.GetOpCode() << ": No handler.";
			gLog->Print();
		}
	}
}

void Networking::RegisterHandler(DoubleWord op, PacketHandler *handler)
{
	mPacketHandlers[op] = handler;
}

void Networking::WriteData(const unsigned char byte)
{
	if(!mServer)
		return;
	mServer->SendData(&byte,1);
}

void Networking::WriteData(const unsigned char *bytes, unsigned int len)
{
	if(!mServer)
		return;
	mServer->SendData(bytes,len);
}

void Networking::WriteData(Packet *pack)
{
	if(!mServer)
		return;
	unsigned short len;
	unsigned int op;

	len = htons(4 + pack->GetDataLength());
	op = pack->GetOpCode();
	mServer->SendData((unsigned char *)&len,2);
	mServer->SendData((unsigned char *)&op,4);
	mServer->SendData(pack->GetBytes(),pack->GetDataLength());
}

bool Networking::Start(const string &ConfigFile)
{
	if(!Initialize())
		return false;
	mConfigurationFile = ConfigFile;
	if(!mConfiguration.Load(mConfigurationFile))
	{
		gLog->Stream << "Configuration " << ConfigFile << ": Could not open.";
		gLog->Print();
	}

#ifdef WIN32
	WSADATA wsa;
	int result = WSAStartup(MAKEWORD(2,2),&wsa);
	if(result != NO_ERROR)
	{
		gLog->Print("Network: Error creating WSA.");
		return false;
	}
#endif

	mRunning = true;
	return true;
}

bool Networking::Update()
{
	if(!mRunning)
		return false;
	if(!Select())
	{
#ifdef WIN32
		N("GameNetworking::Update",WSAGetLastError());
#endif
		return false;
	}
	return true;
}

bool Networking::Stop()
{
	if(!mRunning)
		return false;
	Destroy();
	Deinitialize();
	return true;
}