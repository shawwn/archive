#include "dxstdafx.h"
#include "WorldServerManager.h"

void WorldServerManager::Add(WorldServerInfo *serv)
{
	if(!serv)
		return;
	mWorldServers.push_back(serv);
}

void WorldServerManager::Remove(WorldServerInfo *serv)
{
	Iterator it = mWorldServers.begin();
	while(it != mWorldServers.end())
	{
		if((*it)->NetworkIP == serv->NetworkIP && (*it)->Port == serv->Port)
		{
			delete (*it);
			it = mWorldServers.erase(it);
		}
		else
			it++;
	}
}

void WorldServerManager::Clear()
{
	for(Iterator it = Begin(); it != End(); it++)
		delete (*it);
	mWorldServers.clear();
}

bool WorldServerManager::Connect(WorldServerInfo *serv, const string &Username)
{
	return true;
}