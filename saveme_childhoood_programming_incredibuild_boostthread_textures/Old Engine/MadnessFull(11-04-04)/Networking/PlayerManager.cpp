#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
	Clear();
}

void PlayerManager::Add(Player *ply)
{
	mPlayers[ply->GetGuid()] = ply;
}

void PlayerManager::Remove(Player *ply)
{
	mPlayers.erase(ply->GetGuid());
}

void PlayerManager::Clear()
{
	mPlayers.clear();
}


Player *PlayerManager::Find(QuadWord guid)
{
	Iterator it;

	it = mPlayers.find(guid);
	if(it == mPlayers.end())
		return NULL;
	return (*it).second;
}

Player *PlayerManager::Find(Client *cli)
{
	Iterator it;

	for(it = mPlayers.begin(); it != mPlayers.end(); it++)
	{
		if((*it).second->GetClient() == cli)
			return (*it).second;
	}

	return NULL;
}	

Player *PlayerManager::Find(const char *name)
{
	Iterator it;

	for(it = mPlayers.begin(); it != mPlayers.end(); it++)
	{
		if(stricmp((*it).second->GetName().c_str(), name) == 0)
			return (*it).second;
	}

	return NULL;
}

