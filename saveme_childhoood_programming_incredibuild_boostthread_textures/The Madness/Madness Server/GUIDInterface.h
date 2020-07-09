#ifndef GUIDINTERFACE_H
#define GUIDINTERFACE_H

#include "Types.h"

class GUIDInterface
{
private:
	QuadWord mGUID;
public:
	GUIDInterface() : mGUID(0) { }
	QuadWord GetGUID() { return mGUID++; }
};

extern GUIDInterface gGUID;

#endif