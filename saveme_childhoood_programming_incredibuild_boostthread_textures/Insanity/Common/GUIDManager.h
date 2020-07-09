#ifndef GUIDMANAGER_H
#define GUIDMANAGER_H

#include "Manager.h"
#include "Types.h"
#include <map>

class GUIDManager : public Manager
{
private:
	std::map<UINT,QuadWord> mTracks;
public:
	GUIDManager();

	typedef std::map<UINT,QuadWord>::iterator Iterator;
	inline Iterator Begin() { return mTracks.begin(); }
	inline Iterator End() { return mTracks.end(); }

	QuadWord Get(UINT Track = 0); 
	QuadWord Current(UINT Track = 0);
};
extern GUIDManager gGUID;

#endif