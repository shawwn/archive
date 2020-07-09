#ifndef PLAYERINTERFACE_H
#define PLAYERINTERFACE_H


#include "Macros.h"
#include <map>
#include "Player.h"
#include "Parsing.H"

class PlayerInterface
{
private:
	std::map<QuadWord,Player *> mPlayers;
public:
	typedef std::map<QuadWord,Player *>::iterator Iterator;
	inline Iterator Begin() { return mPlayers.begin(); }
	inline Iterator End()   { return mPlayers.end(); }

	inline void Add(Player *ply) { mPlayers.insert(std::pair<QuadWord,Player*>(ply->GetGUID(),ply)); }
	inline bool Remove(Player *ply) { 
		Iterator it = mPlayers.find(ply->GetGUID()); if(it != End()) { SAFE_DELETE((*it).second); mPlayers.erase(it); return true; } 
		return false; }
	inline Player *Find(QuadWord GUID) { 
		Iterator it = mPlayers.find(GUID); if(it != End()) { return (*it).second; } return NULL; }
	inline Player *Find(Client *cli) { 
		for(Iterator it = Begin(); it != End(); it++) { Player *ply = (*it).second; if(ply->GetClient() == cli) { return ply; } } return NULL; }
	inline Player *Find(const std::string &Name) { 
		for(Iterator it = Begin(); it != End(); it++) { Player *ply = (*it).second; if(Upper(ply->GetName()) == Upper(Name)) { return ply; } } return NULL; }

};
extern PlayerInterface gPlayers;

#endif