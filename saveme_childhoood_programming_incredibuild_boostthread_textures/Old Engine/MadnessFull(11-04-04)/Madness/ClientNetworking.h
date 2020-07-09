#ifndef _CLIENTNETWORKING_H
#define _CLIENTNETWORKING_H

#include "Networking.h"
#include "Client.h"
#include "LoginHandler.h"
#include "ChatHandler.h"
#include "ChatManager.h"

#pragma comment(lib,"Networking.lib")

class ClientNetworking : public Networking
{
private:
	LoginHandler mLoginHandler;
	ChatHandler mChatHandler;

	ChatManager mChatManager;

	string mUsername;
	string mPassword;
public:
	ClientNetworking();
	~ClientNetworking();

	bool Initialize();

	bool ConnectToLoginServer(const string &IP, const int &Port, const string &Username, const string &Password);
	bool ConnectToWorldServer(const DoubleWord &IP, const int &Port, const string &Username);

	const string &GetUsername() { return mUsername; }
	const string &GetPassword() { return mPassword; }

	ChatManager &Chat() { return mChatManager; }
};

extern ClientNetworking *gNetworking;

#endif
