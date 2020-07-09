#ifndef _OBJECTMANAGER_H
#define _OBJECTMANAGER_H

#include <map>
#include "Object.h"

class ObjectManager
{
private:
	QuadWord mAssignGUID;
	std::map<QuadWord, Object *> mObjects;
public:
	ObjectManager();
	~ObjectManager();

	typedef std::map<QuadWord,Object *>::iterator Iterator;

	Iterator Begin() { return mObjects.begin(); }
	Iterator End() { return mObjects.end(); }

	void Add(Object *obj);
	void Remove(Object *obj);
	void Clear();
	size_t Count() { return mObjects.size(); }

	Object *Find(QuadWord guid);
	Object *Find(const char *name);
};

#endif