#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

#undef FD_SETSIZE
#define FD_SETSIZE 5000
#include "Winsock2.h"
#include <string>
#include <list>
#include "Client.h"
#include "Macros.h"

class ClientInterface
{
private:
	std::list<Client *> mClients;
public:
	typedef std::list<Client *>::iterator Iterator;
	inline Iterator Begin() { return mClients.begin(); }
	inline Iterator End() { return mClients.end(); }

	inline void Add(Client *cli) { if(cli) { mClients.push_back(cli); } }
	inline ClientInterface::Iterator Remove(Client *cli) { 
		for(Iterator it = Begin(); it != End(); it++) 
		{ if((*it) == cli) { SAFE_DELETE(*it); it = mClients.erase(it); return it; } } return End(); }

	inline UINT NumClients() { return (UINT)mClients.size(); }
};
extern ClientInterface gClients;

#endif