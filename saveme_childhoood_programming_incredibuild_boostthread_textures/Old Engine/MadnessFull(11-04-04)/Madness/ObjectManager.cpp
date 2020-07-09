#include "dxstdafx.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
	mAssignGUID = 0;
}

ObjectManager::~ObjectManager()
{
	Clear();
}

void ObjectManager::Add(Object *obj)
{
	if(!obj) 
		return;
	obj->SetGUID(mAssignGUID);
	mObjects[mAssignGUID] = obj;
	mAssignGUID++;
}

void ObjectManager::Remove(Object *obj)
{
	if(!obj) {
		return;
	}
	mObjects.erase(obj->GetGUID());
}

void ObjectManager::Clear()
{
	Iterator it;
	for(it = mObjects.begin(); it != mObjects.end(); it++)
	{
		delete (*it).second;
	}
	mObjects.clear();
}

Object *ObjectManager::Find(QuadWord guid)
{
	Iterator it = mObjects.find(guid);
	if(it == mObjects.end())
		return NULL;
	return (*it).second;
}

Object *ObjectManager::Find(const char *name)
{
	Iterator it;

	for(it = mObjects.begin(); it != mObjects.end(); it++)
	{
		if((*it).second->GetName() == name)
			return (*it).second;
	}

	return NULL;
}