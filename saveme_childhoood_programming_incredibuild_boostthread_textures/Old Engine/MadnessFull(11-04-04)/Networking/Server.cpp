#include "stdafx.h"
#include "Server.h"
#include "NetworkError.h"
#include "LoggingInterface.h"

//Server *gServer;

Server::Server()
{
	Destroy();
}

Server::~Server()
{
	Destroy();
}

bool Server::Initialize()
{
	return true;
}

void Server::Deinitialize()
{

}

bool Server::Run()
{	
	
	mRunning = true;
	#ifdef WIN32
	WSADATA wsaData;

	int result = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(result != NO_ERROR)
	{
		gLog->Print("Network: Error creating WSA.");
		return false;
	}
	#endif

	if(!Initialize()) {
		return false;
	}
	Start();

	while(mRunning)
	{
		if(!Update())
			break;
#ifdef WIN32
		//Sleep(1);
#endif
	}
	Stop();

#ifdef WIN32
	WSACleanup();
#endif

	return true;
}

void Server::Destroy()
{
	mConfigurationFile = "Server.cfg";
	mClientCount = 0;
	FD_ZERO(&mRead);
	for(Iterator it = mClients.begin(); it != mClients.end(); it++)
	{
		Disconnect(*it);
	}
#ifdef WIN32
	closesocket(mListen);
#else
	close(mListen);
#endif
	mListen = INVALID_SOCKET;

	mClients.clear();
	mRunning = false;

	mInfo.sin_addr.s_addr = INADDR_ANY;
	mInfo.sin_family = AF_INET;
	mInfo.sin_port = 0;
}

bool Server::Listen(const int &Port)
{
	if(N("GameServer::Start",(mListen = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == INVALID_SOCKET))
	{
		Destroy();
		return false;
	}

	gLog->Stream << "Server: Listening on port " << Port;
	gLog->Print();

	mInfo.sin_family = AF_INET;
	mInfo.sin_addr.s_addr = INADDR_ANY;
	mInfo.sin_port = htons(Port);

	if(N("GameServer::Start::bind",bind(mListen,(sockaddr *)&mInfo,sizeof(sockaddr_in))) != 0)
	{
		gLog->Print("Network: bind() failed.");
		Destroy();
		return false;
	}

	if(N("GameServer::Start::listen",listen(mListen,16)) != 0)
	{
		gLog->Print("Network: listen() failed.");
		Destroy();
		return false;
	}	

	return true;
}

bool Server::Accept()
{
	fd_set rfds;
	struct timeval tv;
	do
	{
		SOCKET s = NSock("GameServer::Update::accept",accept(mListen,NULL,NULL));
		if(s == INVALID_SOCKET)
			return false;
		Client *cli = new Client;
		if(!cli->Make(s))
			return false;
		mClients.push_back(cli);

		mClientCount++;
		ClientConnected(cli);

		FD_ZERO(&rfds);
		FD_SET(mListen,&rfds);

		tv.tv_sec = 0;
		tv.tv_usec = 0;

		if(select(FD_SETSIZE,&rfds,NULL,NULL,&tv) <= 0)
			break;
	} while(FD_ISSET(mListen,&rfds));
	return true;
}

bool Server::Read(Client *cli)
{	
	int read;
	Byte buf[1024];

	while(true)
	{
		read = recv(cli->GetSocket(),(char *)buf,sizeof(buf),0);
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

		cli->RecvData(buf,read);

		if(read < sizeof(buf))
			break;
	}

	ClientDataRecvd(cli);

	return true;
}

bool Server::Write(Client *cli)
{
	int len = send(cli->GetSocket(),cli->GetOutBuffer().c_str(),(int)cli->GetOutBuffer().length(),0);
	if(len == SOCKET_ERROR)
		return false;

	cli->GetOutBuffer() = cli->GetOutBuffer().substr(len);

	if (cli->GetOutBuffer().empty() && cli->GetClose())
		return false;

	return true;
}

bool Server::Disconnect(Client *cli)
{
	mClientCount--;
	ClientDisconnected(cli);
	return true;
}

bool Server::Select()
{	
	fd_set rfds, wfds;
	struct timeval tv;
	int cnt;
	list<Client *>::iterator it;
	Client *cli;
	list<Client *> RemoveList;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);

	if(mListen != INVALID_SOCKET) {
		FD_SET(mListen,&rfds);
		FD_SET(mListen,&wfds);
	}

	for(it = mClients.begin(); it != mClients.end(); it++)
	{
		FD_SET((*it)->GetSocket(),&rfds);
		if(!(*it)->GetOutBuffer().empty())
			FD_SET((*it)->GetSocket(),&wfds);
	}

	cnt = select(FD_SETSIZE,&rfds,&wfds,NULL,&tv);
	if(cnt < 0)
		return false;

	if(FD_ISSET(mListen,&rfds))
		if(!Accept())
			return false;

	for(it = mClients.begin(); it != mClients.end(); it++)
	{
		cli = *it;
		if(FD_ISSET(cli->GetSocket(),&rfds))
		{
			cli->Touch();
			if(!Read(cli))
			{
				Disconnect(cli);
				RemoveList.push_back(cli);
			}
		}
	}
	for(it = RemoveList.begin(); it != RemoveList.end(); it++)
		mClients.remove(*it);
	RemoveList.clear();

	for(it = mClients.begin(); it != mClients.end(); it++)
	{
		cli = *it;
		if(FD_ISSET(cli->GetSocket(),&wfds))
		{
			cli->Touch();
			if(!Write(cli))
			{
				if(!cli->GetRemove())
					Disconnect(cli);
				RemoveList.push_back(cli);
			}
		}
	}
	for(it = RemoveList.begin(); it != RemoveList.end(); it++)
		mClients.remove(*it);
	RemoveList.clear();

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

bool Server::CustomUpdate()
{
	return true;
}

Client *Server::Connect(const char *IP, const int &Port)
{
	sockaddr_in addr;
	SOCKET sock;
		
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);

	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == INVALID_SOCKET)
	{
		gLog->Stream << "Connect to " << IP << " on port " << Port << " failed.";
		gLog->Print();
		return NULL;
	}

