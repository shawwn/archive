#ifndef CONTAINERMANAGER_H
#define CONTAINERMANAGER_H

#include <vector>
#include <map>
#include "SmartPointer.h"
#include "Globals.h"

extern AutoPointer gInvalidAutoPointer;

class VectorManager
{
private:
	std::vector<AutoPointer> mContainer;
public:
	VectorManager() { }
	inline UINT Size() { return (UINT)mContainer.size(); }

	UINT Add(const AutoPointer& p) { mContainer.push_back(p); return Size()-1; }
	AutoPointer &Get(UINT Index) { if(Index < Size()) { return mContainer[Index]; } return gInvalidAutoPointer; }
};

template<class Key>
class MapManager
{
private:
	std::map<Key,AutoPointer> mContainer;
public:
	MapManager() { }

	void Add(const Key &k, const AutoPointer &p) { mContainer.insert(std::pair<Key,AutoPointer>(k,p)); }
};

#endif