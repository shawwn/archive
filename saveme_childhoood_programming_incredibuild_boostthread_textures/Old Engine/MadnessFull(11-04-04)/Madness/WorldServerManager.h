#ifndef _WORLDSERVERMANAGER_H
#define _WORLDSERVERMANAGER_H

#include <list>
using namespace std;

class WorldServerInfo
{
public:
	string Name;
	int Port;
	long NetworkIP;
};

class WorldServerManager
{
private:
	std::list<WorldServerInfo *> mWorldServers;
public:
	typedef std::list<WorldServerInfo *>::iterator Iterator;

	Iterator Begin() { return mWorldServers.begin(); }
	Iterator End() { return mWorldServers.end(); }
	int Count() { return (int)mWorldServers.size(); }

	void Add(WorldServerInfo *serv);
	void Remove(WorldServerInfo *serv);
	void Clear();

	bool Connect(WorldServerInfo *serv, const string &Username);
};

#endif