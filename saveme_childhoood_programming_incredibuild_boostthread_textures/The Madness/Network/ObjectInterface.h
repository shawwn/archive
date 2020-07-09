#ifndef OBJECTINTERFACE_H
#define OBJECTINTERFACE_H

#include "Object.h"
#include <map>
#include "Parsing.h"
#include "Macros.h"

class ObjectInterface
{
private:
	std::map<QuadWord,Object *> mObjects;
public:
	typedef std::map<QuadWord,Object *>::iterator Iterator;
	inline Iterator Begin() { return mObjects.begin(); }
	inline Iterator End()   { return mObjects.end(); }
	inline UINT Size() { return (UINT)mObjects.size(); }

	inline void Add(Object *obj) { mObjects.insert(std::pair<QuadWord,Object*>(obj->GetGUID(),obj)); }
	inline Iterator Remove(Object *obj) { 
		Iterator it = mObjects.find(obj->GetGUID()); if(it != End()) { SAFE_DELETE((*it).second); it = mObjects.erase(it);  } 
		return it; }
	inline Object *Find(QuadWord GUID) { 
		Iterator it = mObjects.find(GUID); if(it != End()) { return (*it).second; } return NULL; }
	inline Object *Find(const std::string &Name) { 
		for(Iterator it = Begin(); it != End(); it++) { Object *obj = (*it).second; if(Upper(obj->GetName()) == Upper(Name)) { return obj; } } return NULL; }

};

extern ObjectInterface gObjects;

#endif