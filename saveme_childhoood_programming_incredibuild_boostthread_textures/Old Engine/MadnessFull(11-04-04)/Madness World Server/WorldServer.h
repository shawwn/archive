#ifndef _WORLDSERVER_H
#define _WORLDSERVER_H

#include "Server.h"
#include "LoggingInterface.h"
#include "LoginHandler.h"
#include "ChatHandler.h"

class WorldServer : public Server
{
private:
	bool Initialize();
	void Deinitialize();
	bool CustomUpdate();

	bool CustomSelect();
	bool Authorize();

	LoginHandler mLoginHandler;
	ChatHandler mChatHandler;

	Client *mLoginServer;
	string mName;
public:
	WorldServer();
	~WorldServer();

	Client *LoginServer() { if(mLoginServer) { return mLoginServer; } return NULL; }
};

extern WorldServer *gServer;

#endif
