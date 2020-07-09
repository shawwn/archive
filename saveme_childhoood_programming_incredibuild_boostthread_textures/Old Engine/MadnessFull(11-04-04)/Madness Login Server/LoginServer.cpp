#include "LoginServer.h"

LoginServer *gServer;

LoginServer::LoginServer()
{
}

LoginServer::~LoginServer()
{

}

bool LoginServer::Initialize()
{
	mConfigurationFile = "LoginServer.cfg";
	mConfiguration.Load(mConfigurationFile);

	mLoginHandler.Register();

	return true;
}

bool LoginServer::CustomUpdate()
{
	if(WorldCount() == 0)
		return true;

	fd_set rfds, wfds;
	WorldIterator it;
	std::list<WorldClient *> RemoveList;
	int cnt;
	struct timeval tv;

	tv.tv_sec = 0;
	tv.tv_usec = 500000;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);

	for(it = mWorldServers.begin(); it != mWorldServers.end(); it++)
	{
		FD_SET((*it)->GetSocket(),&rfds);
		FD_SET((*it)->GetSocket(),&wfds);
	}

	cnt = select(FD_SETSIZE, &rfds, &wfds, NULL, &tv);

	if(cnt < 0)
		return false;
	

	for(it = mWorldServers.begin(); it != mWorldServers.end(); it++)
	{
		if(FD_ISSET((*it)->GetSocket(),&rfds))
		{
			(*it)->Touch();
			if(!Read(*it)) {
				Disconnect(*it);
				RemoveList.push_back(*it);
			}
		}
	}

	for(it = RemoveList.begin(); it != RemoveList.end(); it++)
		mWorldServers.remove(*it);

	for(it = mWorldServers.begin(); it != mWorldServers.end(); it++)
	{
		if(FD_ISSET((*it)->GetSocket(),&wfds))
		{
			(*it)->Touch();
			if(!Write(*it)) {
				Disconnect(*it);
				RemoveList.push_back(*it);
			}
		}
	}

	for(it = RemoveList.begin(); it != RemoveList.end(); it++)
		mWorldServers.remove(*it);

	return true;
}

void LoginServer::TransferToWorldClient(Client *cli, string Name, const DoubleWord &NetworkIP, const Word &Port)
{
	WorldClient *wcli = new WorldClient;
	wcli->Make(cli->GetSocket());
	wcli->SetName(Name);
	wcli->SetIP(NetworkIP);
	wcli->SetPort(Port);
	cli->RemoveWhenClear();
	mWorldServers.push_back(wcli);
}
