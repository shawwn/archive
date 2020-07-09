#ifndef _OBJECT_H
#define _OBJECT_H

#include <d3dx9.h>
#include <string>
#include "Types.h"
using namespace std;

class Object
{
protected:
	string mName;
	QuadWord mGUID;

	Word mType;
	// 0 - Landscape
	// 1 - Building
	// 2 - Character
	// 3 - Object

	void DestroyObject();
public:
	Object();
	virtual ~Object();

	virtual bool Render(LPDIRECT3DDEVICE9 Device);

	void SetName(const string &Name) { mName = Name; }
	void SetGUID(const QuadWord &GUID) { mGUID = GUID; }
	void SetType(const Word &Type) { mType = Type; }
	void SetType(const string &Type) { 
		if(stricmp(Type.c_str(),"landscape") == 0) { mType = 0; }
		if(stricmp(Type.c_str(),"building") == 0) { mType = 1; }
		if(stricmp(Type.c_str(),"player") == 0) { mType = 2; }
		if(stricmp(Type.c_str(),"object") == 0) { mType = 3; }
	}
	QuadWord GetGUID() { return mGUID; }
	string GetName() { return mName; }
	Word GetType() { return mType; }
};

#endif