#ifdef WIN32
	unsigned long arg = 1;
	ioctlsocket(sock,FIONBIO,&arg);
#endif

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	fd_set connfds;
	FD_ZERO(&connfds);
	FD_SET(sock,&connfds);
	
	connect(sock,(sockaddr *)&addr,sizeof(addr));
/*#ifdef WIN32
	if(ret != WSAEWOULDBLOCK && ret != 0)
	{
		gLog->Stream << "Connect to " << IP << " on port " << Port << " failed.";
		gLog->Print();
		return NULL;
	}
#else
	if(ret != 0)
	{
		gLog->Stream << "Connect to " << IP << " on port " << Port << " failed.";
		gLog->Print();
		return NULL;
	}
#endif*/
	if(select(FD_SETSIZE,NULL,&connfds,NULL,&tv) <= 0) {
		return false;
		gLog->Stream << "Server: Connect to " << IP << " on port " << Port << " failed.";
		gLog->Print();
		return NULL;
	}


	Client *cli = new Client;
	cli->Make(sock);
	cli->Touch();
	return cli;
}

void Server::ClientConnected(Client *cli)
{
	gLog->Stream << "Client " << cli->GetIP() << ": connected from port " << cli->GetPort();
	gLog->Print();
	
}

void Server::ClientDisconnected(Client *cli)
{
	Account *acct = mAccounts.Find(cli);
	if(acct)
	{
		acct->SetClient(NULL);
	}
#ifdef WIN32
	closesocket(cli->GetSocket());
#else
	close(cli->GetSocket());
#endif
/*	if(cli->IsWorldServer())
	{
		gLog->Stream << "Worldserver " << cli->GetIP() << ": disconnected form port " << cli->GetPort();
		gLog->Print();
		mWorldServer = NULL;
		mHasWorldServer = false;
		return;
	}*/

	gLog->Stream << "Client " << cli->GetIP() << ": disconnected from port " << cli->GetPort();
	gLog->Print();
}

void Server::ClientDataRecvd(Client *cli)
{
	Packet packet;
	
	map<DoubleWord, PacketHandler *>::iterator it;
	
	while(packet.FromInput(cli))
	{
		it = mPacketHandlers.find(packet.GetOpCode());
		if( it != mPacketHandlers.end())
		{
			if(!(*it).second->Handle(&packet,cli))
			{
				// Something bad happened, disconnect player here
				cli->CloseWhenClear();
			}
		}
		else
		{
			// No handler
			gLog->Print("No handler.");
		}
	}
}

void Server::RegisterHandler(DoubleWord op, PacketHandler *handler)
{
	mPacketHandlers[op] = handler;
}

void Server::WriteData(Client *cli, const unsigned char byte)
{
	cli->SendData(&byte,1);
}

void Server::WriteData(Client *cli, const unsigned char *bytes, unsigned int len)
{
	cli->SendData(bytes,len);
}

void Server::WriteData(Client *cli, Packet *pack)
{
	unsigned short len;
	unsigned int op;

	len = htons(4 + pack->GetDataLength());
	op = pack->GetOpCode();
	cli->SendData((unsigned char *)&len,2);
	cli->SendData((unsigned char *)&op,4);
	cli->SendData(pack->GetBytes(),pack->GetDataLength());
}

void Server::SendPacketTo(Player *ply, Packet *pack)
{
	WriteData(ply->GetClient(),pack);
}

bool Server::Start()
{
	if(!mConfiguration.IsLoaded())
	{
		mConfigurationFile = "Server.cfg";
		if(!mConfiguration.Load(mConfigurationFile)) {
			gLog->Stream << "Configuration " << mConfigurationFile << ": Could not open.";
			gLog->Print();
		}
	}

	int Port = 31337;
	ConfigParameter *param = mConfiguration.Find("ListenPort");
	if(param != NULL)
		Port = atoi(param->Arguments[0].c_str());
	if(Port == 0)
		Port = 31337;

	if(!Listen(Port))
		return false;
	return true;
}

bool Server::Update()
{
	if(!Select())
	{
		N("GameServer::Update",WSAGetLastError());
		return false;
	}
	if(!CustomUpdate()) {
		N("GameServer::CustomUpdate",WSAGetLastError());
		return false;
	}
	return true;
}

bool Server::Stop()
{
	if(!mRunning)
		return false;
	Destroy();
	Deinitialize();
	return true;
}
