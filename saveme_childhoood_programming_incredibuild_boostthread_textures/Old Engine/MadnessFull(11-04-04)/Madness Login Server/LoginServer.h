

#ifndef _LOGINSERVER_H
#define _LOGINSERVER_H

#include "Server.h"
#include "LoggingInterface.h"

#include "WorldClient.h"
#include "LoginHandler.h" 

#ifdef WIN32
#pragma comment(lib,"Networking.lib")
#endif

class LoginServer : public Server
{
private:
	std::list<WorldClient *> mWorldServers;

	LoginHandler mLoginHandler;

	bool Initialize();
	bool CustomUpdate();

public:
	LoginServer();
	~LoginServer();

	typedef std::list<WorldClient *>::iterator WorldIterator;

	void TransferToWorldClient(Client *cli, string Name, const DoubleWord &NetworkIP, const Word &Port);
	
	WorldIterator Begin() { return mWorldServers.begin(); }
	WorldIterator End() { return mWorldServers.end(); }
	int WorldCount() { return (int)mWorldServers.size(); }
};

extern LoginServer *gServer;

#endif
