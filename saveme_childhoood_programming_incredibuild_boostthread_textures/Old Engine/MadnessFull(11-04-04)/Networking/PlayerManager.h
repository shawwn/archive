#ifndef _PLAYERMANAGER_H
#define _PLAYERMANAGER_H

class Player;

#include <map>
#include "Client.h"
#include "Bsd.h"
using namespace std;

class PlayerManager
{
private:
	map<QuadWord, Player *> mPlayers;
public:
	typedef map<QuadWord, Player *>::iterator Iterator;
	PlayerManager();
	~PlayerManager();

	Iterator Begin() { return mPlayers.begin(); }
	Iterator End() { return mPlayers.end(); }

	void Add(Player *ply);
	void Remove(Player *ply);
	void Clear();
	size_t Count() { return mPlayers.size(); }

	Player *Find(QuadWord guid);
	Player *Find(Client *cli);
	Player *Find(const char *name);
};

#endif
