#include "dxstdafx.h"
#include "ClientNetworking.h"
#include "LoggingInterface.h"

ClientNetworking *gNetworking;

ClientNetworking::ClientNetworking()
{
}

ClientNetworking::~ClientNetworking()
{
}

bool ClientNetworking::Initialize()
{
	mLoginHandler.Register();
	mChatHandler.Register();

	return true;
}

bool ClientNetworking::ConnectToLoginServer(const string &IP, const int &Port, const string &Username, const string &Password)
{
	if(!Connect(IP,Port))
		return false;

	Packet pack;
	pack.Build(CMSG_AUTH_SESSION);
	pack.AddWord(1);
	pack.AddBytes((Byte *)Username.c_str(),(int)Username.length()+1);
	pack.AddBytes((Byte *)Password.c_str(),(int)Password.length()+1);
	WriteData(&pack);

	mUsername = Username;
	mPassword = Password;

	return true;
}

bool ClientNetworking::ConnectToWorldServer(const DoubleWord &IP, const int &Port, const string &Username)
{
	Disconnect();

	if(!Connect(NetworkIPToString(IP),Port)) {
		gLog->Stream << "Networking: Could not connect to worldserver " << NetworkIPToString(IP) << " on port " << Port;
		gLog->Print();
		gNetworking->Stop();
		return false;
	}
    
	Packet pack;
	pack.Build(CMSG_AUTH_WORLD);
	pack.AddBytes((Byte *)Username.c_str(),(int)Username.length()+1);
	gNetworking->WriteData(&pack);

	return true;
}