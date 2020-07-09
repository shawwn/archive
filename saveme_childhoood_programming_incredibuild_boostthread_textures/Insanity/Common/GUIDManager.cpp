#include "GUIDManager.h"
using namespace std;

GUIDManager gGUID;

GUIDManager::GUIDManager() : Manager("GUIDManager",MANAGER_GUID)
{
}

QuadWord GUIDManager::Get(UINT Track)
{
	Iterator found = mTracks.find(Track);
	if(found == End()) {
		mTracks.insert(pair<UINT,QuadWord>(Track,QuadWord()));
	}

	return mTracks[Track]++;
}

QuadWord GUIDManager::Current(UINT Track)
{
	Iterator found = mTracks.find(Track);
	if(found == End()) {
		return 0;
	}

	return (*found).second;
}