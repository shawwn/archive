#ifndef TYPES_H
#define TYPES_H

#include <string>

typedef unsigned __int8  Byte;
typedef unsigned __int16 Word;
typedef unsigned __int32 DoubleWord;
typedef unsigned __int64 QuadWord;

#ifndef BYTE
typedef Byte BYTE;
#endif

#ifndef UINT
typedef DoubleWord UINT;
#endif
/*
class CUVECTOR
{
private:
	short theta, phi;
public:
	CUVECTOR() : theta(0), phi(0) { }
	CUVECTOR(const NxVec3 &V) { Compress(V); }
	CUVECTOR(short t, short p) : theta(t), phi(p) { }

	void Compress(const NxVec3 &V);
	NxVec3 Decompress() const;
	short GetTheta() const { return theta; }
	short GetPhi() const {return phi; }
};*/

enum ResourceType
{
	NULLTYPE = 0,
	ENGINE,
	ENGINE_MADNESS,
	ENGINE_DELIRIUM,
	INTERFACE,
	INTERFACE_SETTINGS,
	INTERFACE_CLIENTSETTINGS,
	INTERFACE_SERVERSETTINGS,
	INTERFACE_DIRECT3D,
	MANAGER,
	MANAGER_GUID,
	OBJECT,
	OBJECT_WINDOW,
	OBJECT_PLAYER,
	OBJECT_TEXTURE,
	OBJECT_XMESH
};

static const std::string gResourceTypeToString[] = {
	"NULLTYPE",
	"ENGINE",
	"ENGINE_MADNESS",
	"ENGINE_DELIRIUM",
	"INTERFACE",
	"INTERFACE_SETTINGS",
	"INTERFACE_CLIENTSETTINGS",
	"INTERFACE_SERVERSETTINGS",
	"INTERFACE_DIRECT3D",
	"MANAGER",
	"MANAGER_GUID",
	"OBJECT",
	"OBJECT_WINDOW",
	"OBJECT_PLAYER",
	"OBJECT_TEXTURE",
	"OBJECT_XMESH"
};

inline const std::string &ResourceTypeToString(ResourceType Type) {
	return gResourceTypeToString[Type];
}

#define NOMINMAX

#endif