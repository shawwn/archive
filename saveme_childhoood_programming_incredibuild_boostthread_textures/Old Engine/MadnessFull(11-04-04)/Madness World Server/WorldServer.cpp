#include "WorldServer.h"

WorldServer *gServer;

WorldServer::WorldServer()
{
	mLoginServer = NULL;
}

WorldServer::~WorldServer()
{
	if(mLoginServer)
		delete mLoginServer;
}

bool WorldServer::Initialize()
{
	mConfigurationFile = "WorldServer.cfg";
	mConfiguration.Load(mConfigurationFile);

	ConfigParameter *param = mConfiguration.Find("Name");
	if(!param)
	{
		mName = "Fever";
	}
	else
		mName = param->Arguments[0];
		
	int Port = 31337;
	param = mConfiguration.Find("LoginServerPort");
	if(param)
	{
		int aret = atoi(param->Arguments[0].c_str());
		if(aret != 0)
			Port = aret;
	}
	param = mConfiguration.Find("LoginServerIP");
	if(!param) {
		gLog->Stream << "Configuration " << mConfigurationFile << ": You must specify a LoginServerIP to connect to in the " << mConfigurationFile << " file.";
		gLog->Print();
		return false;
	}
	string LoginServerIP = param->Arguments[0].c_str();
	mLoginServer = Connect(LoginServerIP.c_str(),Port);
	if(!mLoginServer) {
		gLog->Stream << "World Server " << mName << ": Could not connect to login server.";
		gLog->Print();
		return false;
	}
	else
	{
		gLog->Stream << "World Server " << mName << ": Connected to login server " << LoginServerIP;
		gLog->Print();
	}

	mLoginHandler.Register();
	mChatHandler.Register();

	if(!Authorize())
		return false;

	return true;
}

void WorldServer::Deinitialize()
{
	mName = "";
	if(mLoginServer)
		delete mLoginServer;
	mLoginServer = NULL;
}

bool WorldServer::CustomUpdate()
{
	if(!CustomSelect())
		return false;
	return true;
}

bool WorldServer::CustomSelect()
{
	fd_set rfds, wfds;
	struct timeval tv;
	int cnt;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	
	if(!mLoginServer)
		return false;
	SOCKET LoginSocket = mLoginServer->GetSocket();
	if(mLoginServer->GetSocket() != INVALID_SOCKET)
	{
		FD_SET(LoginSocket,&rfds);
		FD_SET(LoginSocket,&wfds);
	}

	cnt = select(FD_SETSIZE,&rfds,&wfds,NULL,&tv);
	if(FD_ISSET(LoginSocket,&rfds))
		if(!Read(mLoginServer))
			return false;
	if(FD_ISSET(LoginSocket,&wfds))
		if(!Write(mLoginServer))
			return false;
	return true;
}

bool WorldServer::Authorize()
{
	if(!mLoginServer)
		return true;

	Packet pack;
	pack.Build(CMSG_AUTH_SESSION);
	pack.AddWord(1);
	string Username;
	string Password;
	ConfigParameter *param = mConfiguration.Find("WorldServerLoginUsername");
	if(param)
		Username = param->Arguments[0];
	else
		return false;

	param = mConfiguration.Find("WorldServerLoginPassword");
	if(param)
		Password = param->Arguments[0];
	else
		return false;

	int Port;
	param = mConfiguration.Find("ListenPort");
	if(param)
		Port = atoi(param->Arguments[0].c_str());
	else
		return false;
	if(Port == 0) {
		gLog->Stream << "World Server " << mName << ": ListenPort is not valid.";
		gLog->Print();
		return false;
	}

	ConfigParameter *IP = mConfiguration.Find("WorldServerIP");
	if(!IP)
	{
		gLog->Stream << "World Server " << mName << ": You must specify your own IP in the config file.";
		gLog->Print();
		return false;
	}

	pack.AddBytes((Byte *)Username.c_str(),(int)Username.length()+1);
	pack.AddBytes((Byte *)Password.c_str(),(int)Password.length()+1);
	pack.AddBytes((Byte *)mName.c_str(),(int)mName.length()+1);
	pack.AddDoubleWord(inet_addr(IP->Arguments[0].c_str()));
	pack.AddWord(Port);
	WriteData(mLoginServer,&pack);
	return true;
}